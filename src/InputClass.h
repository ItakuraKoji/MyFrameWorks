#pragma once
#pragma comment(lib, "winmm.lib")
#include<windows.h>
#include<math.h>
#include<iostream>
#include<Eigen\Core>
using namespace Eigen;

//スティック初期位置の遊び定数
#define BUTTON_EPSILON 0.2f

//ゲームパッドのアナログスティックの値の原点
#define BUTTON_ANALOG_NEUTRAL 32768.f//家のXBoxコントローラー

//接続したゲームパッドのボタンと軸
enum {
	PAD_BUTTON_0 = 1,
	PAD_BUTTON_1 = 2,
	PAD_BUTTON_2 = 4,
	PAD_BUTTON_3 = 8,
	PAD_BUTTON_4 = 16,
	PAD_BUTTON_5 = 32,
	PAD_BUTTON_6 = 64,
	PAD_BUTTON_7 = 128,
	PAD_BUTTON_8 = 256,
	PAD_BUTTON_9 = 512,
	PAD_BUTTON_10 = 1024,
	PAD_BUTTON_11 = 2048,
	PAD_BUTTON_12 = 4096,
	PAD_STICK_X = 0,
	PAD_STICK_Y = 1,
	PAD_STICK_Z = 2,
	PAD_STICK_R = 3,
	PAD_STICK_U = 4,
	PAD_STICK_V = 5,
};

//仮想ゲームパッド配列用定数
//ABXYとLR
//十字キー
//6軸のアナログ情報（0〜1のfloat）
//上記のアナログ情報二つを、横方向と縦方向として持つスティック２本（右と左を想定）
enum {
	VPAD_BUTTON_A,
	VPAD_BUTTON_B,
	VPAD_BUTTON_X,
	VPAD_BUTTON_Y,
	VPAD_BUTTON_R,
	VPAD_BUTTON_L,
	VPAD_BUTTON_RIGHT,
	VPAD_BUTTON_LEFT,
	VPAD_BUTTON_UP,
	VPAD_BUTTON_DOWN,

	VPAD_STICK_LX = 0,
	VPAD_STICK_LY,
	VPAD_STICK_RX,
	VPAD_STICK_RY,
	VPAD_STICK_Z1,
	VPAD_STICK_Z2,

	VPAD_STICK_L = 0,
	VPAD_STICK_R,
};

//ジョイパッドの状態
struct JoyButtonState {
	int button;			//仮想パッドのこのボタンに対応するゲームパッドのボタンのビット
	int keyboard;		//仮想パッドのこのボタンにに対応するキー
	int press;			//今、このボタンが押されているか
	int prevPress;		//前フレーム、このボタンが押されていたか
};

//アナログスティックの軸
struct JoyStickAxis {
	int axis;			//仮想パッドのこの軸に対応するゲームパッドの軸
	int plusButton;		//この軸における生の方向に対応するキーボードのキー
	int minusButton;	//この軸における負の方向に対応するキーボードのキー
	float pos;			//アナログスティックの位置
};

//横と縦の二軸を持つスティック
struct JoyStickState {
public:
	//軸をセット
	void SetAxisX(JoyStickAxis* xAxis) {
		this->x = xAxis;
	}
	void SetAxisY(JoyStickAxis* yAxis) {
		this->y = yAxis;
	}

	//位置
	Vector2f GetPosition() {
		return Vector2f(x->pos, y->pos);
	}
	//角度
	float GetRotation() {
		return atan2(y->pos, x->pos);
	}
	//傾き
	float GetPower() {
		return sqrtf(powf(x->pos, 2) + powf(y->pos, 2));
	}
private:
	JoyStickAxis* x;
	JoyStickAxis* y;
};



class InputClass {
public:
	InputClass(UINT joyID);
	~InputClass();
	bool Initialize(UINT joyID);
	void Run();
	//押された瞬間
	bool isPressButton(int buttonID);
	//押している間
	bool isStayButton(int buttonID);
	//離された瞬間
	bool isReaveButton(int buttonID);

	//スティック軸を取得
	float GetStickState(int axisID);
	//スティック角度を取得
	float GetStickRotation(int stickID);
	//スティックの傾きを取得
	float GetStickPower(int stickID);

private:
	float FixStickState(float state);
	void GetInputState();


private:
	//コントローラーIDとJOYINFOEX構造体
	UINT joyID;
	JOYINFOEX joyInfo;

	//仮想コントローラーのボタン対応と入力情報の配列
	JoyButtonState vpadState[10] = {};
	

	//スティックの状態
	JoyStickState vpadStickState[2] = {};
	JoyStickAxis vpadStickAxis[6] = {};	//0 〜 1 の間に値が収まっている
	float stickState[6] = {};		//値が０から１に収まっていない
};