#include "TrapBlock.h"
#include "Player.h"

TrapBlock::TrapBlock(int bx, int by) {
	x = bx;
	y = by;
	x_size = Blocksize;
	y_size = Blocksize;
	m_kind = trapblock;
	moveflag = -1;//動かない
	damageflag = 0;
	isabletoknock = 1;//叩ける
	trigger_x1 = trigger_x2 = trigger_y1 = trigger_y2 = 0;
	knock_flag = 0;//未回収の状態
	image_handle = LoadGraph("画像/アイテムブロック.png");
	image_handle2 = LoadGraph("画像/ダメージブロック下.png");
}

void TrapBlock::Update(Player player) {

}

void TrapBlock::Knock(char mapdata[mapheight][mapwidth], Player *player) {
	if (KnockJudge(*player) == 1) {//プレイヤーがブロックを叩いたら
		if (knock_flag == 0) {
			knock_flag = 1;
			damageflag = 1;
			player->deathcause = 3;//死因トラップブロック
			//image_handle = LoadGraph("画像/ブロック.png");
		}
	}
}

void TrapBlock::Draw(int c_x, int c_y) {
	DrawGraph(x - c_x, y - c_y, image_handle, TRUE);
	if(knock_flag == 1)
		DrawGraph(x - c_x, y - c_y, image_handle2, TRUE);
}