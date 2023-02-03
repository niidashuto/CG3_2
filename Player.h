#pragma once
#include "Object3d.h"
#include "Input.h"
class Player :public Object3d
{
public:
	//3D�I�u�W�F�N�g����
	static Player* Create(Model* model = nullptr);

public:
	//������
	bool Initialize()override;
	//���t���[������
	void Update()override;
	//�Փˎ��R�[���o�b�N�֐�
	void OnCollision(const CollisionInfo& info)override;

	Input* input = nullptr;

	void SetInput(Input* input) { this->input = input; }

};