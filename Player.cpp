#include "Player.h"
#include "Input.h"
#include "DebugText.h"
#include "SphereCollider.h"
#include "ParticleManager.h"

using namespace DirectX;

Player* Player::Create(Model* model)
{
	//3Dオブジェクトのインスタンスを生成
	Player* instance = new Player();
	if (instance == nullptr)
	{
		return nullptr;
	}
	//初期化
	if (!instance->Initialize())
	{
		delete instance;
		assert(0);
	}
	//モデルのセット
	if (model)
	{
		instance->SetModel(model);
	}

	return instance;
}

bool Player::Initialize()
{
	if (!Object3d::Initialize())
	{
		return false;
	}
	//コライダーの追加
	float radius = 0.6f;
	//半径分だけ足元から浮いた座標を球の中心にする
	SetCollider(new SphereCollider(XMVECTOR({ 0,radius,0,0 }), radius));

	return true;
}

void Player::Update()
{
	//Input* input = Input::GetInstance();

	//A,Dで旋回
	if (input->PushKey(DIK_A))
	{
		rotation.y -= 2.0f;
	}
	else if (input->PushKey(DIK_D))
	{
		rotation.y += 2.0f;
	}
	//移動ベクトルをY軸周りの角度で回転
	XMVECTOR move = { 0.0f,0.0f,0.1f,0.0f };
	DirectX::XMMATRIX matRot =
		XMMatrixRotationY(XMConvertToRadians(rotation.y));
	move = XMVector3TransformNormal(move, matRot);

	//向いてる方向に移動
	if (input->PushKey(DIK_S))
	{
		position.x -= move.m128_f32[0];
		position.y -= move.m128_f32[1];
		position.z -= move.m128_f32[2];
	}
	else if (input->PushKey(DIK_W))
	{
		position.x += move.m128_f32[0];
		position.y += move.m128_f32[1];
		position.z += move.m128_f32[2];
	}

	//行列の更新など
	Object3d::Update();
}

void Player::OnCollision(const CollisionInfo& info)
{
	//デバッグテキスト表示
	//DebugText::GetInstance()->Print("Collision detected.",0.0f,0.0f,1.0f);

	//衝突時にパーティクルを発生させる
	for (int i = 0; i < 1; ++i)
	{
		const float rnd_vel = 0.1f;
		DirectX::XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		ParticleManager::GetInstance()->Add(10,
			DirectX::XMFLOAT3(info.inter.m128_f32), vel, DirectX::XMFLOAT3(), 0.0f, 1.0f);
	}
}