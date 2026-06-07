#pragma once
#include "CommonVari.h"

//初期化関数
void EreaChengeInitialize();

//エリアチェンジの判定を行う関数　チェンジナンバー(do関数で使用する)を返す
int EreaChengeJudge(player_t player, map_t map);

//エリアチェンジの処理をする関数
int EreaChengeDo(player_t* player, map_t map, int chenge_number);

//エリアチェンジの描画をする関数
void EreaChengeDraw();