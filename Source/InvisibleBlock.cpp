#include "InvisibleBlock.h"
#include "Player.h"

//コンストラクタ　itemはアイテムナンバー(現状コイン(0)のみ)
InvisibleBlock::InvisibleBlock(int bx, int by, int itemkind) {
	x = bx;
	y = by;
	x_size = Blocksize;
	y_size = Blocksize;
	m_kind = invisibleblock;
	moveflag = -1;//動かない
	damageflag = 0;
	isabletoknock = 1;//叩ける
	trigger_x1 = trigger_x2 = trigger_y1 = trigger_y2 = 0;
	item = itemkind;
	knock_flag = 0;//未回収の状態
	image_handle = 0;//透明
	cnt = 15;
	flag = 0;
}

void InvisibleBlock::Update(Player *player) {
	knock_flag = 0;
}

void InvisibleBlock::Knock(char mapdata[mapheight][mapwidth], Player *player) {
	//このクラスのみ叩いてから15フレーム後にknockflagを1にする
	if (KnockJudge(*player) == 1) {//プレイヤーがブロックを叩いたら
		if (flag == 0) {
			//cnt = 1;
			image_handle = LoadGraph("画像/ブロック.png");
			mapdata[y / Blocksize - 1][x / Blocksize] = 'c';//1つ上のマスにコインを配置
		}
	}
	if (cnt > 0)
		cnt++;
	if (cnt == 15)
		flag = 1;
}