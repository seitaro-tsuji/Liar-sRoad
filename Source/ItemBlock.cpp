#include "DxLib.h"
#include "ItemBlock.h"
#include "Player.h"

//コンストラクタ 大きさは1×1固定 itemkindは中のアイテムの種類 0:コイン 1:階段出現
ItemBlock::ItemBlock(int bx, int by, int itemkind) {
	x = bx;
	y = by;
	x_size = Blocksize;
	y_size = Blocksize;
	m_kind = itemblock;
	moveflag = -1;//動かない
	damageflag = 0;
	isabletoknock = 1;//叩ける
	trigger_x1 = trigger_x2 = trigger_y1 = trigger_y2 = 0;
	item = itemkind;
	knock_flag = 0;//未回収の状態
	image_handle = LoadGraph("画像/アイテムブロック.png");
}

//ブロックを叩く
void ItemBlock::Knock(char mapdata[mapheight][mapwidth], Player *player) {
	if (KnockJudge(*player) == 1) {//プレイヤーがブロックを叩いたら
		if (knock_flag == 0) {
			knock_flag = 1;
			image_handle = LoadGraph("画像/ブロック.png");
			if (item == 0)
				mapdata[y / Blocksize - 1][x / Blocksize] = 'c';//1つ上のマスにコインを配置
			else if (item == 1) {
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j <= 3 * i; j++) {
						mapdata[y / Blocksize + 6 - j][x / Blocksize + 2 + i] = 'g';
						mapdata[y / Blocksize + 6 - j][x / Blocksize + 2 + 10 - i] = 'g';
					}
				}
			}
			else if (item == 2) {
				for (int i = 0; i < 15; i++)
					mapdata[i][x / Blocksize + 5] = 'g';
			}
		}
	}
}