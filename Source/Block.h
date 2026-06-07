#ifndef BLOCK_H
#define BLOCK_H

#include "Object.h"
#include "CommonVari.h"

class Player;

class Block : public Object {
public:
	b_kinds m_kind;//ブロックの種類
	int moveflag;	//移動フラグ　0から1になると動き出す　-1なら動かない
	int damageflag;	//ダメージフラグ　1なら触れると死亡する
	int isabletoknock;	//叩くことができるブロックかどうか　1なら可能
	int knock_flag;//すでに叩いたかどうか　0ならまだ
	int flag;	//叩いたかどうかのフラグ　なぜかknock_flagだと透明ブロックのプログラムが上手くいかない
	int trigger_x1, trigger_y1, trigger_x2, trigger_y2;//この長方形の範囲内に入るとmoveflagが1になる
	direction movedire;	//動く方向
	int speed;	//動くスピード
public:
	int IsInsideBlockXY(int, int);//指定した座標がブロックの内部かどうか判定する関数
	int P_isInsideBlock(Player);//プレイヤーがブロックに埋まっていたら1を返す
	virtual void Update(Player *player);//ブロック更新の仮想関数 ここでプレイヤーの死亡判定するかも
	int KnockJudge(Player player);//ブロックをたたく位置にプレイヤーがいるかどうかの判定　サブクラスで使用
	virtual void Knock(char mapdata[mapheight][mapwidth], Player *player);//叩く処理の仮想関数
	virtual void Draw(int, int);//描画関数
};

#endif
