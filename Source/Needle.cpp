#include "DxLib.h"
#include "Needle.h"
#include "Player.h"

Needle::Needle(int bx, int by, int bx_size, int by_size, int tx1, int ty1, int tx2, int ty2, direction dire, int sp, int movenumber) {
	x = bx;
	y = by;
	x_size = bx_size;
	y_size = by_size;
	trigger_x1 = tx1;	trigger_y1 = ty1;
	trigger_x2 = tx2;	trigger_y2 = ty2;
	movedire = dire;
	speed = sp;//スピード10
	moveflag = 0;
	m_kind = needleblock;
	if (dire == up)
		image_handle = LoadGraph("画像/上とげ.png");
	else if (dire == down)
		image_handle = LoadGraph("画像/下とげ.png");
	else if (dire == left)
		image_handle = LoadGraph("画像/左とげ.png");
	else
		image_handle = LoadGraph("画像/右とげ.png");
	movepattern = movenumber;//動きの種類　0は通常　1はフェイント 2は低速から斜め移動
	cnt = 0;

}

//Blockクラスからオーバーライド　フェイントなど特殊な動きを実装するときはこの関数内に書く
void Needle::Update(Player* player) {
	if (moveflag == 0) {
		//triggerの範囲内にプレイヤーが触れるとmoveflag=1になる(ブロックによってはダメージブロックに変化)
		if (trigger_x1 - player->x_size + 1 <= player->x && player->x <= trigger_x2 && trigger_y1 - player->y_size + 1 <= player->y && player->y <= trigger_y2) {
			moveflag = 1;
		}
	}
	else if (moveflag == 1) {//ブロックが動く
		cnt++;
		if (movepattern == 2)	speed = 1;	//movepattern2なら最初は低速

		MoveDirection(speed, movedire);

		if (movepattern == 1)//movepatternが1なら上にも移動する
			MoveDirection(speed, up);

		//ダメージブロックとプレイヤーが重なったらその時点で死亡処理(あとでmoveflagの外側に変更する)
		if (P_isInsideBlock(*player) == 1)
			player->deathcause = 5;//とげなら死因5

		//movepattern2ならとげの前方にプレイヤーがいると斜め上に移動するように変化する
		if (movepattern == 2 && x <= player->x && player->x <= x + 3 * Blocksize && y-4*Blocksize <= player->y && player->y <= y + y_size) {
			speed = 8;
			movepattern = 1;
		}
	}
}