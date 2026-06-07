#include "Stage2.h"
#include "Player.h"
#include "Block.h"
#include "Map2.h"
#include "CommonVari.h"
#include "Keyboard.h"

#define PlayerWalkSpeed	4	//プレイヤーの左右移動のスピード
#define PlayerFallSpeed 6	//プレイヤーの落下スピード
#define PlayerRaiseSpeed 6	//プレイヤーの上昇スピード

static int check_cnt;	//中間地点を通過したときのcnt

static int image_blackbox;	//真っ黒画面

//コンストラクタ(ステージに入った瞬間に初期化の処理をする)
Stage::Stage(int snumber, int startcheck) {
	stage_number = snumber;

	cnt = 0;
	map = new Map(snumber);	//マップの配置
	if (startcheck == 0) {
		player = new Player(2 * Blocksize, 12 * Blocksize);//プレイヤーをスタート地点に設置
		check_flag = 0;
	}
	else {
		player = new Player(map->checkpoint_x, 12 * Blocksize);//プレイヤーを中間地点に設置
		check_flag = 1;
	}

	//画像のハンドル
	image_blackbox = LoadGraph("画像/真っ黒画面.png");
}

Stage::~Stage() {
	delete player;
	delete map;
}

//中間地点の通過確認　中間地点に関する処理をすべてここでする
void Stage::Check_Check() {
	if (check_flag == 0) {
		check_cnt = 0;
		if (player->x >= map->checkpoint_x) {
			check_flag = 1;
			check_cnt = 1;
		}
	}
	if (1<=check_cnt && check_cnt <= 300) {
		check_cnt++;
		DrawFormatString(370, 0, green, "中間地点に到達しました");
	}
}


//キャラクターの死亡判定 1以上なら死亡 1落下死　2無し　3トラップブロック　4押しつぶし　5とげ　6偽ゴール
int Stage::DeathCheck() {
	if (player->y >= 480)	return 1;	//落下死
	else if (player->deathcause >= 3) return player->deathcause;	//特殊ブロックによる死亡
	else if (P_IsAbleToMoveXY(player->x, player->y) == 0) {//移動できない位置にプレイヤーがいたら押しつぶされた判定
		return 4;
	}
	else if (map->fake_goal_x - player->x_size + 1 <= player->x && player->x <= map->fake_goal_x + 2 * Blocksize - 1 && map->fake_goal_y - player->y_size + 1 <= player->y && player->y <= map->fake_goal_y + 2 * Blocksize - 1)
		return 6;
	return 0;
}

//死亡時の演出と操作　Zキーを押すとこの関数の返り値が1になってPlay関数の返り値も1になる
int Stage::DeathDraw(int deathflag) {
	//Zキーを押すとタイトルに戻る
	if (Keyboard_Get(KEY_INPUT_Z) == 1)	return 1;

	//画面を暗くする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 192);
	DrawGraph(0, 0, image_blackbox, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if (deathflag == 1)		DrawFormatString(170, 150, red, "死んでしまった...。　　死因:落下死");
	else if (deathflag == 3)		DrawFormatString(130, 150, red, "死んでしまった...。　　死因:トラップブロック");
	else if (deathflag == 4)		DrawFormatString(150, 150, red, "死んでしまった...。　　死因:押しつぶし");
	else if (deathflag == 5)		DrawFormatString(180, 150, red, "死んでしまった...。　　死因:とげ");
	else if (deathflag == 6)			DrawFormatString(180, 150, red, "死んでしまった...。　　死因:????");

	//特定の場所で死亡したら専用のコメントを残す
	if (5 * Blocksize + 1 <= player->x && player->x <= 8 * Blocksize - 1)
		DrawFormatString(70, 200, yellow2, "ステージ2からは落下ブロックの見た目を変えられるようになりました");
	else if (11 * Blocksize + 1 <= player->x && player->x <= 14 * Blocksize - 1 && player->y <= 14 * Blocksize - 1)
		DrawFormatString(170, 200, yellow2, "途中で加速もできるようになりました");
	else if (50 * Blocksize + 1 <= player->x && player->x <= 57 * Blocksize && player->y <= 12 * Blocksize - 1)
		DrawFormatString(250, 200, yellow2, "ジャンプ読みトゲ");
	else if(deathflag == 6)
		DrawFormatString(220, 200, yellow2, "GOALではなくGOA1だった…。");

	DrawFormatString(230, 300, red, "Zキー:タイトルに戻る");

	return 0;
}

//ステージのクリア判定 1ならクリア
int Stage::ClearCheck() {
	if (map->goal_x <= player->x + player->x_size - 1 && player->x <= map->goal_x + 2 * Blocksize - 1 && map->goal_y <= player->y + player->y_size - 1 && player->y <= map->goal_y + 2 * Blocksize - 1)
		return 1;
	else 
		return 0;
}

int Stage::ClearDraw() {
	//Zキーを押すとタイトルに戻る
	if (Keyboard_Get(KEY_INPUT_Z) == 1)
		return 1;

	DrawGraph(0, 0, image_blackbox, TRUE);
	DrawFormatString(270, 200, white, "ク リ ア !!");


	DrawFormatString(230, 300, white, "Zキー:タイトルに戻る");

	return 0;
}

//指定の座標にプレイヤーが移動できるか 0なら不可、1なら可、2なら下以外から可、3なら死亡する
int Stage::P_IsAbleToMoveXY(int p_x, int p_y) {
	if (p_x < map->camera_x)
		return 0;

	int judge[6];	//プレイヤーの当たり判定6箇所
	judge[0] = map->MoveAbleXY(p_x, p_y);
	judge[1] = map->MoveAbleXY(p_x, p_y + Blocksize);
	judge[2] = map->MoveAbleXY(p_x, p_y + Blocksize * 2 - 1);
	judge[3] = map->MoveAbleXY(p_x + Blocksize - 1, p_y);
	judge[4] = map->MoveAbleXY(p_x + Blocksize - 1, p_y + Blocksize);
	judge[5] = map->MoveAbleXY(p_x + Blocksize - 1, p_y + Blocksize * 2 - 1);

	//デバッグ
	//for (int i = 0; i < 6; i++) {
	//	DrawFormatString(130 + 16 * (i / 3), (i % 3) * 20, green, "%d", judge[i]);
	//}

	//当たり判定の6箇所で1つでも0があったらそれを返す
	for (int i = 0; i < 6; i++)
		if (judge[i] == 0) {
			//if (judge[i] >= 3)	player->deathcause = judge[i];//3以上なら死亡なのでその値をplayerに渡す
			return 0;
		}

	//当たり判定0,3が2なら2を返す
	if (judge[0] == 2 || judge[3] == 2)
		return 2;

	return 1;//上のifにかからなければ1
}

//プレイヤーが移動できるだけ移動する関数 移動した距離を返す
int Stage::PlayerMoveWhileAble(int z, direction d) {
	if (d == down) {//移動先が0でなければ可
		for (int i = z; i >= 0; i--)
			if (P_IsAbleToMoveXY(player->x, player->y + i)) {
				player->Move(0, i);
				return i;
			}
	}
	else if (d == left) {//移動先が0でなければ可
		for (int i = z; i >= 0; i--)
			if (P_IsAbleToMoveXY(player->x - i, player->y)) {
				player->Move(-i, 0);
				return i;
			}
	}
	else if (d == right) {//移動先が0でなければ可
		for (int i = z; i >= 0; i--)
			if (P_IsAbleToMoveXY(player->x + i, player->y)) {
				player->Move(i, 0);
				return i;
			}
	}
	else if (d == up) {//移動先が1なら可
		for (int i = z; i >= 0; i--)
			if (P_IsAbleToMoveXY(player->x, player->y - i) == 1) {
				player->Move(0, -i);
				return i;
			}
	}
}

//プレイヤーの操作関数
void Stage::PlayerControl() {
	player->dashing = 0;
	//左右移動
	if (Action_Keyboard_Get(a_moveleft)) {
		PlayerMoveWhileAble(PlayerWalkSpeed, left);
		player->dire = left;
		player->dashing = 1;
	}
	if (Action_Keyboard_Get(a_moveright)) {
		PlayerMoveWhileAble(PlayerWalkSpeed, right);
		player->dire = right;
		player->dashing = 1;
	}

	//ジャンプ、降下処理
	player->Jump();//jump変数の操作
	if (player->jump > 0) {
		player->jump -= PlayerRaiseSpeed;	//上昇分だけjump変数を消費
		if (PlayerMoveWhileAble(PlayerRaiseSpeed, up) != PlayerRaiseSpeed) {//上に移動　何かに突っかかったらjump0、jump_phase3に移行
			player->jump = 0;
			player->jump_phase = 3;
			//全ての特殊ブロックにたたく判定を与える
			for (int i = 0; i < map->block_num; i++)
				map->block[i]->Knock(map->map_ground, player);
		}
	}
	else {
		player->jump_phase = 3;
		if (PlayerMoveWhileAble(PlayerFallSpeed, down) != PlayerFallSpeed) {
			player->jump_phase = 0;
		}
	}
}

//カメラの位置の更新
void Stage::Camera_Update() {
	//カメラの位置の計算(カメラは前に戻れないようにする)
	int camera_x_past = map->camera_x;	//1フレーム前のカメラ座標
	map->camera_x = max(player->x - 315, 0);
	if (map->camera_x < camera_x_past)	map->camera_x = camera_x_past;
}

//プレイヤーの描画
void Stage::PlayerDraw() {
	//DrawGraph(player->x - map->camera_x, player->y - map->camera_y, player->image[0], TRUE);
	if (player->dire == left) {//左向き
		if (player->jump_phase == 0) {//地上にいるとき
			if(player->dashing == 1)//移動中
				DrawGraph(player->x - map->camera_x, player->y - map->camera_y, player->image[4], TRUE);
			else //静止中
				DrawGraph(player->x - map->camera_x, player->y - map->camera_y, player->image[3], TRUE);
		}
		else //空中
			DrawGraph(player->x - map->camera_x, player->y - map->camera_y, player->image[5], TRUE);
	}
	else {//右向き
		if (player->jump_phase == 0) {//地上にいるとき
			if (player->dashing == 1)//移動中
				DrawGraph(player->x - map->camera_x, player->y - map->camera_y, player->image[1], TRUE);
			else //静止中
				DrawGraph(player->x - map->camera_x, player->y - map->camera_y, player->image[0], TRUE);
		}
		else //空中
			DrawGraph(player->x - map->camera_x, player->y - map->camera_y, player->image[2], TRUE);
	}
}

//プレイ関数 返り値が1になればタイトルに戻る
int Stage::Play() {
	cnt++;
	//死亡判定、中間通過判定、クリア判定
	int deathflag = DeathCheck();
	Check_Check();
	int clearflag = ClearCheck();

	if (cnt >= 2 && deathflag == 0 && clearflag == 0) {//最初の1フレームは何もしない
		//プレイヤーとマップの更新、描画関数
		map->Update(player);//マップ更新
		PlayerControl();//プレイヤーを動かす関数
		player->GetCoin(map);

		//カメラの位置の更新をする
		Camera_Update();

		//map->Draw(cnt);
		//PlayerDraw();//プレイヤーの描画
		//なぜか上の2行をこのif内に入れると描画がおかしくなる
		//↑の理由は死んだ瞬間にこのif文がスキップされるから
	}

	map->Draw(cnt);
	PlayerDraw();//プレイヤーの描画
	if (cnt == 1)
		DrawGraph(0, 0, image_blackbox, TRUE);

	//操作説明
	DrawFormatString(0, 0, white, "A,D,←,→:左右移動\nW,↑,Z:ジャンプ");

	//コインの枚数
	DrawFormatString(0, 0, yellow, "\n\nコイン:%d枚", player->coin);

	if (deathflag) {
		if (DeathDraw(deathflag)) {//死亡画面でZキーを押したら
			if (check_flag == 1)//中間地点に到達してたら2
				return 2;
			else//してなければ1を返す
				return 1;
		}
	}

	if (clearflag) {
		if (ClearDraw())
			return 2;
	}

	//デバッグ
	//DrawFormatString(0, 0, green, "\n(x,y)=(%d,%d)\njump_phase=%d\njump_jump=%d\nknock_flag=%d\ncheck_flag=%d"
	//	, player->x, player->y, player->jump_phase, player->jump, map->block[4]->flag, check_flag);

	return 0;
}

Stage* sta; //ステージのデータ

//ステージ2以降の開始処理 startpointはスタート地点からのスタートか中間からのスタートか  0ならスタートから　1なら中間から
void Start_Stagefrom2(int snumber, int startpoint) {
	sta = new Stage(snumber, startpoint);//ステージを生成する
}

//ステージ2以降のプレイ処理
void Play_Stagefrom2(int *scene, int *check_flag) {
	int result = sta->Play();
	if (result == 1) {//死亡したとき
		*scene = title;
		delete sta;
	}
	else if (result == 2) {//中間地点到達後に死亡したとき
		*scene = title;
		delete sta;
		*check_flag = 1;
	}
}