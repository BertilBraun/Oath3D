#pragma once

#include "System/Application/StateBase.h"
#include "System/Application/Application.h"

#include "System/Renderer/Lights/Lights.h"

#include "Component/Camera/Camera.h"
#include "Component/Camera/FreeLook.h"
#include "Component/Camera/FreeMove.h"

#include "Component/Asset/Asset.h"
#include "Component/Asset/AnimatedAsset.h"

#include "Component/Physik/Collider.h"
#include "Component/Physik/RigidBody.h"

#include "Component/Sound/Sound.h"

#include "System/Renderer/Skybox/Skybox.h"

namespace Oath {

	class ExampleBase : public APP::StateBase {
	public:
		ECS::Entity* cameraEntity = nullptr;

		ExampleBase(APP::Application* app, APP::StateBase* parent);

		void CheckForNewState();
	};
}