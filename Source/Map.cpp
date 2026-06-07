#include "AllHeader.h"

static int cnt;	//1フレームで+1　mapupdate関数で
static int m_cnt;	//cnt変数　画像の表示時に点滅させるため

static int ground_block1;
static int needle_block1;
static int image_coin;
static int image_goal[2];
static int image_keyitem[2];
static int image_itemblock;
static int image_block;
static int image_trapblock;
static int image_right;//右矢印

//map関連の初期化関数
void MapInitialize() {
	ground_block1 = LoadGraph("画像/地面1.png");
	needle_block1 = LoadGraph("画像/左とげ.png");
	image_coin = LoadGraph("画像/コイン.png");
	image_goal[0] = LoadGraph("画像/ゴールスター1.png");
	image_goal[1] = LoadGraph("画像/ゴールスター2.png");
	image_itemblock = LoadGraph("画像/アイテムブロック.png");
	image_block = LoadGraph("画像/ブロック.png");
	image_trapblock = LoadGraph("画像/ダメージブロック下.png");
	image_right = LoadGraph("画像/右矢印.png");
	image_keyitem[0] = LoadGraph("画像/キーアイテム1.png");
	image_keyitem[1] = LoadGraph("画像/キーアイテム2.png");
}

//map1にmap2をコピーする関数
void MapCpy(char map1[mapheight][mapwidth], const char map2[mapheight][mapwidth]) {
	for (int i = 0; i < mapheight; i++) {
		for (int j = 0; j < mapwidth; j++) {
			map1[i][j] = map2[i][j];
		}
	}
}

//進めるマスかどうか判定する関数(下の進行可能か判定する関数内で使用する)　下からは1なら進める　下以外からは1,2なら進める
int IsAbleToGoBlock(char s) {
	if (s == 'g' || s == 'b' || s == '!' || s == '?' || s == 'd' || s == 'n')	return 0;	//g,b,!,?,d,nなら進めない
	else if (s == 'i') return 2;	//iなら下以外から進める
	else	return 1;
}

//x,y座標を与えるとmap上のその座標に進めるかを返す関数
int IsAbleToGoXY(int x, int y, map_t map) {
	//まず動くブロックがないか確かめる
	for (int i = 0; i < map.move_block_num; i++) {
		block_t block = map.moveblock[i];
		int x2 = x - block.x;	//見やすくする用の変数
		int y2 = y - block.y;
		if (0 <= x2 && x2 <= block.x_size - 1 && 0 <= y2 && y2 <= block.y_size)	//blockの内部に座標x,yがあれば0を返す
			return 0;
	}

	//動くブロックがなければ動かないブロックの種類によって返す数値を決める
	return IsAbleToGoBlock(map.map_d[y / 32][x / 32]);
}

//プレイヤーが指定された方向と距離に進めるだけ進む関数 進んだ距離を返す
int PlayerGoWhileAble(player_t *player, direction dire, map_t map, int z) {
	int x = player->x;
	int y = player->y;
	if (dire == right) {		
		for (int i = 0; i < z; i++) {
			//右に進める最大だけ進む
			if (IsAbleToGoXY(x + Blocksize + z - i - 1, y, map) != 0 && IsAbleToGoXY(x + Blocksize + z - i - 1, y + Blocksize - 1, map) != 0
				&& IsAbleToGoXY(x + Blocksize + z - i - 1, y + Blocksize, map) != 0 && IsAbleToGoXY(x + Blocksize + z - i - 1, y + Blocksize * 2 - 1, map) != 0) {
				player->x += z - i;
				return z - i;
			}
		}
	}
	else if (dire == left) {
		for (int i = 0; i < z; i++) {
			//左に進める最大だけ進む(x座標0のときのみ左端から外に行ける)
			if (player->x <= 0) {
				player->x -= z;
				return z;
			}
			else if (IsAbleToGoXY(x - z + i, y, map) != 0 && IsAbleToGoXY(x - z + i, y + Blocksize - 1, map) != 0
				&& IsAbleToGoXY(x - z + i, y + Blocksize, map) != 0 && IsAbleToGoXY(x - z + i, y + Blocksize * 2 - 1, map) != 0) {
				player->x -= z - i;
				return z - i;
			}
		}
	}
	else if (dire == up) {
		for (int i = 0; i < z; i++) {
			//上に進める最大だけ進む
			if (IsAbleToGoXY(x, y - z + i, map) == 1 && IsAbleToGoXY(x + Blocksize - 1, y - z + i, map) == 1) {
				player->y -= z - i;
				return z - i;
			}
		}
	}
	else if (dire == down) {
		for (int i = 0; i < z; i++) {
			//下に進める最大だけ進む
			if (IsAbleToGoXY(x, y + Blocksize * 2 - 1 + z - i, map) != 0 && IsAbleToGoXY(x + Blocksize - 1, y + Blocksize * 2 - 1 + z - i, map) != 0) {
				player->y += z - i;
				return z - i;
			}
		}
	}
	return 0;
}

//マップの変化の処理をする関数
void MapUpdate(player_t *player, map_t *map) {
	m_cnt++;
	for (int i = 0; i < map->move_block_num; i++) {
		block_t block = map->moveblock[i];

		//fallのマスを1フレームに10ずつ下に移動する
		if (block.kind == fall)
			map->moveblock[i].y += 10;

		//waitfallのマスの上にプレイヤーが3/4以上乗ったらfallに変化する
		if (block.kind == waitfall) {
			if ((player->y + Blocksize * 2) >= block.y && block.x - Blocksize * 3 / 4 <= player->x && player->x <= block.x + block.x_size - Blocksize / 4)
				map->moveblock[i].kind = fall;
		}

		//waitwaitfallのマスの上にプレイヤーが乗って30フレーム経過したらwaitfallに変化する
		if (block.kind == waitwaitfall) {
			if ((player->y + Blocksize * 2) >= block.y && block.x - Blocksize  * 3 / 4 <= player->x && player->x <= block.x + block.x_size - Blocksize / 4)
				cnt++;
			else
				cnt = 0;
			if (cnt >= 30) {
				map->moveblock[i].kind = waitfall;
				cnt = 0;
			}
		}

		//needleのマスを1フレームに10ずつ左に移動する
		if (block.kind == needle) {
			map->moveblock[i].x -= 10;
		}

		//waitneedleのマスの左側の一定範囲(前4マス、高さ2倍分)に入るとneedleに変化する
		if (block.kind == waitneedle) {
			if (block.x - 4 * Blocksize <= player->x && player->x <= block.x && block.y - block.y_size <= player->y && player->y <= block.y + block.y_size) {
				map->moveblock[i].kind = needle;
			}
		}
	}
}

//マップの描画をする関数　ブロックは全部でg,b,c,i,?,!,d
void MapDraw(player_t player, int camera_x, int camera_y, map_t map) {
	if (player.sta == 1) {
		//動かないブロックの描画
		for (int i = 0; i < mapheight; i++) {
			for (int j = 0; j < mapwidth; j++) {
				if (map.map_d[i][j] == 'g') {//地面ブロック
					DrawGraph(j * 32 - camera_x, i * 32, ground_block1, TRUE);
				}
				else if (map.map_d[i][j] == 'b')//破壊不可能ブロック
					//DrawBox(j * 32 - camera_x, i * 32, j * 32 + 32 - camera_x, i * 32 + 32, white, TRUE);
					DrawGraph(j * 32 - camera_x, i * 32, image_block, TRUE);//地面ブロックの描画
				else if (map.map_d[i][j] == 'i')//透明ブロック
					;
				else if (map.map_d[i][j] == 'c')//コイン
					DrawGraph(j * Blocksize - camera_x, i * Blocksize, image_coin, TRUE);
				else if (map.map_d[i][j] == 'k')//キーアイテム
					DrawGraph(j * Blocksize - camera_x, i * Blocksize, image_keyitem[(m_cnt / 10) % 2], TRUE);
				else if (map.map_d[i][j] == '?' || map.map_d[i][j] == '!')//アイテムブロック、トラップブロック
					DrawGraph(j * Blocksize - camera_x, i * Blocksize, image_itemblock, TRUE);
				else if (map.map_d[i][j] == 'd') {//ダメージブロック
					DrawGraph(j * Blocksize - camera_x, i * Blocksize, image_itemblock, TRUE);
					DrawGraph(j * Blocksize - camera_x, i * Blocksize, image_trapblock, TRUE);//追加でとげの描画を入れる
				}
				else if (map.map_d[i][j] == 'n')//とげブロック
					DrawGraph(j * 32 - camera_x, i * 32, needle_block1, TRUE);//とげブロックの描画
			}
		}
		//動くブロックの描画
		for (int i = 0; i < map.move_block_num; i++) {
			if (map.moveblock[i].kind == waitwaitfall || map.moveblock[i].kind == waitfall || map.moveblock[i].kind == fall) {//落ちる床　見た目は地面ブロックと同様
				for (int j = 0; j < map.moveblock[i].y_size / Blocksize; j++) {
					for (int k = 0; k < map.moveblock[i].x_size / Blocksize; k++) {
						DrawGraph(map.moveblock[i].x - camera_x + k * Blocksize, map.moveblock[i].y + j * Blocksize, ground_block1, TRUE);
					}
				}
			}
			if (map.moveblock[i].kind == waitneedle || map.moveblock[i].kind == needle || map.moveblock[i].kind == stopneedle) {//動くとげ,動かないとげ　見た目はとげ付きブロック
				for (int j = 0; j < map.moveblock[i].y_size / Blocksize; j++) {
					for (int k = 0; k < map.moveblock[i].x_size / Blocksize; k++) {
						if (map.moveblock[i].dire == left)//左向き
							DrawGraph(map.moveblock[i].x - camera_x + k * Blocksize, map.moveblock[i].y + j * Blocksize, needle_block1, TRUE);
						else if (map.moveblock[i].dire == up)//上向き
							DrawGraph(map.moveblock[i].x - camera_x + k * Blocksize, map.moveblock[i].y + j * Blocksize, needle_block1, TRUE);
					}
				}
			}
		}

		//ゴールの表示 点滅演出
		DrawFormatString(map.goal_x - camera_x, map.goal_y - camera_y - 20, white, "G O A L");
		DrawGraph(map.goal_x - camera_x, map.goal_y - camera_y, image_goal[(m_cnt / 10) % 2], TRUE);

		//右矢印の表示 
		DrawGraph(4 * Blocksize - camera_x, 9 * Blocksize - camera_y, image_right, TRUE);

		//操作説明
		DrawFormatString(0, 0, white, "A,D,←,→:左右移動\nW,↑,Z:ジャンプ");

		//コインの枚数
		DrawFormatString(0, 0, yellow, "\n\nコイン:%d枚", player.coin);

		//デバック用の数値
		//DrawFormatString(0, 0, white, "jamp_phase = %d\nplayer.x,y=%d,%d\nplayer.coin=%d\n", player.jamp_phase, player.x, player.y, player.coin);
		//DrawFormatString(0, 0, white, "\n\n\n中間フラグ:%d", player.checkflag[player.sta]);
	}
}

//　　動かないブロック一覧
//g 地面ブロック
//b 破壊できるブロック
//c コイン
//k 重要そうなアイテム　意味はない
//i	透明ブロック
//? アイテムブロック
//!	トラップブロック
//d	ダメージブロック
//n とげブロック(左)