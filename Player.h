#pragma once
#include "Object3d.h"
#include "Input.h"
class Player :public Object3d
{
public:
	//3Dオブジェクト生成
	static Player* Create(Model* model = nullptr);

public:
	//初期化
	bool Initialize()override;
	//毎フレーム処理
	void Update()override;
	//衝突時コールバック関数
	void OnCollision(const CollisionInfo& info)override;

	Input* input = nullptr;

	void SetInput(Input* input) { this->input = input; }

};