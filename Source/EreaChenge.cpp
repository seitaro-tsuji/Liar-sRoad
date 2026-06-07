#include "AllHeader.h"

static int ec_step;	//ステップ変数
static int ec_cnt;	//cnt変数

static int image_blackbox;	//真っ黒画面

//初期化関数
void EreaChengeInitialize() {
	ec_step = 0;
	ec_cnt = 0;

	image_blackbox = LoadGraph("画像/真っ黒画面.png");
}

//エリアチェンジの判定を行う関数　チェンジナンバー(do関数で使用する)を返す 0なら移動しないからフラグとしても使用可能
int EreaChengeJudge(player_t player, map_t map) {
	//チェンジナンバー1　ステージ1の左端
	if (player.sta == 1 && player.x <= -32) {
		return 1;
	}
	else
		return 0;
}
 
//エリアチェンジの処理をする関数 終了すれば1を返す
int EreaChengeDo(player_t* player, map_t map, int chenge_number) {
	if (ec_step == 0) {
		ec_cnt++;
		if (ec_cnt >= 30) {//30フレームで次ステップ
			ec_step++;
			ec_cnt = 0;
		}
	}
	if (ec_step == 1) {
		//ナンバー1　ステージ1の左端
		if (chenge_number == 1) {
			player->x = 135 * Blocksize;
			player->y = 0 * Blocksize;
		}

		ec_step++;//処理したら次ステップ
	}
	if (ec_step == 2) {
		ec_cnt++;
		if (ec_cnt >= 30) {//30フレームで次ステップ
			ec_step = 0;
			ec_cnt = 0;
			return 1;	//終了
		}
	}
	return 0;//未終了
}

//エリアチェンジの描画をする関数
void EreaChengeDraw() {
	//DrawFormatString(0, 48, white, "エリアチェンジ中");
	if (ec_step == 0) {//暗転処理
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ec_cnt* 8);
		DrawGraph(0, 0, image_blackbox, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	else if (ec_step == 1) {
		DrawGraph(0, 0, image_blackbox, TRUE);
	}
	else if (ec_step == 2) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - ec_cnt * 8);
		DrawGraph(0, 0, image_blackbox, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}