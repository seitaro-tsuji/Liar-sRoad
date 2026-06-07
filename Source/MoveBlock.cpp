#include "MoveBlock.h"
#include "Player.h"

//コンストラクタ　imagenumberは見た目のパターン(0ならステージ1と同様地面ブロック　1ならアイテムブロック)
//movepatternは挙動のパターン(0は普通　1は途中で加速　2は上下で挙動が変わる　3は8フレーム(2マス分)だけ移動して終了  4は60フレーム(15マス分)移動して停止  5は480フレーム移動したら3倍速になる)
MoveBlock::MoveBlock(int bx, int by, int bx_size, int by_size, int tx1, int ty1, int tx2, int ty2, direction dire, int spe, int imagenumber,int movenumber) {
	x = bx;
	y = by;
	x_size = bx_size;
	y_size = by_size;
	trigger_x1 = tx1;	trigger_y1 = ty1;
	trigger_x2 = tx2;	trigger_y2 = ty2;
	movedire = dire;
	speed = spe;//スピード
	moveflag = 0;
	cnt = 0;

	imagepattern = imagenumber;
	if (imagepattern == 0) {
		image_handle = LoadGraph("画像/地面1.png");
		image_handle2 = 0;
	}
	else if (imagepattern == 1) {
		image_handle = LoadGraph("画像/地面1.png");
		image_handle2 = LoadGraph("画像/アイテムブロック.png");
	}
	else if (imagepattern == 2) {
		image_handle = LoadGraph("画像/左矢印.png");
	}

	movepattern = movenumber;//動きの種類　ふつうは0
}

void MoveBlock::Update(Player* player) {
	if (moveflag == 0) {
		//triggerの範囲内にプレイヤーが触れるとmoveflag=1になる(ブロックによってはダメージブロックに変化)
		if (trigger_x1 - player->x_size + 1 <= player->x && player->x <= trigger_x2 && trigger_y1 - player->y_size + 1 <= player->y && player->y <= trigger_y2) {
			moveflag = 1;
			cnt = 0;

			if (movepattern == 2 && player->y + player->y_size == y) {//movepatternが2の時、上に乗った場合のみ上向き2倍になる
				movedire = up;
				speed *= 2;
			}
		}
	}
	else if (moveflag == 1) {//ブロックが動く　動いた先にプレイヤーがいたらプレイヤーをずらして移動する　結果プレイヤーがめり込んだら死亡
		cnt++;
		MoveDirection(speed, movedire);

		int player_movedis = 0;
		while (P_isInsideBlock(*player) == 1 && player_movedis <= speed) {//プレイヤーが埋まっていたら出るまで移動
			player->MoveDirection(1, movedire);
			player_movedis++;
		}

		if (movepattern == 1 && cnt == 70)//movepatternが1の時、70フレーム経過で6倍速になる
			speed *= 6;
		if (movepattern == 3 && cnt == 8)//movepatternが3の時、8フレームで止まる
			moveflag = -1;
		if (movepattern == 4 && cnt == 60)//movepatternが4の時4×15フレームで止まる
			moveflag = -1;
		if (movepattern == 5 && cnt == 480)//movepatternが5なら480フレームで3倍速になる
			speed *= 3;
	}
}

//オーバーライドした描画関数
void MoveBlock::Draw(int c_x, int c_y) {
	if (imagepattern == 0) {
		for (int i = 0; i < y_size / Blocksize; i++) {
			for (int j = 0; j < x_size / Blocksize; j++) {
				DrawGraph(x - c_x + j * Blocksize, y - c_y + i * Blocksize, image_handle, TRUE);
			}
		}
	}
	else if (imagepattern == 1) {
		DrawGraph(x - c_x, y - c_y, image_handle, TRUE);
		DrawGraph(x - c_x + Blocksize, y - c_y, image_handle2, TRUE);
		DrawGraph(x - c_x +Blocksize * 2, y - c_y, image_handle, TRUE);
	}
	else if (imagepattern == 2) {
		DrawGraph(x - c_x, y - c_y, image_handle, TRUE);
	}
}