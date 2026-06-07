#include "AllHeader.h"

static int c_cnt;	//cnt変数

static int image_blackbox;	//真っ黒画面

//初期化関数
void ClearInitialize() {
	image_blackbox = LoadGraph("画像/真っ黒画面.png");
}

//クリア判定の関数 クリアすれば1を返す
int ClearJudge(player_t player, map_t map) {
	if (map.goal_x - Blocksize <= player.x && player.x <= map.goal_x + 2 * Blocksize &&
		map.goal_y - 2 * Blocksize <= player.y && player.y <= map.goal_y + 2 * Blocksize)
		return 1;
	else 
		return 0;
}

//クリア画面の処理関数
void ClearUpdate(player_t* player, int *scene) {
	c_cnt++;
	if (Keyboard_Get(KEY_INPUT_Z) == 1 && c_cnt >= 2) {
		c_cnt = 0;
		*scene = title;//タイトルに戻る
		player->degree = 2;
	}
}

//クリア画面の描画関数
void ClearDraw() {
	DrawGraph(0, 0, image_blackbox, TRUE);
	DrawFormatString(300, 200, white, "ク リ ア !!");
}