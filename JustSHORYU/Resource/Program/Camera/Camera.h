#pragma once

/*

Camera

*/

#include<DirectXMath.h>
#include<SimpleMath.h>
#include"../Component/TransformComponent.h"

class Camera
{
public:

	Camera();

	void Update();

	//Transform
	void SetPosition(const DirectX::SimpleMath::Vector3& position) { transform.SetPosition(position); }
	void SetRotation(const DirectX::SimpleMath::Vector3& rotation) { transform.SetRotation(rotation); }

			TransformComponent& GetTransform()			{ return transform; }
	const	TransformComponent& GetTransform() const	{ return transform; }

	DirectX::SimpleMath::Vector3 GetPosition() const { return transform.GetPosition(); }
	DirectX::SimpleMath::Vector3 GetRotation() const { return transform.GetRotation(); }

	//LookAt
	void SetLookAt(const DirectX::SimpleMath::Vector3& target) { lookAt = target; }

	const DirectX::SimpleMath::Vector3 GetLookAt() const { return lookAt; }

	//Projection
	void SetPerspective(
		float fov,
		float aspectRatio,
		float nearClip, float farClip
	);

	void SetFov(float fov);
	void SetAspectRatio(float aspectRatio);
	void SetNearClip(float nearClip);
	void SetFarClip(float nearClip);

	float GetFov()			const { return fov; }
	float GetAspectRatio()	const { return aspectRatio; }
	float GetNearClip()		const { return nearClip; }
	float GetFarClip()		const { return farClip; }

	//Matrix
	const DirectX::SimpleMath::Matrix& GetrViewMatrix()		 const { return view; }
	const DirectX::SimpleMath::Matrix& GetProjectionMatrix() const { return proj; }

	//�Ǐ]
	void SetFollowTarget(TransformComponent* target)				 { followTarget = target; }
	void SetFollowOffset(const DirectX::SimpleMath::Vector3& offset) { followOffset = offset; }
	void SetFollowDistance(float distance)							 { followDistance = distance; }
	void SetFollowEnabled(bool enabled)								 { followEnabled = enabled; }
	void ClearFollowTarget();

	TransformComponent* GetFollowTarget() const { return followTarget; }

	DirectX::SimpleMath::Vector3& GetFollowOffset() const { return followOffset; }

	float GetFollowDistance() { return followDistance; }

	bool IsFollowEnabled() { return followEnabled; }


private:

	void UpdateFollow();
	void UpdateView();
	void UpdateProjection();

	TransformComponent transform;

	DirectX::SimpleMath::Vector3 lookAt;

	//Projection
	float fov;
	float aspectRatio;
	float nearClip;
	float farClip;

	//Matirx
	DirectX::SimpleMath::Matrix view;
	DirectX::SimpleMath::Matrix proj;

	//�Ǐ]�p
	TransformComponent*				followTarget;
	DirectX::SimpleMath::Vector3	followOffset;
	float							followDistance;
	bool							followEnabled;

};