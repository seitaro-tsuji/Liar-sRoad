#pragma once
#include "CommonVari.h"

//プレイヤー関連の初期化関数
void PlayerInitialaize();

//ジャンプの処理をする関数
void PlayerJamp(player_t* player, map_t *map);

//プレイヤーの移動などをする関数
void PlayerUpdate(player_t* player, map_t *map, int camera_x);

//プレイヤーの画像を描画する関数
void PlayerDraw(player_t player, int camera_x, int camera_y);