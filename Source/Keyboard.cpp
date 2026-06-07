#include "AllHeader.h"

// キーの入力状態更新
void Keyboard_Update() {
    char tmpKey[256];             // 現在のキーの入力状態を格納する
    GetHitKeyStateAll(tmpKey);  // 全てのキーの入力状態を得る
    for (int i = 0; i < 256; i++) {
        if (tmpKey[i] != 0) { // i番のキーコードに対応するキーが押されていたら
            m_Key[i]++;   // 加算
        }
        else {              // 押されていなければ
            m_Key[i] = 0; // 0にする
        }
    }
}

// KeyCodeのキーの入力状態を取得する
int Keyboard_Get(int KeyCode) {
    return m_Key[KeyCode]; // KeyCodeの入力状態を返す
}

//ジャンプや左右移動のキーを受け取る
int Action_Keyboard_Get(action act) {
    if (act == a_jump) {
        if (Keyboard_Get(KEY_INPUT_W))  return Keyboard_Get(KEY_INPUT_W);
        if (Keyboard_Get(KEY_INPUT_Z))  return Keyboard_Get(KEY_INPUT_Z);
        if (Keyboard_Get(KEY_INPUT_UP))  return Keyboard_Get(KEY_INPUT_UP);
        return 0;
    }
    else if (act == a_moveleft) {
        if (Keyboard_Get(KEY_INPUT_A))  return Keyboard_Get(KEY_INPUT_A);
        if (Keyboard_Get(KEY_INPUT_LEFT))  return Keyboard_Get(KEY_INPUT_LEFT);
        return 0;
    }
    else if (act == a_moveright) {
        if (Keyboard_Get(KEY_INPUT_D))  return Keyboard_Get(KEY_INPUT_D);
        if (Keyboard_Get(KEY_INPUT_RIGHT))  return Keyboard_Get(KEY_INPUT_RIGHT);
        return 0;
    }
    return 0;
}