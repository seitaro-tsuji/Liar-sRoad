#ifndef PLAYER_H
#define PLAYER_H

#include "Object.h"

class Map;

class Player : public Object {
public:
	int image[6];	//プレイヤーの画像ハンドル
	int state;	//プレイヤーの状態　ジャンプ中かどうか、向いている方向など
	int jump_phase;	//ジャンプの状態
	int jump;	//この数字が1以上なら消費しながら上昇する
	int jump_cnt;	//ジャンプ中のcnt
	int deathcause;//死因　deathcheckの外で死亡したとき用
	int coin;	//コインの枚数
	direction dire;	//向き
	int dashing;//移動中かどうか
public:
	Player(int start_x, int start_y);//コンストラクタ 
	void Jump();//ジャンプ関数(jump変数の操作)
	void Draw();	//描画関数　ObjectのDraw関数のオーバーライド
	void GetCoin(Map* map);	//コイン回収
};

#endif
