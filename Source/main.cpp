#include "AllHeader.h"

player_t player;

//ゲーム全体の初期化処理
void GameInitialize() {
	DxLib_Init();
	TitleInitialize();
	StageInitialize();
	MapInitialize();
	DeathInitialize();
	PlayerInitialaize();
	EreaChengeInitialize();
	ClearInitialize();

	scene = title;	//タイトル画面に移動
	player.degree = 1;	//進行度は1にする
	player.coin = 0;	//コインは0にする
}

//ゲーム全体の計算処理
void GameUpdate() {
	Keyboard_Update();//キーボード入力の取得

	if (scene == title)	TitleUpdate(&player, &scene);
	if (scene == stage) StageUpdate(&player, &scene);
	if (scene == clear) ClearUpdate(&player, &scene);
	if (scene == stagefrom2) Play_Stagefrom2(&scene, &player.checkflag[player.sta]);
}

//ゲーム全体の描画処理
void GameDraw() {
	if (scene == title)	TitleDraw(player);
	if (scene == stage)	StageDraw(player);
	if (scene == clear) ClearDraw();
	//デバック
	//DrawFormatString(0, 0, green, "scene=%d", scene);
}

//ゲーム全体の終了処理
void GameFinalize() {
	DxLib_End();
}

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(FALSE);//全画面モードに変更

	//初期化処理
	GameInitialize();

	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && Keyboard_Get(KEY_INPUT_ESCAPE) == 0) {
		//計算処理
		GameUpdate();
		//描画処理
		GameDraw();
	}

	//終了処理
	GameFinalize();

	return 0;				// ソフトの終了 
}