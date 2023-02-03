#include "GameScene.h"
#include <cassert>
#include "Collision.h"
#include <sstream>
#include <iomanip>
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "Player.h"

using namespace DirectX;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete spriteBG;
	delete object3d_1;
	//delete boxObject;
}

void GameScene::Initialize(DirectXCommon* dxCommon, Input* input)
{
	// nullptrチェック
	assert(dxCommon);
	assert(input);

	this->dxCommon = dxCommon;
	this->input = input;

	// デバッグテキスト用テクスチャ読み込み
	Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png");
	// デバッグテキスト初期化
	debugText.Initialize(debugTextTexNumber);

	// テクスチャ読み込み
	Sprite::LoadTexture(1, L"Resources/background.png");

	// テクスチャ2番に読み込み
	Sprite::LoadTexture(2, L"Resources/texture.png");

	//座標{0,0}に、テクスチャ2番のスプライトを生成
	sprite1 = Sprite::Create(2, { 0,0 });
	//座標{500,500}に、テクスチャ2番のスプライトを生成
	sprite2 = Sprite::Create(2, { 500,500 }, { 1,0,0,1 }, { 0,0 }, false, true);

	// 背景スプライト生成
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });

	// 3Dオブジェクト生成
	model_1 = Model::LoadFromOBJ("sphere");
	model_2 = Model::LoadFromOBJ("chr_sword");
	model_3 = Model::LoadFromOBJ("ground");

	collisionManager = CollisionManager::GetInstance();

	object3d_1 = Object3d::Create();
	object3d_2 = Object3d::Create();
	player_ = Player::Create(model_2);

	object3d_1->SetModel(model_1);
	object3d_2->SetModel(model_3);

	object3d_1->SetCollider(new SphereCollider);

	

	object3d_2->SetPosition({ 0,-5,0 });
	//object3d_2->SetScale({ 5.0f,5.0f,5.0f });

	object3d_1->SetScale({ 5,5,5 });
	player_->SetScale({ 5,5,5 });


	//球の初期値を設定
	sphere.center = XMVectorSet(0, 2, 0, 1);//中心点座標
	sphere.radius = 5.0f;//半径

	//平面の初期値を設定
	plane.normal = XMVectorSet(0, -5, 0, 0);//法線ベクトル
	plane.distance = 0.0f;//原点(0,0,0)からの距離
	
}

void GameScene::Update()
{

	//球移動
	{
		XMVECTOR moveY = XMVectorSet(0, 0.5f, 0, 0);
		if (input->PushKey(DIK_UP)) { sphere.center += moveY; }
		else if (input->PushKey(DIK_DOWN)) { sphere.center -= moveY; }

		XMVECTOR moveX = XMVectorSet(0.5f, 0, 0, 0);
		if (input->PushKey(DIK_LEFT)) { sphere.center += moveX; }
		else if (input->PushKey(DIK_RIGHT)) { sphere.center -= moveX; }

		object3d_1->SetPosition({ sphere.center.m128_f32[0],sphere.center.m128_f32[1],sphere.center.m128_f32[2] });
	}
	//stringstreamで変数の値を埋め込んで整形する
	std::ostringstream spherestr;
	spherestr << "Sphere:("
		<< std::fixed << std::setprecision(2)    //小数点以下2桁まで
		<< sphere.center.m128_f32[0] << ","        //x
		<< sphere.center.m128_f32[1] << ","        //y
		<< sphere.center.m128_f32[2] << ","        //z
		<< sphere.radius << ")";
	spherestr << "Ground:("
		<< std::fixed << std::setprecision(2)    //小数点以下2桁まで
		<< plane.normal.m128_f32[0] << ","        //x
		<< plane.normal.m128_f32[1] << ","        //y
		<< plane.normal.m128_f32[2] << ")";        //z

	debugText.Print(spherestr.str(), 50, 180, 1.0f);

	//球と平面の当たり判定
	XMVECTOR inter;
	bool hit = Collision::CheckSphere2Plane(sphere, plane,&inter);
	if (hit)
	{
		debugText.Print("HIT", 50, 200, 1.0f);
		//stringstreamをリセットし、交点座標を埋め込む
		spherestr.str("");
		spherestr.clear();
		spherestr<<"("
			<< std::fixed << std::setprecision(2)//小数点以下二桁まで
			<< sphere.center.m128_f32[0] << ","//x
			<< sphere.center.m128_f32[1] << ","//y
			<< sphere.center.m128_f32[2] << ")",//z

			debugText.Print(spherestr.str(), 50, 220, 1.0f);

		object3d_1->SetColor({ 1,0,0,1 });
	}
	else
	{
		object3d_1->SetColor({ 1,1,1,1 });
	}

	object3d_2->SetPosition({ plane.normal.m128_f32[0],plane.normal.m128_f32[1],plane.normal.m128_f32[2] });

	player_->SetInput(input);

	object3d_1->Update();
	object3d_2->Update();
	player_->Update();


	//スペースキーを押していたら
	if (input->PushKey(DIK_SPACE)) {
		//現在の座標を取得
		XMFLOAT2 position = sprite1->GetPosition();
		//移動後の座標を計算
		position.x += 1.0f;
		//座標の変更を反映
		sprite1->SetPosition(position);
	}

	//全ての衝突をチェック
	collisionManager->CheckAllCollisions();

}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
    //spriteBG->Draw();

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Object3d::PreDraw(cmdList);

	// 3Dオブクジェクトの描画
	object3d_1->Draw();
	object3d_2->Draw();
	//player_->Draw();
	//boxObject->Draw();

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	//sprite1->Draw();
	//sprite2->Draw();

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText.DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}
