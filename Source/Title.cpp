#include "AllHeader.h"

static int titledrawfont;	//ゲームタイトルを表示する文字のフォント
static int titlefont;		//タイトル画面の文字のフォント

static int t_cnt;	//cnt変数
static int cursor;	//カーソル変数
static int cursor2;	//カーソル　開始地点選択用
static int page;	//ページ変数(ステージの2面以降を追加した時用)
static int select_flag;		//スタート地点のセレクト画面

//タイトル画面用の初期化関数
void TitleInitialize() {
	titledrawfont = CreateFontToHandle("ＭＳ 明朝", 64, 9, DX_FONTTYPE_ANTIALIASING);
	titlefont = CreateFontToHandle("HGS創英角ﾎﾟｯﾌﾟ体", 40, 3, DX_FONTTYPE_ANTIALIASING_EDGE);

	cursor = 0;
}

//タイトル画面の計算処理関数
void TitleUpdate(player_t *player, int *scene) {
	t_cnt++;

	//セレクト画面でない時
	if (select_flag == 0) {
		//カーソル移動の処理をする
		int cursornum = player->degree;	//カーソルの移動範囲
		if (Keyboard_Get(KEY_INPUT_DOWN) == 1 || Keyboard_Get(KEY_INPUT_S) == 1)
			cursor = (cursor + 1) % cursornum;
		if (Keyboard_Get(KEY_INPUT_UP) == 1 || Keyboard_Get(KEY_INPUT_W) == 1)
			cursor = (cursor + cursornum - 1) % cursornum;

		//決定キーを押したときの処理をする(Zが2なのはステージに入った瞬間にジャンプ入力しないようにするため) 中間地点に到達していない時
		if ((Keyboard_Get(KEY_INPUT_Z) == 2 || Keyboard_Get(KEY_INPUT_F) == 1 || Keyboard_Get(KEY_INPUT_RETURN) == 1) && t_cnt >= 2 && player->checkflag[cursor + 1] == 0) {
			t_cnt = 0;
			if (cursor == 0) {
				*scene = stage;	//ステージに入る
				player->sta = cursor + 1;//ステージナンバーの決定
				StageStart(player, 0);	//ステージにはいる処理
			}
			else {
				t_cnt = 0;
				*scene = stagefrom2;
				player->sta = cursor + 1;
				Start_Stagefrom2(cursor + 1);//ステージ2以降のスタート処理
			}
		}

		//決定キーを押したときの処理をする(Zが2なのはステージに入った瞬間にジャンプ入力しないようにするため) 中間地点に到達している時
		if ((Keyboard_Get(KEY_INPUT_Z) == 2 || Keyboard_Get(KEY_INPUT_F) == 1 || Keyboard_Get(KEY_INPUT_RETURN) == 1) && t_cnt >= 2 && player->checkflag[cursor + 1] == 1) {
			t_cnt = 0;
			select_flag = 1;//スタート、中間セレクト画面に入る
		}
	}

	//スタート地点のセレクト画面での処理
	if (select_flag == 1) {
		if ((Keyboard_Get(KEY_INPUT_Z) == 2 || Keyboard_Get(KEY_INPUT_F) == 1 || Keyboard_Get(KEY_INPUT_RETURN) == 1) && t_cnt >= 2) {
			if (cursor == 0) {//ステージ1の処理
				t_cnt = 0;
				*scene = stage;	//ステージに入る
				player->sta = cursor + 1;//ステージナンバーの決定
				StageStart(player, cursor2);	//ステージにはいる処理(カーソル2は1なら中間地点からスタート)
				select_flag = 0;
				cursor2 = 0;
			}
			else {//ステージ2以降
				t_cnt = 0;
				*scene = stagefrom2;
				player->sta = cursor + 1;
				Start_Stagefrom2(cursor + 1, cursor2);
				select_flag = 0;
				cursor2 = 0;
			}
		}
		if (Keyboard_Get(KEY_INPUT_DOWN) == 1 || Keyboard_Get(KEY_INPUT_S) == 1 || Keyboard_Get(KEY_INPUT_UP) == 1 || Keyboard_Get(KEY_INPUT_W) == 1)
			cursor2 = (cursor2 + 1) % 2;
	}
}

//タイトル画面の描画処理関数
void TitleDraw(player_t player) {
	DrawStringToHandle(60, 30, "ライアーズロード", l_blue, titledrawfont, white);

	for (int i = 0; i < player.degree; i++) {
		DrawFormatStringToHandle(380, 130 + 60 * i, white, titlefont, "ステージ%d", i + 1);
	}
	for (int i = player.degree; i < AllStageNum; i++) {
		DrawFormatStringToHandle(380, 130 + 60 * i, gray, titlefont, "ステージ%d", i + 1);
	}
	DrawStringToHandle(320, 130 + cursor * 60, "→", yellow, titlefont);

	if (select_flag == 1) {//開始地点選択
		DrawBox(50, 100, 300, 200, white, FALSE);//選択ウィンドウの表示
		DrawFormatString(70, 120, white, "スタート地点から");
		DrawFormatString(70, 150, white, "中間地点から");
		DrawFormatString(54, 120 + cursor2 * 30, white, "→");
	}
}