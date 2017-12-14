#pragma once

#include<Eigen\Core>

using namespace Eigen;

enum CharacterState {
	Normal,
	Damege,
	Dead,
};

//キャラクターの持つステータス、外部からのアクセスは許可する予定
struct CharacterParameter {
	CharacterState state;//状態
	int HP;//ヒットポイント
	int EPLevel;//電力レベル
	int EP;//現在使用可能な電力
};
