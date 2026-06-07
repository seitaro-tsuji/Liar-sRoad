#include "AllHeader.h"

#define jamp_height_min	28		//最小ジャンプ
#define player_fall_speed	6	//落下スピード
#define Player_raise_speed  6	//上昇スピード
#define Player_walk_speed	4	//左右移動のスピード　ダッシュは2倍の速度で移動する

static int m_cnt;	//cnt変数　ジャンプの処理に用いているので注意
static int check_cnt;		//中間地点到達時のメッセージ用のcnt変数
static int player_IsRunning;	//プレイヤーが移動しているなら1になる変数　画像表示用

static int image_player_stop_right;	//右向き静止プレイヤー
static int image_player[2][3];		//プレイヤーの画像セット　左or右向き、静止or移動orジャンプで計6通りの画像

//プレイヤー関連の初期化関数
void PlayerInitialaize() {
	image_player_stop_right = LoadGraph("画像/棒人間/棒人間＿右向き静止.png");
	image_player[0][0] = LoadGraph("画像/棒人間/棒人間＿左向き静止.png");
	image_player[0][1] = LoadGraph("画像/棒人間/棒人間＿左向き移動.png");
	image_player[0][2] = LoadGraph("画像/棒人間/棒人間＿左向きジャンプ.png");
	image_player[1][0] = LoadGraph("画像/棒人間/棒人間＿右向き静止.png");
	image_player[1][1] = LoadGraph("画像/棒人間/棒人間＿右向き移動.png");
	image_player[1][2] = LoadGraph("画像/棒人間/棒人間＿右向きジャンプ.png");
}

//ジャンプの処理をする関数
void PlayerJamp(player_t *player, map_t *map) {
	//ジャンプのキーを受け付ける処理
	if (player->jamp_phase == 0) {
		if (Keyboard_Get(KEY_INPUT_Z) == 1 || Keyboard_Get(KEY_INPUT_W) == 1 || Keyboard_Get(KEY_INPUT_UP) == 1) {
			player->jamp_phase = 1;
			m_cnt = 0;
		}
		else if(PlayerGoWhileAble(player,down, *map, 1) == 1)
			player->jamp_phase = 3;
	}
	else if (player->jamp_phase == 1) {
		m_cnt++;
		player->jamp = jamp_height_min;
		if ((Keyboard_Get(KEY_INPUT_W) == 0 && Keyboard_Get(KEY_INPUT_UP) == 0 && Keyboard_Get(KEY_INPUT_Z) == 0) || m_cnt >= 35)
			player->jamp_phase = 2;
	}
	else if (player->jamp_phase == 2) {
		player->jamp -= Player_raise_speed;	//jamp変数を-4(raise_speed)
		if (player->jamp <= 0)
			player->jamp_phase = 3;
	}
	else if (player->jamp_phase == 3) {
		int fall_d = PlayerGoWhileAble(player, down, *map, player_fall_speed);//落下距離を代入する変数
		if (fall_d != player_fall_speed)
			player->jamp_phase = 0;	//max距離落下していないならフェーズ0に戻る
	}

	//jamp変数が1以上ならプレイヤーを上に上げる
	if (player->jamp > 0) {
		//jamp変数があるのに上に最大まで移動できなかった場合の処理
		if (PlayerGoWhileAble(player, up, *map, Player_raise_speed) != Player_raise_speed) {
			player->jamp = 0;
			player->jamp_phase = 3;

			//何のブロックに当たったかでそれぞれの処理をする
			//当たったブロックと座標を取得する処理がいるかも
			if (map->map_d[player->y / 32 - 1][player->x / 32] == '?') { //アイテムブロックは破壊不可能ブロックに変化(現状コインしか出ない)
				map->map_d[player->y / 32 - 1][player->x / 32] = 'b';
				map->map_d[player->y / 32 - 2][player->x / 32] = 'c';
			}
			if (map->map_d[player->y / 32 - 1][player->x / 32 + 1] == '?') {
				map->map_d[player->y / 32 - 1][player->x / 32 + 1] = 'b';
				map->map_d[player->y / 32 - 2][player->x / 32 + 1] = 'c';
			}

			if (map->map_d[player->y / 32 - 1][player->x / 32] == '!')//トラップブロックはダメージブロックに変化
				map->map_d[player->y / 32 - 1][player->x / 32] = 'd';
			if (map->map_d[player->y / 32 - 1][player->x / 32 + 1] == '!')
				map->map_d[player->y / 32 - 1][player->x / 32 + 1] = 'd';

			/*if (map->map_d[player->y / 32 - 1][player->x / 32] == 'b')//破壊可能なブロックは消滅
				map->map_d[player->y / 32 - 1][player->x / 32] = '.';
			if (map->map_d[player->y / 32 - 1][player->x / 32 + 1] == 'b')
				map->map_d[player->y / 32 - 1][player->x / 32 + 1] = '.';*/

			if (map->map_d[player->y / 32 - 1][player->x / 32] == 'i')//透明ブロックは地面ブロックに変化
				map->map_d[player->y / 32 - 1][player->x / 32] = 'b';
			if (map->map_d[player->y / 32 - 1][player->x / 32 + 1] == 'i')
				map->map_d[player->y / 32 - 1][player->x / 32 + 1] = 'b';
		}
	}
}

//コインを回収する関数
void GetCoin(player_t *player, map_t *map) {
	int x = player->x / Blocksize;
	int y = player->y / Blocksize;

	//動かないコインの回収　プレイヤーを中心に縦横3×2マスのコインを回収する処理　図を書くと3×2が最適という結論になった
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
			if (map->map_d[y + i][x + j] == 'c') {	//コインのマスであれば回収する
				map->map_d[y + i][x + j] = '.';	//コインマスから何もないマスに変更
				player->coin += 1;
			}
			if (map->map_d[y + i][x + j] == 'k') {	//k(重要そうなアイテム)のマスであれば回収する
				map->map_d[y + i][x + j] = '.';	//何もないマスに変更
				player->keyitem += 1;
			}
		}
	}
	//動くコインを追加した場合はここに処理を追加する
}

//プレイヤーの移動などをする関数
void PlayerUpdate(player_t *player, map_t *map, int camera_x) {
	player_IsRunning = 0;	//移動判定は0に

	//左右移動の処理(左へはカメラの範囲外へは行けない)
	if (Keyboard_Get(KEY_INPUT_RIGHT) || Keyboard_Get(KEY_INPUT_D)) {
		PlayerGoWhileAble(player, right, *map, Player_walk_speed);
		player_IsRunning = 1;
		player->dire = 1;	//方向は右
	}
	if (Keyboard_Get(KEY_INPUT_LEFT) || Keyboard_Get(KEY_INPUT_A)) {
		if ((player->x > camera_x) || (player->sta == 1 &&  player->x <= 0)) {//カメラの範囲外へは出られないようにする ステージ1の左端のみ例外
			PlayerGoWhileAble(player, left, *map, Player_walk_speed);
		}
		player_IsRunning = 1;
		player->dire = 0;	//方向は左
	}

	//ジャンプ処理
	PlayerJamp(player, map);

	//コインを回収する処理
	GetCoin(player, map);

	//中間地点を通ったときの処理(判定の幅は10マス)
	if (map->checkpoint_x <= player->x && player->x <= map->checkpoint_x + 10 && player->checkflag[player->sta] == 0) {
		player->checkflag[player->sta] = 1;
		check_cnt = 1;
	}
	//中間地点のメッセージ表示用の処理
	if (check_cnt >= 1) {
		check_cnt++;
		if (check_cnt >= 300) {//300フレームでcntが0になる
			check_cnt = 0;
		}
	}
}

//プレイヤーの画像を描画する関数
void PlayerDraw(player_t player, int camera_x,int camera_y) {
	//プレイヤーの移動状態　画像表示のため　0は静止　1は地上移動　2は空中
	int player_state = 0;
	if (player.jamp_phase != 0)
		player_state = 2;
	else if (player_IsRunning == 1)
		player_state = 1;

	DrawGraph(player.x - camera_x, player.y - camera_y, image_player[player.dire][player_state], TRUE);

	//中間地点のメッセージ
	if (check_cnt >= 1) {
		DrawFormatString(400, 0, green, "中間地点に到達!!");
	}
}