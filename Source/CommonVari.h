#pragma once

#include "DxLib.h"

//scene用の数字
#define title	99999
#define stage	99998
#define stagefrom2	99996
#define clear	99997

#define AllStageNum 2	//ステージの総数

#define Blocksize	32	//ブロックの大きさ(ピクセル)

#define mapheight   15	//mapの高さ
#define mapwidth	150	//mapの横幅

typedef enum { a_jump, a_moveleft, a_moveright }action;	//複数のキーを一つのアクションの対応させる関数用
typedef enum { down, left, right, up }direction;	//方向の列挙体
typedef enum { fall, waitfall, waitwaitfall, stopfall, needle, waitneedle, stopneedle }blockkinds;			//動くブロックの種類の列挙体
typedef enum { itemblock, trapblock, invisibleblock, needleblock }b_kinds;//特殊なブロックの種類　ステージ2以降で使用する

//プレイヤー用の構造体
typedef struct {
	int x;
	int y;
	int dire;	//向いている方向　0が左
	int jamp_phase;	//ジャンプフェーズ
	int jamp;
	int sta;	//ステージナンバー
	int degree;	//進行度
	int checkflag[AllStageNum + 1];	//中間地点に到達したかどうかのフラグ 添え字は1,2,...allstagenum
	int coin,keyitem;	//コイン、キーアイテム(見た目だけで意味はないアイテム)
	int ereachenge_flag;	//エリアチェンジのフラグ
}player_t;

//mapのブロック用の構造体
typedef struct {
	int x, y;	//x,y座標(左上)
	int x_size, y_size;	//x,y方向の大きさ
	blockkinds kind;	//ブロックの種類
	direction dire;		//方向
}block_t;

//map用の構造体
typedef struct {
	char map_d[mapheight][mapwidth];	//mapのデータ
	int move_block_num;		//動くブロックの総数
	block_t moveblock[20];	//動くブロックのデータ
	int goal_x, goal_y;	//ゴール座標(サイズは2×2)
	int checkpoint_x;	//中間地点のx座標
}map_t;

static int scene;	//ゲームの場面を表す

static int white = GetColor(255, 255, 255);
static int l_blue = GetColor(102, 178, 255);
static int green = GetColor(0, 255, 0);
static int yellow = GetColor(255, 255, 0);
static int red = GetColor(255, 0, 0);
static int gray = GetColor(128, 128, 128);
static int yellow2 = GetColor(153, 255, 255);//黄色だったが水色になった　死亡時の画面で使う