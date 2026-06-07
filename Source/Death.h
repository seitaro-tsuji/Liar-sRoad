#pragma once

//deathファイルの初期化関数
void DeathInitialize();

//プレイヤーの死亡判定をする関数 死亡したら1を返す
int DeathCheck(player_t player, map_t map);

//死亡時の演出の描画をする関数
void DeathDraw(player_t player, int deathflag);