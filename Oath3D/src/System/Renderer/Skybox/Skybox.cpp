#include "Common/Common.h"
#include "Skybox.h"

#include "System/Renderer/MasterRenderer.h"
#include "System/Renderer/Shader/Shader.h"
#include "System/Loader/Loader.h"

#define SKYBOX_SIZE 500

static Oath::Vector<GLfloat> skyboxVertecies {

	-SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE,
	-SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
	 SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
	 SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
	 SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE,
	-SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE,

	-SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE,
	-SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
	-SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE,
	-SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE,
	-SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE,
	-SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE,

	 SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
	 SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE,
	 SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE,
	 SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE,
	 SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE,
	 SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,

	-SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE,
	-SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE,
	 SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE,
	 SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE,
	 SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE,
	-SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE,

	-SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE,
	 SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE,
	 SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE,
	 SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE,
	-SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE,
	-SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE,

	-SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
	-SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE,
	 SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
	 SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
	-SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE,
	 SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE
};

Oath::Render::SkyboxComp::SkyboxComp(const String & path) :
	shader(Shader::GetShader("Skybox", "Skybox")),
	tex(System::Loader::LoadCubeMap(path))
{
	box.Init(skyboxVertecies, 3);
}

Oath::Render::SkyboxComp::~SkyboxComp()
{
	delete tex;
}

void Oath::Render::SkyboxComp::Draw() const {

	shader->start();
	shader->updateCamera(Comp::GCam);

	tex->Bind();

	box.bind();
	box.Draw();

	GLCall(glBindVertexArray(0));

	shader->Stop();
}

void Oath::Render::SkyboxListener::onMakeEntity(ECS::Entity * handle)
{
	GRenderer->process(handle->getComponent<Skybox>());
}

void Oath::Render::SkyboxListener::onRemoveEntity(ECS::Entity * handle)
{
	GRenderer->process((Skybox*)nullptr);
}

void Oath::Render::SkyboxListener::onAddComponent(ECS::Entity * handle, uint32 id)
{
	GRenderer->process(handle->getComponent<Skybox>());
}

void Oath::Render::SkyboxListener::onRemoveComponent(ECS::Entity * handle, uint32 id)
{
	GRenderer->process((Skybox*)nullptr);
}
