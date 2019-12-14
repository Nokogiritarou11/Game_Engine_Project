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
//		Scene(�Ǘ�)�N���X
//----------------------------------------------------

class Scene
{
private:
	std::unique_ptr<Shader> shader2D ;
	std::unique_ptr<Sprite> number;

protected:

	std::unique_ptr<Scene> newScene;
	//�R���X�^���g�o�b�t�@
	ComPtr<ID3D11Buffer> ConstantBuffer;
	//�R���X�^���g�o�b�t�@�쐬
	bool CreateConstantBuffer(ID3D11Buffer**ppCB, u_int size)
	{
		// �萔�o�b�t�@����
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
	//�r���[
	std::unique_ptr<View> view;
	XMFLOAT3 cpos;
	float cangle;
	float cdist;

	//���C�g
	XMFLOAT3 LightDir;

	//�r���[�E���C�g����
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
		//�V�F�[�_�[
		shader2D = std::make_unique<Shader>();
		shader2D->Create(L"2D.fx", "VSMain", "PSMain");
		//�I�u�W�F�N�g
		number = std::make_unique<Sprite>(L"ASSETS/TestNumber.png");
		//�r���[
		view = std::make_unique<View>();

	}
	void NumberRender(){

		//�f�v�X�X�e���V���X�e�[�g�ݒ�
		DxSystem::DeviceContext->OMSetDepthStencilState(DxSystem::GetDephtStencilState(DxSystem::DS_FALSE), 1);
		number.get()->Render(shader2D.get(), 1100.0f, .0f, 64*2.0f, 32.0f, (sceneNumber/8)*256.0f, (sceneNumber%8)*64.0f, 256.0f, 64.0f);
		//�f�v�X�X�e���V���X�e�[�g�ݒ�
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

	//�A�N�Z�T
	void setScene(std::unique_ptr<Scene> scene) { newScene = std::move(scene); }
	std::unique_ptr<Scene> getScene() { return std::move(newScene); }
};
//----------------------------------------------------
//	SceneTest01�u2D�`��ƍ����v
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
//	SceneTest02�u�X�N���[���t�B���^�v
//----------------------------------------------------

class SceneTest02 : public Scene
{
private:
	std::unique_ptr<Shader> shader3D = nullptr;
	std::unique_ptr<Shader> filter2D = nullptr;

	std::unique_ptr<Sprite> back = nullptr;
	std::unique_ptr<Sprite> fire1 = nullptr;
	std::unique_ptr<Sprite> fire2 = nullptr;


	//�R���X�^���g�o�b�t�@�\����
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
//	SceneTest03�u3D�`��v
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


	//�R���X�^���g�o�b�t�@�\����
	struct ConstantBufferForPerFrame
	{
		XMFLOAT4	LightColor;		//���C�g�̐F
		XMFLOAT4	LightDir;		//���C�g�̕���
		XMFLOAT4	AmbientColor;	//����
	};

public:
	SceneTest03() {}
	~SceneTest03() {}

	bool Initialize();
	void Update();
	void Render();
};
//----------------------------------------------------
//	SceneTest04�u�n�C���C�g�v
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


	//�R���X�^���g�o�b�t�@�\����
	struct ConstantBufferForPerFrame
	{
		XMFLOAT4	LightColor;		//���C�g�̐F
		XMFLOAT4	LightDir;		//���C�g�̕���
		XMFLOAT4	AmbientColor;	//����
		XMFLOAT4	EyePos;			//�J����
	};

public:
	SceneTest04() {}
	~SceneTest04() {}

	bool Initialize();
	void Update();
	void Render();
};


//----------------------------------------------------
//	SceneTest05�u�������C�g���t�H�O�v
//----------------------------------------------------

class SceneTest05 : public Scene
{
private:
	//���f��
	std::unique_ptr<SphereMesh> sphere = nullptr;

	std::unique_ptr<OBJ_Mesh> model1 = nullptr;
	std::unique_ptr<OBJ_Mesh> model2 = nullptr;
	std::unique_ptr<OBJ_Mesh> field = nullptr;
	//�V�F�[�_�[
	std::unique_ptr<Shader> modelShader = nullptr;
	std::unique_ptr<Shader> fogShader = nullptr;
	std::unique_ptr<Shader> hemiSphereLight = nullptr;

	//�R���X�^���g�o�b�t�@�\����
	struct ConstantBufferForPerFrame
	{
		XMFLOAT4	LightColor;		//���C�g�̐F
		XMFLOAT4	LightDir;		//���C�g�̕���
		XMFLOAT4	AmbientColor;	//����
		XMFLOAT4	EyePos;			//�J�����ʒu
	};

public:
	SceneTest05() {}
	~SceneTest05() {}

	bool Initialize();
	void Update();
	void Render();
};
//----------------------------------------------------
//	SceneTest06�u���}�b�v�@�v
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



	//�R���X�^���g�o�b�t�@�\����
	struct ConstantBufferForPerFrame
	{
		XMFLOAT4	LightColor;		//���C�g�̐F
		XMFLOAT4	LightDir;		//���C�g�̕���
		XMFLOAT4	AmbientColor;	//����
		XMFLOAT4	EyePos;			//�J����
		XMFLOAT4	param1;			//���}�b�s���O�u�����h��
		XMFLOAT4	param2;			//���ܗ�
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
//	SceneTest07�u�@���}�b�v�E�����}�b�v�v
//----------------------------------------------------

class SceneTest07 : public Scene
{
private:
	//���f��


	//std::unique_ptr<CubeMesh> cube = nullptr;
	std::unique_ptr<SphereMesh> earth = nullptr;

	std::unique_ptr<OBJ_Mesh> model1 = nullptr;
	std::unique_ptr<OBJ_Mesh> model2 = nullptr;
	std::unique_ptr<OBJ_Mesh> field = nullptr;
	//�V�F�[�_�[
	std::unique_ptr<Shader> modelShader = nullptr;
	std::unique_ptr<Shader> normalMapShader = nullptr;
	std::unique_ptr<Shader> phoneShader = nullptr;

	std::unique_ptr<Texture> normalMap = nullptr;
	std::unique_ptr<Texture> heightMap = nullptr;

	std::unique_ptr<Shader> hemiSphereLight = nullptr;

	//�R���X�^���g�o�b�t�@�\����
	struct ConstantBufferForPerFrame
	{
		XMFLOAT4	LightColor;		//���C�g�̐F
		XMFLOAT4	LightDir;		//���C�g�̕���
		XMFLOAT4	AmbientColor;	//����
		XMFLOAT4	EyePos;			//�J����
		float	param1;			//���}�b�s���O�u�����h��
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
//	SceneTest08�u�|�C���g���C�g�v
//----------------------------------------------------

class SceneTest08 : public Scene
{
private:
	//���f��


	//std::unique_ptr<CubeMesh> cube = nullptr;
	std::unique_ptr<SphereMesh> earth = nullptr;
	std::unique_ptr<OBJ_Mesh> model1 = nullptr;
	std::unique_ptr<OBJ_Mesh> model2 = nullptr;
	std::unique_ptr<OBJ_Mesh> field = nullptr;
	//�V�F�[�_�[
	std::unique_ptr<Shader> modelShader = nullptr;
	std::unique_ptr<Shader> normalMapShader = nullptr;
	std::unique_ptr<Shader> phoneShader = nullptr;


	std::unique_ptr<Shader> hemiSphereLight = nullptr;
	std::unique_ptr<Shader> pointLight = nullptr;
	std::unique_ptr<Shader> pointLightNormal = nullptr;

	std::unique_ptr<Texture> normalMap = nullptr;
	std::unique_ptr<Texture> heightMap = nullptr;


	//�R���X�^���g�o�b�t�@�\����
	struct ConstantBufferForPerFrame
	{
		XMFLOAT4	LightColor;		//���C�g�̐F
		XMFLOAT4	LightDir;		//���C�g�̕���
		XMFLOAT4	AmbientColor;	//����
		XMFLOAT4	EyePos;			//�J����
		float	param1;			//���}�b�s���O�u�����h��
		float	param2;
		float	param3;
		float	param4;

		POINTLIGHT  PointLight[Light::POINTMAX];//�|�C���g���C�g


	};

public:
	SceneTest08() {}
	~SceneTest08() {}

	bool Initialize();
	void Update();
	void Render();
};
//----------------------------------------------------
//	SceneTest09�u�X�|�b�g���C�g�v
//----------------------------------------------------


class SceneTest09 : public Scene
{
private:
	//���f��


	//std::unique_ptr<CubeMesh> cube = nullptr;
	std::unique_ptr<SphereMesh> earth = nullptr;

	std::unique_ptr<OBJ_Mesh> model1 = nullptr;
	std::unique_ptr<OBJ_Mesh> model2 = nullptr;
	std::unique_ptr<OBJ_Mesh> field = nullptr;
	//�V�F�[�_�[
	std::unique_ptr<Shader> modelShader = nullptr;
	std::unique_ptr<Shader> normalMapShader = nullptr;
	std::unique_ptr<Shader> phoneShader = nullptr;


	std::unique_ptr<Shader> hemiSphereLight = nullptr;
	std::unique_ptr<Shader> spotLight = nullptr;
	std::unique_ptr<Shader> pointLightNormal = nullptr;
	std::unique_ptr<Shader> spotLightNormal = nullptr;

	std::unique_ptr<Texture> normalMap = nullptr;
	std::unique_ptr<Texture> heightMap = nullptr;

	//�R���X�^���g�o�b�t�@�\����
	struct ConstantBufferForPerFrame
	{
		XMFLOAT4	LightColor;		//���C�g�̐F
		XMFLOAT4	LightDir;		//���C�g�̕���
		XMFLOAT4	AmbientColor;	//����
		XMFLOAT4	EyePos;			//�J����
		float	param1;			//���}�b�s���O�u�����h��
		float	param2;
		float	param3;
		float	param4;

		POINTLIGHT  PointLight[Light::POINTMAX];//�|�C���g���C�g
		SPOTLIGHT  SpotLight[Light::SPOTMAX];	//�X�|�b�g���C�g


	};

public:
	SceneTest09() {}
	~SceneTest09() {}

	bool Initialize();
	void Update();
	void Render();
};

//----------------------------------------------------
//	SceneTest10�u�g�D�[���V�F�[�f�B���O�v
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

	//�R���X�^���g�o�b�t�@�\����
	struct ConstantBufferForPerFrame
	{
		XMFLOAT4	LightColor;		//���C�g�̐F
		XMFLOAT4	LightDir;		//���C�g�̕���
		XMFLOAT4	AmbientColor;	//����
		XMFLOAT4	EyePos;			//�J����
		float	param1;			//���}�b�s���O�u�����h��
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
//	SceneTest11�u�V���h�[�}�b�v�v
//----------------------------------------------------

class SceneTest11 : public Scene
{
private:
	//���f��

	std::unique_ptr<Texture> shadowMap = nullptr;
	std::unique_ptr<Sprite> test = nullptr;
	std::unique_ptr<SphereMesh> sphere = nullptr;

	std::unique_ptr<OBJ_Mesh> model1 = nullptr;
	std::unique_ptr<OBJ_Mesh> model2 = nullptr;
	std::unique_ptr<OBJ_Mesh> field = nullptr;
	//�V�F�[�_�[
	std::unique_ptr<Shader> modelShader = nullptr;
	std::unique_ptr<Shader> fogShader = nullptr;
	std::unique_ptr<Shader> hemiSphereLight = nullptr;
	std::unique_ptr<Shader> shader2D = nullptr;
	std::unique_ptr<Shader> shader3D = nullptr;

	std::unique_ptr<Shader> toShadowMap = nullptr;
	std::unique_ptr<Shader> fromShadowMap = nullptr;

	//�R���X�^���g�o�b�t�@�\����
	struct ConstantBufferForPerFrame
	{
		XMFLOAT4	LightColor;		//���C�g�̐F
		XMFLOAT4	LightDir;		//���C�g�̕���
		XMFLOAT4	AmbientColor;	//����
		XMFLOAT4	EyePos;			//�J�����ʒu
		float	param1;			//���}�b�s���O�u�����h��
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

	//�R���X�^���g�o�b�t�@�\����
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

	//�R���X�^���g�o�b�t�@�\����
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
//	SceneTest14�u���}�b�v�A�v
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



	//�R���X�^���g�o�b�t�@�\����
	struct ConstantBufferForPerFrame
	{
		XMFLOAT4	LightColor;		//���C�g�̐F
		XMFLOAT4	LightDir;		//���C�g�̕���
		XMFLOAT4	AmbientColor;	//����
		XMFLOAT4	EyePos;			//�J����
		XMFLOAT4	param1;			//���}�b�s���O�u�����h��
		XMFLOAT4	param2;			//���ܗ�
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

	//�R���X�^���g�o�b�t�@�\����
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

	//�R���X�^���g�o�b�t�@�\����
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
//		ScenManager(�Ǘ�)�N���X
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
