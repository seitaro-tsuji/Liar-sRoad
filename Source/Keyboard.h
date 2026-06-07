#ifndef DEF_KEYBOARD_H //二重include防止
#define DEF_KEYBOARD_H


static int m_Key[256];  // キーの入力状態格納用変数

// キーの入力状態を更新する
void Keyboard_Update();

// 引数のキーコードのキーの入力状態を返す
int Keyboard_Get(int KeyCode);

//ジャンプや左右移動のキーを受け取る:
int Action_Keyboard_Get(action act);

#endif