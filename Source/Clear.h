#pragma once
#include "CommonVari.h"

//初期化関数
void ClearInitialize();

//クリア判定の関数 クリアすれば1を返す
int ClearJudge(player_t player, map_t map);

//クリア画面の処理関数
void ClearUpdate(player_t* player,int *scene);

//クリア画面の描画関数
void ClearDraw();