#include "camera.h"

camera::camera(ID3D11DeviceContext* g_pImmediateContext)
{
	g_pImmediateContext->RSGetViewports(&num_viewports, &viewport);
}
/*
void camera::Init_Camera(shared_ptr<player> player)
{
	player_obj = player;
}
*/

void camera::Update()
{
	// プロジェクション行列を作成
	{
		// 角度をラジアン(θ)に変換
		float fov_y = XMConvertToRadians(30.0f);	// 画角
		float aspect = viewport.Width / viewport.Height;	// 画面比率
		float near_z = 0.1f;	// 表示最近面までの距離
		float far_z = 10000.0f;	// 表示最遠面までの距離

		XMStoreFloat4x4(&P, XMMatrixPerspectiveFovLH(fov_y, aspect, near_z, far_z));
	}
	// ビュー行列を作成
	// カメラの設定
	{
		XMVECTOR eye, focus, up;
		eye = XMVectorSet(pos.x, pos.y, pos.z, 1.0f);
		focus = XMVectorSet(look.x, look.y, look.z, 1.0f);
		up = XMVectorSet(rot.x, rot.y, rot.z, 1.0f);

		XMStoreFloat4x4(&V, XMMatrixLookAtLH(eye, focus, up));
	}
}

void camera::Update_FPS()
{
	// プロジェクション行列を作成
	// 画面サイズ取得のためビューポートを取得
	{
		// 角度をラジアン(θ)に変換
		float fov_y = XMConvertToRadians(30.0f);	// 画角
		float aspect = viewport.Width / viewport.Height;	// 画面比率
		float near_z = 0.1f;	// 表示最近面までの距離
		float far_z = 10000.0f;	// 表示最遠面までの距離

		XMStoreFloat4x4(&P, XMMatrixPerspectiveFovLH(fov_y, aspect, near_z, far_z));
	}
	// ビュー行列を作成
	// カメラの設定
	{
		XMMATRIX camRotationMatrix;
		XMVECTOR eye_v = XMLoadFloat4(&eye);
		XMVECTOR focus_v = XMLoadFloat4(&focus);
		XMVECTOR up_v = XMLoadFloat4(&up);

		//First Person Camera
		camRotationMatrix = XMMatrixRotationRollPitchYaw(camPitch, camYaw, 0);
		focus_v = XMVector3TransformCoord(DefaultForward, camRotationMatrix);
		focus_v = XMVector3Normalize(focus_v);

		XMMATRIX RotateYTempMatrix;
		RotateYTempMatrix = XMMatrixRotationY(camYaw);

		camRight = XMVector3TransformCoord(DefaultRight, RotateYTempMatrix);
		up_v = XMVector3TransformCoord(up_v, RotateYTempMatrix);
		camForward = XMVector3TransformCoord(DefaultForward, RotateYTempMatrix);

		eye_v += moveLeftRight * camRight;
		eye_v += moveBackForward * camForward;

		moveLeftRight = 0.0f;
		moveBackForward = 0.0f;

		focus_v = eye_v + focus_v;


		XMStoreFloat4(&eye, eye_v);
		XMStoreFloat4(&focus, focus_v);
		XMStoreFloat4(&up, up_v);
		XMStoreFloat4x4(&V, XMMatrixLookAtLH(eye_v, focus_v, up_v));
	}
}

void camera::Update_TPS(XMFLOAT3 pos)
{
	// プロジェクション行列を作成
	// 画面サイズ取得のためビューポートを取得
	{
		// 角度をラジアン(θ)に変換
		float fov_y = XMConvertToRadians(30.0f);	// 画角
		float aspect = viewport.Width / viewport.Height;	// 画面比率
		float near_z = 0.1f;	// 表示最近面までの距離
		float far_z = 10000.0f;	// 表示最遠面までの距離

		XMStoreFloat4x4(&P, XMMatrixPerspectiveFovLH(fov_y, aspect, near_z, far_z));
	}
	// ビュー行列を作成
	// カメラの設定
	{
		XMFLOAT4 look_pos = { pos.x,pos.y + 10,pos.z,0.0f };

		XMMATRIX camRotationMatrix;
		XMVECTOR eye_v = XMLoadFloat4(&eye);
		XMVECTOR focus_v = XMLoadFloat4(&look_pos);
		XMVECTOR up_v = XMLoadFloat4(&up);

		// Third Person Camera

		camRotationMatrix = XMMatrixRotationRollPitchYaw(-camPitch, camYaw, 0);
		eye_v = XMVector3TransformNormal(DefaultForward, camRotationMatrix);
		eye_v = XMVector3Normalize(eye_v);

		eye_v = (eye_v * 50) + focus_v;

		camForward = XMVector3Normalize(focus_v - eye_v);    // Get forward vector based on target
		camForward = XMVectorSetY(camForward, 0.0f);    // set forwards y component to 0 so it lays only on
		camForward = XMVector3Normalize(camForward);
		camRight = XMVectorSet(-XMVectorGetZ(camForward), 0.0f, XMVectorGetX(camForward), 0.0f);

		up_v = XMVector3Cross(XMVector3Normalize(eye_v - focus_v), camRight);

		XMStoreFloat4(&eye, eye_v);
		XMStoreFloat4(&focus, focus_v);
		XMStoreFloat4(&up, up_v);
		XMStoreFloat4x4(&V, XMMatrixLookAtLH(eye_v, focus_v, up_v));
	}
}

void camera::Update_LookAt(XMFLOAT3 root, XMFLOAT3 pos, float elapsed_time)
{
	// プロジェクション行列を作成
	// 画面サイズ取得のためビューポートを取得
	{
		// 角度をラジアン(θ)に変換
		float fov_y = XMConvertToRadians(30.0f);	// 画角
		float aspect = viewport.Width / viewport.Height;	// 画面比率
		float near_z = 0.1f;	// 表示最近面までの距離
		float far_z = 10000.0f;	// 表示最遠面までの距離

		XMStoreFloat4x4(&P, XMMatrixPerspectiveFovLH(fov_y, aspect, near_z, far_z));
	}
	// ビュー行列を作成
	// カメラの設定
	{
		XMFLOAT4 player_pos = { root.x,root.y,root.z,0.0f };
		float dis = (float)sqrt((double)(pow((double)pos.x - player_pos.x, 2) + pow((double)pos.z - player_pos.z, 2)));
		XMFLOAT4 target_pos = { pos.x,pos.y,pos.z,0.0f };
		float y = min(max(pos.y - dis / 6 + 5.0f, -10.0f), 5);
		XMFLOAT4 look_pos = { pos.x,y,pos.z,0 };

		XMMATRIX camRotationMatrix;
		XMVECTOR eye_v = XMLoadFloat4(&eye);
		XMVECTOR focus_v = XMLoadFloat4(&look_pos);
		XMVECTOR up_v = XMLoadFloat4(&up);
		XMVECTOR player_v = XMLoadFloat4(&player_pos);

		// Third Person Camera

		float degree_y = -XMConvertToDegrees(atan2((double)target_pos.z - player_pos.z, (double)target_pos.x - player_pos.x)) - 90;
		camRotationMatrix = XMMatrixRotationRollPitchYaw(0, XMConvertToRadians(degree_y), 0);
		eye_v = XMVector3TransformNormal(DefaultForward, camRotationMatrix);
		eye_v = XMVector3Normalize(eye_v);
		float dis_per_y = dis / 1000;
		float dis_per_z = dis / 750;
		eye_x = 0;
		eye_y = min(max(80 - dis_per_y * 2000, 35.0f), 80);
		if (eye_y <= 35.0f)
		{
			eye_y = min(max(eye_y + 5 - dis_per_y * 200, 25.0f), 35);
		}
		eye_z = min(max(0 + dis_per_z * 2000.0f, 2.5f), 40);
		eye_v = player_v + (eye_v * eye_z) + DefaultUp * eye_y + camRight * eye_x;
		/*
		if (!p->death)
		{
			if (!p->kill_wait)
			{
				float dis_per_y = dis / 1000;
				float dis_per_z = dis / 750;
				eye_x = 0;
				eye_y = min(max(80 - dis_per_y * 2000, 35.0f), 80);
				if (eye_y <= 35.0f)
				{
					eye_y = min(max(eye_y + 5 - dis_per_y * 200, 25.0f), 35);
				}
				eye_z = min(max(0 + dis_per_z * 2000.0f, 2.5f), 40);
				eye_v = player_v + (eye_v * eye_z) + DefaultUp * eye_y + camRight * eye_x;
			}
			else
			{
				if (p->kill_state == 0 || p->kill_state == 1)
				{
					lerp_timer = min(max(lerp_timer += elapsed_time / 2, 0.0f), 1.0f);
					float x = -7.5f;
					float y = 15.0f;
					float z = 30.0f;
					x = lerp(eye_x, x, lerp_timer);
					y = lerp(eye_y, y, lerp_timer);
					z = lerp(eye_z, z, lerp_timer);
					eye_v = player_v + (eye_v * z) + DefaultUp * y + camRight_set * x;
				}
				else
				{
					float x = -7.5f;
					float y = 10.0f;
					float z = -30.0f;
					eye_v = (DefaultForward * z) + DefaultUp * y + DefaultRight * x;
					XMFLOAT4 look = { -5,10,20,0 };
					focus_v = XMLoadFloat4(&look);
				}
			}
		}
		else
		{
			lerp_timer = min(max(lerp_timer += elapsed_time / 4, 0.0f), 1.0f);
			float x = 0.0f;
			float y = 40.0f;
			float z = 60.0f;
			x = lerp(eye_x, x, lerp_timer);
			y = lerp(eye_y, y, lerp_timer);
			z = lerp(eye_z, z, lerp_timer);
			eye_v = player_v + (eye_v * z) + DefaultUp * y + camRight_set * x;
		}*/
		camForward = XMVector3Normalize(focus_v - eye_v);    // Get forward vector based on target
		cam_Light = -camForward;
		camForward = XMVectorSetY(camForward, 0.0f);    // set forwards y component to 0 so it lays only on
		camForward = XMVector3Normalize(camForward);
		camRight = XMVectorSet(-XMVectorGetZ(camForward), 0.0f, XMVectorGetX(camForward), 0.0f);
		lerp_timer = 0;
		camRight_set = camRight;
		up_v = XMVector3Cross(XMVector3Normalize(eye_v - focus_v), camRight);
		//up_v = XMVectorSet(0, 1, 0, 0);

		XMStoreFloat4(&eye, eye_v);
		XMStoreFloat4(&focus, focus_v);
		XMStoreFloat4(&up, up_v);
		XMStoreFloat4x4(&V, XMMatrixLookAtLH(eye_v, focus_v, up_v));
	}
}

XMFLOAT2 camera::ToScreenPos(XMFLOAT3 pos)
{
	XMVECTOR p = XMLoadFloat3(&pos);
	XMVECTOR screen
		= XMVector3Project(
			p,
			viewport.TopLeftX,
			viewport.TopLeftY,
			viewport.Width,
			viewport.Height,
			viewport.MinDepth,
			viewport.MaxDepth,
			XMLoadFloat4x4(&P),
			XMLoadFloat4x4(&V),
			XMMatrixIdentity()
		);
	XMFLOAT2 re;
	XMStoreFloat2(&re, screen);
	return re;
}