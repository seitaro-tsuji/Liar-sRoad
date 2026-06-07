#pragma once

//ステージ関連の初期化関数
void StageInitialize();

//ステージ開始時の関数
void StageStart(player_t *player, int Checkflag);

//ステージ関連の動作処理関数
void StageUpdate(player_t *player, int *scene);

//ステージ関連の描画処理関数
void StageDraw(player_t player);