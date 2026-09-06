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

	RenderComponent(
		TransformComponent& transform,
		ModelComponent& model,
		MaterialComponent& material
	);
	~RenderComponent() = default;

	//Getter
			TransformComponent& GetTransform()			{ return transformComponent; }
	const	TransformComponent& GetTransform() const	{ return transformComponent; }

			ModelComponent& GetModel()			{ return modelComponent; }
	const	ModelComponent& GetModel() const	{ return modelComponent; }

			MaterialComponent& GetMaterial()		{ return materialComponent; }
	const	MaterialComponent& GetMaterial() const	{ return materialComponent; }

private:

	//描画に使用するコンポーネントへの参照
	TransformComponent& transformComponent;
	ModelComponent& modelComponent;
	MaterialComponent& materialComponent;

};