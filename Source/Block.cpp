#include "DxLib.h"
#include "Player.h"
#include "Block.h"
#include "Stage2.h"

//座標がブロック内部なら1を返す
int Block::IsInsideBlockXY(int px, int py) {
	if (x <= px && px <= x + x_size - 1 && y <= py && py <= y + y_size - 1)
		return 1;
	else
		return 0;
}

//プレイヤーがブロックに埋まっていたら1を返す
int Block::P_isInsideBlock(Player player) {
	int judge[6];//当たり判定6箇所
	judge[0] = IsInsideBlockXY(player.x, player.y);
	judge[1] = IsInsideBlockXY(player.x, player.y + Blocksize);
	judge[2] = IsInsideBlockXY(player.x, player.y + 2 * Blocksize - 1);
	judge[3] = IsInsideBlockXY(player.x + Blocksize - 1, player.y);
	judge[4] = IsInsideBlockXY(player.x + Blocksize - 1, player.y + Blocksize - 1);
	judge[5] = IsInsideBlockXY(player.x + Blocksize - 1, player.y + 2 * Blocksize - 1);

	if (judge[0] + judge[1] + judge[2] + judge[3] + judge[4] + judge[5] != 0)	return 1;
	else return 0;
}

//moveflagが1の時にブロックの移動をする関数
void Block::Update(Player *player) {
	if (moveflag == 0) {
		//triggerの範囲内にプレイヤーが触れるとmoveflag=1になる(ブロックによってはダメージブロックに変化)
		if (trigger_x1 - player->x_size + 1 <= player->x && player->x <= trigger_x2 && trigger_y1 - player->y_size + 1 <= player->y && player->y <= trigger_y2) {
			moveflag = 1;
		}
	}
	else if (moveflag == 1) {//ブロックが動く　動いた先にプレイヤーがいたらプレイヤーをずらして移動する　結果プレイヤーがめり込んだら死亡
		MoveDirection(speed, movedire);

		//ダメージブロックとプレイヤーが重なったらその時点で死亡処理(あとでmoveflagの外側に変更する)
		if (P_isInsideBlock(*player) == 1 && m_kind == needleblock)
			player->deathcause = 5;//とげなら死因5

		int player_movedis = 0;
		while (P_isInsideBlock(*player) == 1 && player_movedis <= speed) {//プレイヤーが埋まっていたら出るまで移動
			player->MoveDirection(1, movedire);
			player_movedis++;
		}
	}
}

//ブロックを叩いたか判定 1なら叩いた
int Block::KnockJudge(Player player) {
	if (player.y == y + y_size && x - player.x_size + 1 <= player.x && player.x <= x + x_size - 1)
		return 1;
	return 0;
}

//ブロックを叩く処理(継承用)
void Block::Knock(char mapdata[mapheight][mapwidth], Player *player) {

}

//描画関数
void Block::Draw(int c_x, int c_y) {
	for (int i = 0; i < y_size / Blocksize; i++) {
		for (int j = 0; j < x_size / Blocksize; j++) {
			DrawGraph(x - c_x + j * Blocksize, y - c_y + i * Blocksize, image_handle, TRUE);
		}
	}
}