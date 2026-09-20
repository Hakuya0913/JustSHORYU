#pragma once

/*

モデルの描画を行うコンポーネント

*/

#include"TransformComponent.h"
#include"ModelComponent.h"
#include"MaterialComponent.h"

class RenderComponent
{
public:

	RenderComponent() = default;
	~RenderComponent() = default;

	//Setter
	void SetTransform(TransformComponent& transform);
	void SetModel(ModelComponent& model);
	void SetMaterial(MaterialComponent& material);

	//Getter
	TransformComponent& GetTransform() const	{ return *transformComponent; }
	ModelComponent& GetModel() const { return *modelComponent; }
	MaterialComponent& GetMaterial() const	{ return *materialComponent; }

private:

	//描画に使用するコンポーネントへの参照
	TransformComponent* transformComponent;
	ModelComponent* modelComponent;
	MaterialComponent* materialComponent;

};