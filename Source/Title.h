#pragma once
#include "CommonVari.h"

//タイトル画面用の初期化関数
void TitleInitialize();

//タイトル画面の計算処理関数
void TitleUpdate(player_t *player,int *scene);

//タイトル画面の描画処理関数
void TitleDraw(player_t player);
