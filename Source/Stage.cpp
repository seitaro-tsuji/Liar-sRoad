#include "AllHeader.h"

static int camera_x, camera_x_past;	//画面に表示する範囲の左端の(map上での)x座標
static int death_flag;		//死亡していれば1以上になるフラグ
static int ereachenge_flag;	//エリアチェンジ時に1以上になるフラグ
static int clear_flag;		//クリアすると1以上になるフラグ
static int blackout_cnt;	//暗転時に使うcnt変数

map_t map;

//ステージ関連の初期化関数
void StageInitialize() {

}

//ステージ開始時の関数checkflagは中間フラグで1なら中間地点からスタート
void StageStart(player_t *player, int Checkflag) {
	player->x = 3 * Blocksize;
	player->y = 12 * Blocksize - 1;
	player->jamp_phase = 0;
	player->jamp = 0;
	camera_x = 0;
	player->keyitem = 0;
	//player->x = 90 * Blocksize;
	//player->y = 1 * Blocksize;//デバッグ用
	MapCpy(map.map_d, mapdata[player->sta - 1]);
	//あとでプレイヤーの画像をセットする処理を入れる
	if (player->sta == 1) {
		if (Checkflag == 1) {
			player->x = 70 * Blocksize;
			player->y = 12 * Blocksize;
		}

		map.move_block_num = 14;
		map.moveblock[0] = { 35 * Blocksize, 11 * Blocksize, Blocksize, Blocksize * 3, waitneedle, left };
		map.moveblock[1] = { 44 * Blocksize, 11 * Blocksize, Blocksize * 2, Blocksize * 4, waitfall };
		map.moveblock[2] = { 46 * Blocksize, 11 * Blocksize, Blocksize * 2, Blocksize * 4, waitfall };
		map.moveblock[3] = { 48 * Blocksize, 11 * Blocksize, Blocksize * 2, Blocksize * 4, waitwaitfall };
		map.moveblock[4] = { 63 * Blocksize, 14 * Blocksize, Blocksize * 4, Blocksize, waitfall };
		map.moveblock[5] = { 91 * Blocksize, 14 * Blocksize, Blocksize, Blocksize, waitfall };
		map.moveblock[6] = { 94 * Blocksize, 14 * Blocksize, Blocksize * 3, Blocksize, waitfall };
		//map.moveblock[7] = { 97 * Blocksize, 10 * Blocksize, Blocksize, Blocksize, stopneedle, up };
		map.moveblock[8] = { 105 * Blocksize, 5 * Blocksize, Blocksize, Blocksize * 6, waitneedle, left };
		map.moveblock[9] = { 107 * Blocksize, 14 * Blocksize, Blocksize * 4, Blocksize, waitfall };
		map.moveblock[10] = { 115 * Blocksize, 7 * Blocksize, Blocksize * 4, Blocksize, waitfall };
		//map.moveblock[11] = { 123 * Blocksize, 6 * Blocksize, Blocksize * 3, Blocksize, waitfall };
		map.moveblock[12] = { 125 * Blocksize, 7 * Blocksize, Blocksize * 4, Blocksize, waitfall };
		map.moveblock[13] = { 121 * Blocksize, 14 * Blocksize, Blocksize, Blocksize, waitfall };

		map.checkpoint_x = 70 * Blocksize;
		map.goal_x = 143 * Blocksize;
		map.goal_y = 7 * Blocksize;
	}
}

//ステージ関連の動作処理関数
void StageUpdate(player_t* player, int *scene) {
	//カメラの位置の計算(カメラは前に戻れないようにする)
	camera_x_past = camera_x;	//1フレーム前のカメラ座標
	camera_x = max(player->x - 315, 0);
	if (camera_x < camera_x_past)	camera_x = camera_x_past;
	
	//死亡していない時
	if (death_flag == 0 && ereachenge_flag == 0 && clear_flag == 0) {
		//マップの変化などの処理
		MapUpdate(player, &map);

		//プレイヤーの移動などの処理
		PlayerUpdate(player, &map, camera_x);

		//エリアチェンジの判定
		ereachenge_flag = EreaChengeJudge(*player, map);

		//ゴール判定
		clear_flag = ClearJudge(*player, map);
	}
	else if (death_flag) {
		if (Keyboard_Get(KEY_INPUT_Z) == 3)//タイトル画面でZが2フレーム押されるとステージに入るから3にした
			*scene = title;	//死亡した後にZキーでタイトルに戻る
	}
	else if (clear_flag) {
		*scene = clear;
		clear_flag = 0;
	}

	//キャラクターの死亡判定を行う
	death_flag = DeathCheck(*player, map);

	//エリアチェンジを行う
	if (ereachenge_flag != 0) {
		if (EreaChengeDo(player, map, ereachenge_flag))
			ereachenge_flag = 0;	//移動処理が終了したら0に戻す
	}
}

//ステージ関連の描画処理関数
void StageDraw(player_t player) {
	//プレイヤーの描画
	PlayerDraw(player, camera_x, 0);

	//マップの描画
	MapDraw(player, camera_x, 0, map);

	//死亡判定がされたときはその描画をする
	DeathDraw(player,death_flag);

	//エリアチェンジの処理中は暗転の描画
	if (ereachenge_flag != 0) {
		EreaChengeDraw();
	}
}