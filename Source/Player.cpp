#include "DxLib.h"
#include "Player.h"
#include "CommonVari.h"
#include "Keyboard.h"
#include "Map2.h"

#define PlayerJumpPower	32	//プレイヤーの最低ジャンプ

//コンストラクタ　ステージの開始地点にプレイヤーを配置する
Player::Player(int start_x, int start_y) {
	x = start_x;
	y = start_y;
	x_size = Blocksize * 1;
	y_size = Blocksize * 2;
	jump_phase = 0;
	jump = 0;
	image[0] = LoadGraph("画像/棒人間/棒人間＿右向き静止.png");
	image[1] = LoadGraph("画像/棒人間/棒人間＿右向き移動.png");
	image[2] = LoadGraph("画像/棒人間/棒人間＿右向きジャンプ.png");
	image[3] = LoadGraph("画像/棒人間/棒人間＿左向き静止.png");
	image[4] = LoadGraph("画像/棒人間/棒人間＿左向き移動.png");
	image[5] = LoadGraph("画像/棒人間/棒人間＿左向きジャンプ.png");
	coin = 0;
	dire = right;
	deathcause = 0;

	//デバッグ用の初期値
	//x = 75 * Blocksize, y = 12 * Blocksize;
}

//ジャンプ関数(jump変数の操作)
void Player::Jump() {
	if (jump_phase == 0) {
		if (Action_Keyboard_Get(a_jump) == 1) {//ジャンプ
			jump_phase = 1;
			jump = PlayerJumpPower;
			jump_cnt = 0;
		}
	}
	else if (jump_phase == 1) {
		jump_cnt++;
		jump = PlayerJumpPower;	//フェーズ1の間はjump変数は固定
		if (Action_Keyboard_Get(a_jump) == 0 || jump_cnt >= 30) {//Z,↑キー離すか30フレーム経過でフェーズ２に
			jump_phase = 2;
		}
	}
	else if (jump_phase == 2) {
		if (jump <= 0) {//jump変数が0になったら上昇終了
			jump_phase = 3;
			jump = 0;
		}
	}
}

void Player::GetCoin(Map* map) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
			if (map->map_ground[y / Blocksize + i][x / Blocksize + j] == 'c') {
				coin++;
				map->map_ground[y / Blocksize + i][x / Blocksize + j] = '.';
			}
			else if(map->map_ground[y / Blocksize + i][x / Blocksize + j] == 'k')
				map->map_ground[y / Blocksize + i][x / Blocksize + j] = '.';
		}
	}
}

//描画関数
void Player::Draw() {
	//DrawGraph(x, y, image[0], TRUE);
}