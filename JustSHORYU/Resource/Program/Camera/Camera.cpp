#include"Camera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Camera::Camera()
	:transform(),
	lookAt(Vector3::Zero),
	fov(XMConvertToRadians(60.0f)),
	aspectRatio(16.0 / 9.0f),
	nearClip(0.1f), farClip(5000.0f),
	view(Matrix::Identity), proj(Matrix::Identity),
	followTarget(nullptr),
	followOffset(Vector3::Zero),
	followDistance(10.0f),
	followEnabled(false)
{

	//初期カメラ位置
	transform.SetPosition(Vector3(0.0f, 2.0f, -10.0f));

	//初期注視点
	lookAt = Vector3(0.0f, 1.0f, 0.0f);

	UpdateProjection();
	UpdateView();

}

void Camera::Update()
{

	if (followEnabled && followTarget != nullptr)
	{

		UpdateFollow();

	}

	UpdateView();

}

void Camera::UpdateView()
{

	const Vector3 position = transform.GetPosition();

	view = Matrix::CreateLookAt(
		position,
		lookAt,
		Vector3::Up
	);

}