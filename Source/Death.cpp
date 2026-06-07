#include "AllHeader.h"

static int death_place;	//死んだ場所

static int image_blackbox;	//真っ黒の画像

void DeathInitialize() {
	image_blackbox = LoadGraph("画像/真っ黒画面.png");
}

//触れると死亡するマスかどうか判定する関数 1以上なら死ぬマス
int GetDeath(char s) {
	if (s == 'd') {	//dに触れると死亡 3を返す
		return 3;
	}
	else if (s == 'n')	//nに触れると死亡　2
		return 2;	
	else
		return 0;
}

//プレイヤーが死亡判定のあるブロックに重なっているか判定する関数
int DeathBlockCheck(player_t player, map_t map) {
	int x = player.x;
	int y = player.y;
	int deathjudge[10];	//死亡判定の配列(上下2箇所、左右3か所ずつの判定を格納する)

	//動かないブロックの死亡判定を配列に格納する
	deathjudge[0] = GetDeath(map.map_d[(y - 1) / Blocksize][x / Blocksize]);
	deathjudge[1] = GetDeath(map.map_d[(y - 1) / Blocksize][(x + Blocksize - 1) / Blocksize]);
	deathjudge[2] = GetDeath(map.map_d[y / Blocksize][(x - 1) / Blocksize]);
	deathjudge[3] = GetDeath(map.map_d[(y + Blocksize) / Blocksize][(x - 1) / Blocksize]);
	deathjudge[4] = GetDeath(map.map_d[(y + 2 * Blocksize - 1) / Blocksize][(x - 1) / Blocksize]);
	deathjudge[5] = GetDeath(map.map_d[y / Blocksize][(x + Blocksize) / Blocksize]);
	deathjudge[6] = GetDeath(map.map_d[(y + Blocksize) / Blocksize][(x + Blocksize) / Blocksize]);
	deathjudge[7] = GetDeath(map.map_d[(y + 2 * Blocksize - 1) / Blocksize][(x + Blocksize) / Blocksize]);
	deathjudge[8] = GetDeath(map.map_d[(y + Blocksize) / Blocksize][x / Blocksize]);
	deathjudge[9] = GetDeath(map.map_d[(y + Blocksize) / Blocksize][(x + Blocksize - 1) / Blocksize]);
	//完成した配列を使用して死亡判定を行う(最大値を求める)
	int death = 0;
	for (int i = 0; i < 10; i++) {
		death = deathjudge[i] > death ? deathjudge[i] : death;
	}
	//死亡判定配列の最大値が0でない時はその値を返す
	if (death != 0)
		return death;		//動かないブロックの死亡判定終了

	//動くブロックの死亡判定
	for (int i = 0; i < map.move_block_num; i++) {
		int xb = map.moveblock[i].x;
		int yb = map.moveblock[i].y;
		direction dire = map.moveblock[i].dire;

		//block[i]が落下ブロックの時の死亡判定
		if (map.moveblock[i].kind == fall) {
			if (xb - Blocksize + 1 <= x && x <= xb + map.moveblock[i].x_size - 1 && yb <= y && y <= yb + map.moveblock[i].y_size)
				return 4;
		}

		//block[i]がとげのブロックのときの死亡判定 返り値は2
		if (map.moveblock[i].kind == needle || map.moveblock[i].kind == stopneedle) {
			if (dire == left && xb - Blocksize <= x && x <= xb && yb - Blocksize * 2 <= y && y <= yb + map.moveblock[i].y_size - 2 * Blocksize)//左とげの当たり判定
				return 2;
			if (dire == up && xb - Blocksize <= x && x <= xb + map.moveblock[i].x_size && yb - Blocksize * 2 <= y && y <= yb + map.moveblock[i].y_size)//上とげの当たり判定
				return 2;
		}
	}
	return 0;
}

//プレイヤーの死亡判定をする関数 死亡したら1,2,...を返す
//1 落下　2 とげ　3 トラップブロック　4 落下ブロックにあたる
int DeathCheck(player_t player, map_t map) {
	int deathflag;
	if (player.y >= 480)	//画面の下側に行けば1
		return 1;
	else if ((deathflag = DeathBlockCheck(player, map)) != 0)	//死亡判定のブロックに当たればdeathflagの値を返す
		return deathflag;
	else
		return 0;
}

//死亡したときの画面表示の処理をする関数
void DeathDraw(player_t player, int deathflag) {
	if (deathflag != 0) {
		//死亡したときは画面を暗くする
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 192);
		DrawModiGraph(0, 0, 640, 0, 640, 480, 0, 480, image_blackbox, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		//DrawFormatString(200, 80, red, "deathflag=%d", deathflag);
		DrawFormatString(150, 100, red, "死んでしまった...");

		//死因の表示
		if (deathflag == 1) {//フラグ1　落下死の時
			DrawFormatString(350, 100, red, "死因:落下死");
		}
		if (deathflag == 2) {//フラグ2　とげによる死の時
			DrawFormatString(350, 100, red, "死因:とげ");
		}
		if (deathflag == 3) {
			DrawFormatString(350, 100, red, "死因:トラップブロック");
		}
		if (deathflag == 4) {
			DrawFormatString(350, 100, red, "死因:落下ブロック");
		}

		//特定の死に方をしたときの表示
		if (player.x / Blocksize == 12 || player.x / Blocksize == 13)
			DrawFormatString(200, 150, yellow2, "アイテムブロックを信用しないこと");
		if (40 <= player.x / Blocksize && player.x / Blocksize <= 42 && player.y / Blocksize >= 13)
			DrawFormatString(150, 150, yellow2, "隠しブロックに引っかかった??");
		if (40 <= player.x / Blocksize && player.x / Blocksize <= 42 && player.y / Blocksize <= 5)
			DrawFormatString(200, 150, yellow2, "コインにつられたね。");
		if (player.x / Blocksize == 71 || player.x / Blocksize == 70)
			DrawFormatString(200, 150, yellow2, "こんな見え見えの罠にかかるなんて...");
		if (107 <= player.x / Blocksize && player.x / Blocksize <= 110 && player.y / Blocksize >= 10)
			DrawFormatString(200, 150, yellow2, "深読みしちゃったかな??");
		if (132 <= player.x / Blocksize && player.x / Blocksize <= 133)
			DrawFormatString(150, 150, yellow2, "スタート地点の右矢印、信じきっていないか?");
		if (player.keyitem == 1)
			DrawFormatString(200, 150, yellow2, "重要そうなアイテムだけど意味ないよ");

		DrawFormatString(200, 300, red, "Zキー:タイトルに戻る");
	}
}