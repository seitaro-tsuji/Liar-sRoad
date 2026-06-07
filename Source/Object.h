#ifndef OBJECT_H
#define OBJECT_H

#include "DxLib.h"
#include "CommonVari.h"

class Object {
public:
	int x;//x座標
	int y;//y座標
	int x_size;//x方向の大きさ
	int y_size;//y方向の大きさ
	int image_handle;	//画像ハンドル
public:
	void Move(int, int);//移動メソッド
	void MoveDirection(int, direction);//指定した方向に移動する関数
	void Set(int, int);	//セットメソッド
	virtual void Draw();	//描画メソッド
};

#endif
