#pragma once

#include "Block.h"

class Player;

class TrapBlock : public Block {
public:
	int image_handle2;
public:
	TrapBlock(int, int);//コンストラクタ
	void Update(Player);
	void Knock(char mapdata[mapheight][mapwidth], Player* player);
	void Draw(int,int);//オーバーライド
};