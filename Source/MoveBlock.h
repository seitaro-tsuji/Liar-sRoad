#pragma once
#include "Block.h"

class MoveBlock : public Block {
public:
	int cnt;	//動き始めてからのフレーム数　特殊な挙動のブロック用
	int imagepattern;//見た目のパターン
	int image_handle2;	//見た目のパターンが特殊な時に使う　2ブロック以上の時
	int movepattern;	//動きのパターン
public:
	MoveBlock(int bx, int by, int bx_size, int by_size, int tx1, int ty1, int tx2, int ty2, direction dire, int speed = 8, int imagenumber=0, int movenumber=0);//コンストラクタ
	void Update(Player* player);//オーバーライド
	void Draw(int, int);//オーバーライド　見た目を変えるため
};