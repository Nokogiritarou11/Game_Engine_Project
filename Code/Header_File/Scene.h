#pragma once
#include "DxSystem.h"
#include <DirectXMath.h>
#include "Shader.h"
#include "View.h"
#include "Light.h"
#include "Sprite.h"
#include "Texture.h"

//#include "CircleShadow.h"

//----------------------------------------------------
//		Scene(管理)クラス
//----------------------------------------------------

class Scene
{
private:
	std::unique_ptr<Shader> shader2D ;
	std::unique_ptr<Sprite> number;

protected:

	std::unique_ptr<Scene> newScene;
	//コンスタントバッファ
	ComPtr<ID3D11Buffer> ConstantBuffer;
	//コンスタントバッファ作成
	bool CreateConstantBuffer(ID3D11Buffer**ppCB, u_int size)
	{
		// 定数バッファ生成
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
		bd.ByteWidth = size;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;

		HRESULT hr = DxSystem::Device->CreateBuffer(&bd, NULL, ppCB);
		assert(SUCCEEDED(hr));

		return true;
	}
	//ビュー
	std::unique_ptr<View> view;
	XMFLOAT3 cpos;
	float cangle;
	float cdist;

	//ライト
	XMFLOAT3 LightDir;

	//ビュー・ライト操作
	void Operate() {

		const float camera_up_speed = 12.0f;
		if (GetKeyState('J') < 0)  cangle += DxSystem::elapsed_time;
		if (GetKeyState('L') < 0)  cangle -= DxSystem::elapsed_time;
		if (GetKeyState('I') < 0)  cpos.y += DxSystem::elapsed_time*camera_up_speed;
		if (GetKeyState('K') < 0)  cpos.y -= DxSystem::elapsed_time*camera_up_speed;
		cpos.x = sinf(cangle)* cdist;
		cpos.z = cosf(cangle)* cdist;
		view->Set(cpos, XMFLOAT3(0, 1, 0), XMFLOAT3(0, 1, 0));
		view->SetProjection(XM_PI / 8.0f, (FLOAT)DxSystem::GetScreenWidth() / (FLOAT)DxSystem::GetScreenHeight(), 0.1f, 500.0f);

	}


	static int sceneNumber;

	void NumberInitialize() {
		//シェーダー
		shader2D = std::make_unique<Shader>();
		shader2D->Create(L"2D.fx", "VSMain", "PSMain");
		//オブジェクト
		number = std::make_unique<Sprite>(L"ASSETS/TestNumber.png");
		//ビュー
		view = std::make_unique<View>();

	}
	void NumberRender(){

		//デプスステンシルステート設定
		DxSystem::DeviceContext->OMSetDepthStencilState(DxSystem::GetDephtStencilState(DxSystem::DS_FALSE), 1);
		number.get()->Render(shader2D.get(), 1100.0f, .0f, 64*2.0f, 32.0f, (sceneNumber/8)*256.0f, (sceneNumber%8)*64.0f, 256.0f, 64.0f);
		//デプスステンシルステート設定
		DxSystem::DeviceContext->OMSetDepthStencilState(DxSystem::GetDephtStencilState(DxSystem::DS_TRUE), 1);
	}
public:

	Scene() :newScene(nullptr), view(nullptr) {
		cangle = 0.0f; 
		cdist = 15.0f; 
		cpos = XMFLOAT3(0, 4, 20); 
		sceneNumber++;
	}
	virtual ~Scene() {}
	virtual bool Initialize() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

	//アクセサ
	void setScene(std::unique_ptr<Scene> scene) { newScene = std::move(scene); }
	std::unique_ptr<Scene> getScene() { return std::move(newScene); }
};
//----------------------------------------------------
//	SceneTest01「2D描画と合成」
//----------------------------------------------------

class SceneTest01 : public Scene
{
private:
	std::unique_ptr<Shader> shader3D = nullptr;
	std::unique_ptr<Shader> shader2D = nullptr;
	
	std::unique_ptr<Sprite> back = nullptr;
	std::unique_ptr<Sprite> fire1 = nullptr;
	std::unique_ptr<Sprite> fire2 = nullptr;


	
public:
	SceneTest01() {}
	~SceneTest01() {}

	bool Initialize();
	void Update();
	void Render();
};

//----------------------------------------------------
//	SceneTest02「スクリーンフィルタ」
//----------------------------------------------------

class SceneTest02 : public Scene
{
private:
	std::unique_ptr<Shader> shader3D = nullptr;
	std::unique_ptr<Shader> filter2D = nullptr;

	std::unique_ptr<Sprite> back = nullptr;
	std::unique_ptr<Sprite> fire1 = nullptr;
	std::unique_ptr<Sprite> fire2 = nullptr;


	//コンスタントバッファ構造体
	struct ConstantBufferForPerFrame
	{
		float bright;
		float contrast;
		float saturate;
		float blur;

		//XMFLOAT4	BCSD;			//bright/contrast/saturate/dumy
		XMFLOAT3	SCOLOR;			//screenColor
		float mosaic;
	};

public:
	SceneTest02() {}
	~SceneTest02() {}

	bool Initialize();
	void Update();
	void Render();
};

//----------------------------------------------------
//	SceneTest03「3D描画」
//----------------------------------------------------

class SceneTest03 : public Scene
{
private:
	std::unique_ptr<Shader> gouraud = nullptr;


	std::unique_ptr<CubeMesh> bisuko = nullptr;
	std::unique_ptr<CubeMesh> cube = nullptr;
	std::unique_ptr<CubeMesh> wood = nullptr;
	std::unique_ptr<CubeMesh>ground = nullptr;
	std::unique_ptr<SphereMesh> sphere = nullptr;


	//コンスタントバッファ構造体
	struct ConstantBufferForPerFrame
	{
		XMFLOAT4	LightColor;		//ライトの色
		XMFLOAT4	LightDir;		//ライトの方向
		XMFLOAT4	AmbientColor;	//環境光
	};

public:
	SceneTest03() {}
	~SceneTest03() {}

	bool Initialize();
	void Update();
	void Render();
};
//----------------------------------------------------
//	SceneTest04「ハイライト」
//----------------------------------------------------

class SceneTest04 : public Scene
{
private:
	std::unique_ptr<Shader> phong = nullptr;

	std::unique_ptr<CubeMesh> bisuko = nullptr;
	std::unique_ptr<CubeMesh> cube = nullptr;
	std::unique_ptr<CubeMesh> wood = nullptr;
	std::unique_ptr<CubeMesh>ground = nullptr;
	std::unique_ptr<SphereMesh> sphere = nullptr;


	//コンスタントバッファ構造体
	struct ConstantBufferForPerFrame
	{
		XMFLOAT4	LightColor;		//ライトの色
		XMFLOAT4	LightDir;		//ライトの方向
		XMFLOAT4	AmbientColor;	//環境光
		XMFLOAT4	EyePos;			//カメラ
	};

public:
	SceneTest04() {}
	~SceneTest04() {}

	bool Initialize();
	void Update();
	void Render();
};


//----------------------------------------------------
//	SceneTest05「半球ライト＆フォグ」
//----------------------------------------------------

class SceneTest05 : public Scene
{
private:
	//モデル
	std::unique_ptr<SphereMesh> sphere = nullptr;

	std::unique_ptr<OBJ_Mesh> model1 = nullptr;
	std::unique_ptr<OBJ_Mesh> model2 = nullptr;
	std::unique_ptr<OBJ_Mesh> field = nullptr;
	//シェーダー
	std::unique_ptr<Shader> modelShader = nullptr;
	std::unique_ptr<Shader> fogShader = nullptr;
	std::unique_ptr<Shader> hemiSphereLight = nullptr;

	//コンスタントバッファ構造体
	struct ConstantBufferForPerFrame
	{
		XMFLOAT4	LightColor;		//ライトの色
		XMFLOAT4	LightDir;		//ライトの方向
		XMFLOAT4	AmbientColor;	//環境光
		XMFLOAT4	EyePos;			//カメラ位置
	};

public:
	SceneTest05() {}
	~SceneTest05() {}

	bool Initialize();
	void Update();
	void Render();
};
//----------------------------------------------------
//	SceneTest06「環境マップ①」
//----------------------------------------------------

class SceneTest06 : public Scene
{
private:
	std::unique_ptr<Texture> env = nullptr;

	std::unique_ptr<SphereMesh> sphere = nullptr;

	std::unique_ptr<OBJ_Mesh> model1 = nullptr;
	std::unique_ptr<OBJ_Mesh> model2 = nullptr;
	std::unique_ptr<OBJ_Mesh> field = nullptr;
	std::unique_ptr<Shader> modelShader = nullptr;
	std::unique_ptr<Shader> fogShader = nullptr;
	std::unique_ptr<Shader> hemiSphereLight = nullptr;
	std::unique_ptr<Shader> enviromentShader = nullptr;
	std::unique_ptr<Shader> shader2D = nullptr;
	std::unique_ptr<Shader> cristal = nullptr;



	//コンスタントバッファ構造体
	struct ConstantBufferForPerFrame
	{
		XMFLOAT4	LightColor;		//ライトの色
		XMFLOAT4	LightDir;		//ライトの方向
		XMFLOAT4	AmbientColor;	//環境光
		XMFLOAT4	EyePos;			//カメラ
		XMFLOAT4	param1;			//環境マッピングブレンド率
		XMFLOAT4	param2;			//屈折率
		XMFLOAT4	param3;
		XMFLOAT4	param4;

	};
	float blendalpha = 0.5f;
	float refract = 0.02f;


public:
	SceneTest06() {}
	~SceneTest06() {}

	bool Initialize();
	void Update();
	void Render();
};


//----------------------------------------------------
//	SceneTest07「法線マップ・視差マップ」
//----------------------------------------------------

class SceneTest07 : public Scene
{
private:
	//モデル


	//std::unique_ptr<CubeMesh> cube = nullptr;
	std::unique_ptr<SphereMesh> earth = nullptr;

	std::unique_ptr<OBJ_Mesh> model1 = nullptr;
	std::unique_ptr<OBJ_Mesh> model2 = nullptr;
	std::unique_ptr<OBJ_Mesh> field = nullptr;
	//シェーダー
	std::unique_ptr<Shader> modelShader = nullptr;
	std::unique_ptr<Shader> normalMapShader = nullptr;
	std::unique_ptr<Shader> phoneShader = nullptr;

	std::unique_ptr<Texture> normalMap = nullptr;
	std::unique_ptr<Texture> heightMap = nullptr;

	std::unique_ptr<Shader> hemiSphereLight = nullptr;

	//コンスタントバッファ構造体
	struct ConstantBufferForPerFrame
	{
		XMFLOAT4	LightColor;		//ライトの色
		XMFLOAT4	LightDir;		//ライトの方向
		XMFLOAT4	AmbientColor;	//環境光
		XMFLOAT4	EyePos;			//カメラ
		float	param1;			//環境マッピングブレンド率
		float	param2;
		float	param3;
		float	param4;


	};

public:
	SceneTest07() {}
	~SceneTest07() {}

	bool Initialize();
	void Update();
	void Render();
};
//----------------------------------------------------
//	SceneTest08「ポイントライト」
//----------------------------------------------------

class SceneTest08 : public Scene
{
private:
	//モデル


	//std::unique_ptr<CubeMesh> cube = nullptr;
	std::unique_ptr<SphereMesh> earth = nullptr;
	std::unique_ptr<OBJ_Mesh> model1 = nullptr;
	std::unique_ptr<OBJ_Mesh> model2 = nullptr;
	std::unique_ptr<OBJ_Mesh> field = nullptr;
	//シェーダー
	std::unique_ptr<Shader> modelShader = nullptr;
	std::unique_ptr<Shader> normalMapShader = nullptr;
	std::unique_ptr<Shader> phoneShader = nullptr;


	std::unique_ptr<Shader> hemiSphereLight = nullptr;
	std::unique_ptr<Shader> pointLight = nullptr;
	std::unique_ptr<Shader> pointLightNormal = nullptr;

	std::unique_ptr<Texture> normalMap = nullptr;
	std::unique_ptr<Texture> heightMap = nullptr;


	//コンスタントバッファ構造体
	struct ConstantBufferForPerFrame
	{
		XMFLOAT4	LightColor;		//ライトの色
		XMFLOAT4	LightDir;		//ライトの方向
		XMFLOAT4	AmbientColor;	//環境光
		XMFLOAT4	EyePos;			//カメラ
		float	param1;			//環境マッピングブレンド率
		float	param2;
		float	param3;
		float	param4;

		POINTLIGHT  PointLight[Light::POINTMAX];//ポイントライト


	};

public:
	SceneTest08() {}
	~SceneTest08() {}

	bool Initialize();
	void Update();
	void Render();
};
//----------------------------------------------------
//	SceneTest09「スポットライト」
//----------------------------------------------------


class SceneTest09 : public Scene
{
private:
	//モデル


	//std::unique_ptr<CubeMesh> cube = nullptr;
	std::unique_ptr<SphereMesh> earth = nullptr;

	std::unique_ptr<OBJ_Mesh> model1 = nullptr;
	std::unique_ptr<OBJ_Mesh> model2 = nullptr;
	std::unique_ptr<OBJ_Mesh> field = nullptr;
	//シェーダー
	std::unique_ptr<Shader> modelShader = nullptr;
	std::unique_ptr<Shader> normalMapShader = nullptr;
	std::unique_ptr<Shader> phoneShader = nullptr;


	std::unique_ptr<Shader> hemiSphereLight = nullptr;
	std::unique_ptr<Shader> spotLight = nullptr;
	std::unique_ptr<Shader> pointLightNormal = nullptr;
	std::unique_ptr<Shader> spotLightNormal = nullptr;

	std::unique_ptr<Texture> normalMap = nullptr;
	std::unique_ptr<Texture> heightMap = nullptr;

	//コンスタントバッファ構造体
	struct ConstantBufferForPerFrame
	{
		XMFLOAT4	LightColor;		//ライトの色
		XMFLOAT4	LightDir;		//ライトの方向
		XMFLOAT4	AmbientColor;	//環境光
		XMFLOAT4	EyePos;			//カメラ
		float	param1;			//環境マッピングブレンド率
		float	param2;
		float	param3;
		float	param4;

		POINTLIGHT  PointLight[Light::POINTMAX];//ポイントライト
		SPOTLIGHT  SpotLight[Light::SPOTMAX];	//スポットライト


	};

public:
	SceneTest09() {}
	~SceneTest09() {}

	bool Initialize();
	void Update();
	void Render();
};

//----------------------------------------------------
//	SceneTest10「トゥーンシェーディング」
//----------------------------------------------------

class SceneTest10 : public Scene
{
private:
	std::unique_ptr<Texture> env = nullptr;
	std::unique_ptr<SphereMesh> sphere = nullptr;

	std::unique_ptr<OBJ_Mesh> model1 = nullptr;
	std::unique_ptr<OBJ_Mesh> model2 = nullptr;
	std::unique_ptr<OBJ_Mesh> field = nullptr;
	std::unique_ptr<Shader> modelShader = nullptr;
	std::unique_ptr<Shader> fogShader = nullptr;
	std::unique_ptr<Shader> hemiSphereLight = nullptr;
	std::unique_ptr<Shader> enviromentShader = nullptr;
	std::unique_ptr<Shader> toonShader = nullptr;

	//コンスタントバッファ構造体
	struct ConstantBufferForPerFrame
	{
		XMFLOAT4	LightColor;		//ライトの色
		XMFLOAT4	LightDir;		//ライトの方向
		XMFLOAT4	AmbientColor;	//環境光
		XMFLOAT4	EyePos;			//カメラ
		float	param1;			//環境マッピングブレンド率
		float	param2;
		float	param3;
		float	param4;

	};
	float blendalpha = 0.5f;
public:
	SceneTest10() {}
	~SceneTest10() {}

	bool Initialize();
	void Update();
	void Render();
};

//----------------------------------------------------
//	SceneTest11「シャドーマップ」
//----------------------------------------------------

class SceneTest11 : public Scene
{
private:
	//モデル

	std::unique_ptr<Texture> shadowMap = nullptr;
	std::unique_ptr<Sprite> test = nullptr;
	std::unique_ptr<SphereMesh> sphere = nullptr;

	std::unique_ptr<OBJ_Mesh> model1 = nullptr;
	std::unique_ptr<OBJ_Mesh> model2 = nullptr;
	std::unique_ptr<OBJ_Mesh> field = nullptr;
	//シェーダー
	std::unique_ptr<Shader> modelShader = nullptr;
	std::unique_ptr<Shader> fogShader = nullptr;
	std::unique_ptr<Shader> hemiSphereLight = nullptr;
	std::unique_ptr<Shader> shader2D = nullptr;
	std::unique_ptr<Shader> shader3D = nullptr;

	std::unique_ptr<Shader> toShadowMap = nullptr;
	std::unique_ptr<Shader> fromShadowMap = nullptr;

	//コンスタントバッファ構造体
	struct ConstantBufferForPerFrame
	{
		XMFLOAT4	LightColor;		//ライトの色
		XMFLOAT4	LightDir;		//ライトの方向
		XMFLOAT4	AmbientColor;	//環境光
		XMFLOAT4	EyePos;			//カメラ位置
		float	param1;			//環境マッピングブレンド率
		float	param2;
		float	param3;
		float	param4;
		XMFLOAT4X4 LightViewProjection;


	};
	XMFLOAT4X4 LightViewProjection;
	void RenderShadow();

public:
	SceneTest11() {}
	~SceneTest11() {}

	bool Initialize();
	void Update();
	void Render();
};

//----------------------------------------------------
//	SceneTest12
//----------------------------------------------------

class SceneTest12 : public Scene
{
private:

	//コンスタントバッファ構造体
	struct ConstantBufferForPerFrame
	{
	};

public:
	SceneTest12() {}
	~SceneTest12() {}

	bool Initialize();
	void Update();
	void Render();
};

//----------------------------------------------------
//	SceneTest13
//----------------------------------------------------

class SceneTest13 : public Scene
{
private:

	//コンスタントバッファ構造体
	struct ConstantBufferForPerFrame
	{
	};

public:
	SceneTest13() {}
	~SceneTest13() {}

	bool Initialize();
	void Update();
	void Render();
};
//----------------------------------------------------
//	SceneTest14「環境マップ②」
//----------------------------------------------------

class SceneTest14 : public Scene
{
private:
	std::unique_ptr<Texture> env = nullptr;
	std::unique_ptr<Texture> back = nullptr;

	std::unique_ptr<Sprite> test = nullptr;

	std::unique_ptr<SphereMesh> sphere = nullptr;

	std::unique_ptr<OBJ_Mesh> model1 = nullptr;
	std::unique_ptr<OBJ_Mesh> model2 = nullptr;
	std::unique_ptr<OBJ_Mesh> field = nullptr;
	std::unique_ptr<Shader> modelShader = nullptr;
	std::unique_ptr<Shader> fogShader = nullptr;
	std::unique_ptr<Shader> hemiSphereLight = nullptr;
	std::unique_ptr<Shader> enviromentShader = nullptr;
	std::unique_ptr<Shader> shader2D = nullptr;
	std::unique_ptr<Shader> cristal = nullptr;



	//コンスタントバッファ構造体
	struct ConstantBufferForPerFrame
	{
		XMFLOAT4	LightColor;		//ライトの色
		XMFLOAT4	LightDir;		//ライトの方向
		XMFLOAT4	AmbientColor;	//環境光
		XMFLOAT4	EyePos;			//カメラ
		XMFLOAT4	param1;			//環境マッピングブレンド率
		XMFLOAT4	param2;			//屈折率
		XMFLOAT4	param3;
		XMFLOAT4	param4;

	};
	float blendalpha = 0.5f;
	float refract = 0.02f;

	void RenderScreen();

public:
	SceneTest14() {}
	~SceneTest14() {}

	bool Initialize();
	void Update();
	void Render();
};
//----------------------------------------------------
//	SceneTest15
//----------------------------------------------------

class SceneTest15 : public Scene
{
private:

	//コンスタントバッファ構造体
	struct ConstantBufferForPerFrame
	{
	};

public:
	SceneTest15() {}
	~SceneTest15() {}

	bool Initialize();
	void Update();
	void Render();
};

//----------------------------------------------------
//	SceneTest16
//----------------------------------------------------

class SceneTest16 : public Scene
{
private:

	//コンスタントバッファ構造体
	struct ConstantBufferForPerFrame
	{
	};

public:
	SceneTest16() {}
	~SceneTest16() {}

	bool Initialize();
	void Update();
	void Render();
};


//----------------------------------------------------
//		ScenManager(管理)クラス
//----------------------------------------------------
class SceneManager
{
private:
	std::unique_ptr<Scene> scene;
public:
	SceneManager() :scene(nullptr){}
	~SceneManager() {}

	void Update();
	void Render();
	void ChangeScene(std::unique_ptr<Scene> newScene);
};
