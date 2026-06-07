#pragma once
#include "Block.h"

class Player;

class Needle :public Block {
public:
	int movepattern;
	int cnt;	//動き始めてからのフレーム数　特殊な挙動で使用する
public:
	Needle(int,int,int,int,int,int,int,int,direction,int sp=10,int movenumber=0);//movepatternは挙動の種類
	void Update(Player*);//オーバーライド
};