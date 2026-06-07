#ifndef STAGE2_H
#define STAGE2_H

#include "CommonVari.h"

class Player;
class Map;

class Stage {
public:
	int cnt;	//cnt変数
	int stage_number;
	Player* player;
	Map* map;
	int check_flag;	//中間地点に到達したかどうか
public: 
	Stage(int snumber, int startcheck);//コンストラクタ startcheckはスタートから開始か中間地点から開始か
	~Stage();//デストラクタ　メンバ変数のクラスのデストラクタを呼び出す
	void Check_Check();//中間地点に到達したかを確認する関数
	int DeathCheck();//死亡チェック
	int DeathDraw(int);//死亡時の演出の描画 1が返されるとタイトルに戻る
	int ClearCheck();//クリアチェック
	int ClearDraw();	//クリア時の描画
	int P_IsAbleToMoveXY(int,int);//指定されたxy座標にプレイヤーが移動できるか確認する関数
	int PlayerMoveWhileAble(int z, direction d);//プレイヤーが指定された方向に移動できるだけ移動する関数
	void PlayerControl();//プレイヤーの操作をする関数
	void Camera_Update();	//カメラ位置の更新
	void PlayerDraw();	//プレイヤーを描画する
	int Play();//プレイ関数　この中で全ての処理を行う
};

//ステージ2以降の開始処理 startpointはスタート地点からのスタートか中間からのスタートか
void Start_Stagefrom2(int snumber, int startpoint=0);

//ステージ2以降のプレイ処理
void Play_Stagefrom2(int*, int*);

#endif

