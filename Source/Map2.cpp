#include "DxLib.h"
#include "Player.h"
#include "Block.h"
#include "ItemBlock.h"
#include "InvisibleBlock.h"
#include "TrapBlock.h"
#include "MoveBlock.h"
#include "Needle.h"
#include "Map2.h"
#include "CommonVari.h"

using namespace std;

static int image_ground;
static int image_coin;
static int image_goal[2];
static int image_key[2];

void Map_Ini(Map* map) {
	image_ground = LoadGraph("画像/地面1.png");
	
}

//コンストラクタ　ブロックの設置など
Map::Map(int snumber) {
	//画像の設定
	image_ground = LoadGraph("画像/地面1.png");
	image_coin = LoadGraph("画像/コイン.png");
	image_goal[0] = LoadGraph("画像/ゴールスター1.png");
	image_goal[1] = LoadGraph("画像/ゴールスター2.png");
	image_key[0] = LoadGraph("画像/キーアイテム1.png");
	image_key[1] = LoadGraph("画像/キーアイテム2.png");
	//カメラの初期位置の設定
	set_camera(0, 0);

	//地面ブロックの設置をする
	if (snumber == 2)
		for (int i = 0; i < mapheight; i++)
			for (int j = 0; j < mapwidth; j++)
				map_ground[i][j] = map_ground_data_stage2[i][j];
	else if (snumber == 3)
		for (int i = 0; i < mapheight; i++)
			for (int j = 0; j < mapwidth; j++)
				map_ground[i][j] = map_ground_data_stage3[i][j];

	//特殊ブロックの配置
	if (snumber == 2) {
		block_num = 0;
		block[0] = new MoveBlock(1 * Blocksize, 4 * Blocksize, 3 * Blocksize, 1 * Blocksize, 1 * Blocksize, 0, 4 * Blocksize - 1, 15 * Blocksize, down); block_num++;
		block[1] = new MoveBlock(0 * Blocksize, 14 * Blocksize, 1 * Blocksize, 1 * Blocksize, 0 * Blocksize, 14*Blocksize-1, 1 * Blocksize - 1, 15 * Blocksize, down); block_num++;
		block[2] = new MoveBlock(6 * Blocksize, 9 * Blocksize, 2 * Blocksize, 1 * Blocksize, 6 * Blocksize, 9 * Blocksize, 8 * Blocksize - 1, 15 * Blocksize, down, 8, 2); block_num++;
		block[3] = new MoveBlock(12 * Blocksize, 14 * Blocksize, 2 * Blocksize, 1 * Blocksize, 12 * Blocksize, 14 * Blocksize-1, 14 * Blocksize - 1, 15 * Blocksize, up, 3, 0, 1); block_num++;
		block[4] = new InvisibleBlock(19 * Blocksize, 6 * Blocksize); block_num++;
		block[5] = new InvisibleBlock(20 * Blocksize, 2 * Blocksize); block_num++;
		block[6] = new MoveBlock(30 * Blocksize, 9 * Blocksize, 3 * Blocksize, 1 * Blocksize, 30 * Blocksize, 9 * Blocksize-1, 33 * Blocksize - 1, 15 * Blocksize, down, 8, 1, 2); block_num++;
		block[7] = new MoveBlock(38 * Blocksize, 12 * Blocksize, 1 * Blocksize, 2 * Blocksize, 38 * Blocksize-1, 14 * Blocksize - 1, 38 * Blocksize - 1, 14 * Blocksize-1, left,8,0,3); block_num++;
		block[8] = new MoveBlock(35 * Blocksize, 14 * Blocksize, 1 * Blocksize, 1 * Blocksize, 38 * Blocksize - 1, 14 * Blocksize - 1, 38 * Blocksize - 1, 14 * Blocksize - 1, down); block_num++;
		block[9] = new MoveBlock(39 * Blocksize, 12 * Blocksize, 4 * Blocksize, 2 * Blocksize, 38 * Blocksize - 1, 14 * Blocksize - 1, 38 * Blocksize - 1, 14 * Blocksize - 1, down); block_num++;
		block[10] = new MoveBlock(42 * Blocksize, 10 * Blocksize, 1 * Blocksize, 2 * Blocksize, 40 * Blocksize - 1, 14 * Blocksize - 1, 40 * Blocksize - 1, 14 * Blocksize - 1, down, 8, 0, 3); block_num++;
		block[11] = new MoveBlock(38 * Blocksize, 10 * Blocksize, 1 * Blocksize, 2 * Blocksize, 40 * Blocksize - 1, 14 * Blocksize - 1, 40 * Blocksize - 1, 14 * Blocksize - 1, down, 8, 0, 3); block_num++;
		block[12] = new MoveBlock(39 * Blocksize, 14 * Blocksize, 1 * Blocksize, 1 * Blocksize, 40 * Blocksize - 1, 14 * Blocksize - 1, 40 * Blocksize - 1, 14 * Blocksize - 1, down); block_num++;
		block[13] = new InvisibleBlock(37 * Blocksize, 10 * Blocksize); block_num++;
		block[14] = new Needle(43 * Blocksize, 15 * Blocksize, 4 * Blocksize, Blocksize, 43 * Blocksize, 0, 47 * Blocksize, 15 * Blocksize, up, 20); block_num++;
		block[15] = new Needle(49 * Blocksize, 1 * Blocksize, 1 * Blocksize, 10 * Blocksize, 49 * Blocksize - 1, Blocksize, 49 * Blocksize - 1, 11 * Blocksize - 1, left,1); block_num++;
		block[16] = new Needle(57 * Blocksize, 11 * Blocksize, 1 * Blocksize, 3 * Blocksize, 54 * Blocksize, Blocksize, 57 * Blocksize - 1, 14 * Blocksize - 1, left,10,1); block_num++;
		block[17] = new MoveBlock(56 * Blocksize, 14 * Blocksize, 2 * Blocksize, Blocksize, 56 * Blocksize, 14 * Blocksize - 1, 58 * Blocksize - 1, 14 * Blocksize - 1, down); block_num++;
		block[18] = new MoveBlock(62 * Blocksize, 13 * Blocksize, 1 * Blocksize, Blocksize, 61 * Blocksize, 14 * Blocksize - 1, 62 * Blocksize - 1, 14 * Blocksize - 1, left, 16, 0, 3); block_num++;
		block[19] = new Needle(67 * Blocksize, 15 * Blocksize, 3 * Blocksize, Blocksize, 67 * Blocksize, 0, 68 * Blocksize - 1, 8 * Blocksize - 1, up, 16); block_num++;
		block[20] = new MoveBlock(72 * Blocksize, 4 * Blocksize, 3 * Blocksize, 1 * Blocksize, 50 * Blocksize, 0, 74 * Blocksize - 1, 15 * Blocksize, down); block_num++;
		block[21] = new ItemBlock(77 * Blocksize, 8 * Blocksize, 1); block_num++;
		block[22] = new MoveBlock(83 * Blocksize, 4 * Blocksize, 3 * Blocksize, 11 * Blocksize, 83 * Blocksize, 4 * Blocksize - 1, 86 * Blocksize - 1, 4 * Blocksize - 1, down); block_num++;
		block[23] = new InvisibleBlock(78 * Blocksize, 4 * Blocksize); block_num++;
		block[24] = new MoveBlock(92 * Blocksize, 6 * Blocksize, Blocksize, 2 * Blocksize, 93 * Blocksize, 8 * Blocksize - 1, 94 * Blocksize - 1, 8 * Blocksize - 1, right, 4, 0, 3); block_num++;
		block[25] = new InvisibleBlock(96 * Blocksize, 3 * Blocksize); block_num++;
		block[26] = new MoveBlock(98 * Blocksize, 6 * Blocksize, Blocksize, 3 * Blocksize, 96 * Blocksize, 3 * Blocksize, 97 * Blocksize - 1, 4 * Blocksize, left, 4, 0, 3); block_num++;
		block[27] = new InvisibleBlock(100 * Blocksize, 0); block_num++;
		block[28] = new ItemBlock(110* Blocksize, 8 * Blocksize, 2); block_num++;
		block[29] = new Needle(105 * Blocksize, 11 * Blocksize, Blocksize, 3 * Blocksize, 105 * Blocksize, 14 * Blocksize - 1, 115 * Blocksize, 14 * Blocksize - 1, right, 10, 2); block_num++;
		block[30] = new MoveBlock(106 * Blocksize, 14 * Blocksize, Blocksize, Blocksize, 110 * Blocksize, 9 * Blocksize, 111 * Blocksize - 1, 9 * Blocksize, down); block_num++;
		block[31] = new MoveBlock(115 * Blocksize, 15 * Blocksize, Blocksize, 15 * Blocksize, 117 * Blocksize, 14 * Blocksize - 1, 118 * Blocksize - 1, 14 * Blocksize - 1, up, 8, 0, 4); block_num++;
		block[32] = new InvisibleBlock(116 * Blocksize, 11 * Blocksize); block_num++;
		block[33] = new InvisibleBlock(117 * Blocksize, 11 * Blocksize); block_num++;
		block[34] = new Needle(116 * Blocksize, -2 * Blocksize, 2 * Blocksize, Blocksize, 117 * Blocksize, 14 * Blocksize - 1, 118 * Blocksize - 1, 14 * Blocksize - 1, down, 2); block_num++;
		block[35] = new Needle(121 * Blocksize, -5 * Blocksize, 9 * Blocksize, Blocksize, 125 * Blocksize, 8 * Blocksize - 1, 126 * Blocksize - 1, 8 * Blocksize - 1, down, 20); block_num++;
		block[36] = new MoveBlock(128 * Blocksize, 6 * Blocksize, Blocksize, 9 * Blocksize, 128 * Blocksize, 6 * Blocksize - 1, 129 * Blocksize - 1, 6 * Blocksize - 1, right, 1, 0, 5); block_num++;
		block[37] = new Needle(150 * Blocksize, 3 * Blocksize, Blocksize, 3 * Blocksize, 134 * Blocksize, 0, 135 * Blocksize, 6 * Blocksize, left, 20); block_num++;
	}
	if (snumber == 3) {
		block_num = 0;
		block[0] = new MoveBlock(1 * Blocksize, 4 * Blocksize, 3 * Blocksize, 1 * Blocksize, 1 * Blocksize, 0, 4 * Blocksize - 1, 15 * Blocksize, down); block_num++;
	}

	//中間地点、ゴール、偽ゴールの座標を設定
	if (snumber == 2) {
		fake_goal_x = 71 * Blocksize;
		fake_goal_y = 2 * Blocksize;
		checkpoint_x = 73 * Blocksize;
		goal_x = 150 * Blocksize;
		goal_y = 3 * Blocksize;
	}

}

Map::~Map() {
	for (int i = 0; i < block_num; i++) {
		delete block[i];
	}
}

//マップの計算　毎フレーム呼び出す、動くブロックの移動判定もここでする
void Map::Update(Player *player) {
	//for文で全てのブロックに対して移動の処理をする
	for (int i = 0; i < block_num; i++) {
		block[i]->Update(player);
	}
}

//カメラの位置の更新
void Map::set_camera(int c_x, int c_y) {
	camera_x = c_x;
	camera_y = c_y;
}

//マップの描画処理
void Map::Draw(int cnt) {
	//地面の描画
	for (int i = 0; i < mapheight; i++) {
		for (int j = 0; j < mapwidth; j++) {
			if (map_ground[i][j] == 'g')
				DrawGraph(j * Blocksize - camera_x, i * Blocksize - camera_y, image_ground, TRUE);
			else if (map_ground[i][j] == 'c')
				DrawGraph(j * Blocksize - camera_x, i * Blocksize - camera_y, image_coin, TRUE);
			else if (map_ground[i][j] == 'k')
				DrawGraph(j * Blocksize - camera_x, i * Blocksize - camera_y, image_key[(cnt / 10) % 2], TRUE);
		}
	}
	//特殊なブロックの描画
	for (int i = 0; i < block_num; i++) {
		block[i]->Draw(camera_x, camera_y);
	}

	//偽ゴールの描画
	DrawFormatString(fake_goal_x - camera_x, fake_goal_y - camera_y - 16, white, "G O A 1");
	DrawGraph(fake_goal_x - camera_x, fake_goal_y - camera_y, image_goal[(cnt / 10) % 2], TRUE);

	//ゴールの描画
	DrawFormatString(goal_x - camera_x, goal_y - camera_y - 16, white, "G O A L");
	DrawGraph(goal_x - camera_x, goal_y - camera_y, image_goal[(cnt / 10) % 2], TRUE);
}

//通れないブロック内なら0、進める場所なら1、下以外からなら進める場所なら2、死亡ブロックなら3以上の数字を返す
int Map::MoveAbleXY(int ma_x, int ma_y) {
	if (map_ground[ma_y / Blocksize][ma_x / Blocksize] == 'g')
		return 0;//指定の座標が地面の内部なら0を返す

	//特殊ブロックについての処理
	for (int i = 0; i < block_num; i++) {
		if (block[i]->x <= ma_x && ma_x <= block[i]->x + block[i]->x_size - 1 && block[i]->y <= ma_y && ma_y <= block[i]->y + block[i]->y_size - 1) {
			if ((block[i]->m_kind == invisibleblock) && block[i]->image_handle == 0)//まだ叩いてない透明ブロックは2
				return 2;
			//else if (block[i]->m_kind == trapblock && block[i]->damageflag == 1)//叩いたトラップブロックは3
				//return 3;
			else
				return 0;
		}
	}

	return 1;//上のifに引っかからない時は1
}
