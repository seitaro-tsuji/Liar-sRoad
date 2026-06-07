#pragma once
#include "Block.h"

class ItemBlock : public Block {
public :
	int item;	//中身のアイテム
	int knock_flag;	//ブロックをたたいたかどうか 0ならまだ
public:
	ItemBlock(int,int,int);	//コンストラクタ
	void Knock(char mapdata[mapheight][mapwidth], Player*);	//ブロックを叩く処理
};