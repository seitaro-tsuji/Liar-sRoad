#pragma once
#include "Block.h"

class Player;

class InvisibleBlock :public Block {
public:
	int item;	//中身のアイテム
	int knock_flag;	//ブロックをたたいたかどうか 0ならまだ
	int cnt;//叩いてからのcnt
public:
	InvisibleBlock(int, int, int itemkind = 0);	//コンストラクタ
	void Update(Player* player);
	void Knock(char mapdata[mapheight][mapwidth], Player*);	//ブロックを叩く処理(注意：叩いたかの判定がこのクラスのみ異なる)
};