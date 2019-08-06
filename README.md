# Oath3D
3D Game Engine Framework WIP

Design:
  
  Everything is wrapped in a namespace OATH. In it there are namespaces for all major differences.

```c++
  namespace OATH {
    namespace Comp
    namespace App
    namespace LOGGER
    namespace Physic
    namespace WINDOW
    namespace System
    namespace Render
    namespace EVENT
 }
```

States are the base of the engine. You push and pop States as you please and once all are poped the game ends.
You can start the engine's main loop by calling `runLoop<>(args)`, pushing a state to the engine and starting the main game loop.

```c++
#include "Test.h"

void main() {

	LOG("STARTED");

	OATH::APP::Application app("Test", 1600, 1000);

	app.runLoop<OATH::TestState>(&app);
}
```

States are implementing a `StateBase` which provides overridable functions for updating, rendering, and init and shutdown.

```c++
class StateBase {
	StateBase(APP::Application* app, APP::StateBase* parent = nullptr);
	virtual ~StateBase() = default;

	virtual void update(float deltaTime) = 0;
	virtual void render() = 0;

	virtual void onOpen() {};
	virtual void onClose() {};
	virtual void onResume() {};

public:
	Physic::World* world;
	ECS::Manager ecs;
	ECS::SystemList rendering, updatelist;
protected:
	APP::Application* app;
	APP::StateBase* parent;
};
```
A basic example can be seen here, showing off some of the stuff this engine has to feature.

```c++
class TestState : public APP::StateBase {
public:
	TestState(APP::Application* app, APP::StateBase* parent = nullptr) : 
		APP::StateBase(app, parent)
	{
		Skybox sky("Skybox/");
		ecs.makeEntity(sky);

		AmbientLight alight(0.1f);
		ecs.makeEntity(alight);

		DirectionalLight dlight(vec3(1), 0.1f, 0.9f, vec3(0.2, -0.8, -0.2));
		ecs.makeEntity(dlight);

		PointLight pLight(vec3(1, 0, 0), 1, 1.2f, 0, 0, 0.3f);
		ecs.makeEntity(pLight)->pos(vec3(0, 5, 0));

		SpotLight sLight(vec3(0, 0, 1), 2, 2.2f, 0.5, 0.5, 0.5, vec3(0, 0, 1), 20);
		ecs.makeEntity(sLight);

		Asset sponza("Sponza/sponza.obj");
		ecs.makeEntity(sponza)->scale(vec3(0.1)).rot(vec3(0, 0, 180));

		AnimatedAsset anim("CharacterRunning.dae");
		ecs.makeEntity(anim)/*->pos(vec3(10, 0, 0)).rot(vec3(0, 0, 90))*/;

		Camera cam(true);
		Reciever r;
		FreeMove move(5);
		FreeLook look;
		look.rot = vec2(50, 30);
		ecs.makeEntity(cam, look, move, r);
	}

	void update(float deltaTime) override {

	}

	vec3 p, f, u;
	void render(float deltaTime) override {

		ImGui::Begin("WINDOW");

		ImGui::SliderFloat3("UP", glm::value_ptr(u), 0, 1);
		ImGui::SliderFloat3("POS", glm::value_ptr(p), -50, 50);
		ImGui::SliderFloat3("TARGET", glm::value_ptr(f), 0, 1);

		ImGui::End();
	}

};
```

Features:

- Fully functional ECS
  - Systems
    - Update based on Component Types
```c++
class CameraSystem : public ECS::System {
public:
    CameraSystem() {

      addComponentType(Camera::ID);
      addComponentType(FreeLook::ID, ECS::FLAG_OPTIONAL);
      addComponentType(FreeMove::ID, ECS::FLAG_OPTIONAL);
    }

    // This function gets called for every entity which has all of the Components nescessary for this system
    
    void updateComponents(float delta, ECS::Entity* entity, ECS::BaseComponent** components) override 
        Camera* camera = (Camera*)components[0];
        FreeLook* freeLook = (FreeLook*)components[1];
        FreeMove* freeMove = (FreeMove*)components[2];

        if (freeLook)
            UpdateFreeLook(camera, freeLook);

        if (freeMove) 
            UpdateFreeMove(delta, entity, camera, freeMove);
    }
};
```    
   - Components
      - pure data (basically)
      - own ID per type
      - owning entity
```c++
    struct Camera : public ECS::Component<Camera> {

      vec3 forward;
      vec3 right;
      vec3 up;

      mat4 view;
      mat4 proj;
    };
```
    
   - Entity
```c++
	bool enabled = true;
	Entity* parent = nullptr;
	String tag = "";
	Vector<Entity*> childs;
	Vector<Script*> scripts;
	Vector<std::pair<ComponentID, IndexInData>> components;
	Transform transform;
```

  - usage
```c++
ECS::Manager ecs; // Controls all entity's systems and listeners and updates on call of	void updateSystems(SystemList& systems, float delta);
 
ECS::SystemList rendering, mainlist; 
    
mainlist.addSystem(new CameraSystem());
rendering.addSystem(new AssertRenderer());
rendering.addSystem(new AnimationRenderer());
      
ecs.addListener(new CameraListener());
ecs.addListener(new AssetListener());
      
Asset asset("Earth/earth.obj", ASSET::DONT_RENDER_MESH1 | ASSET::DONT_RENDER_MESH2); // Asset is a Component
      
ECS::Entity* entity = ecs.makeEntity(asset); // Manager creates entity and adds all components passed into the function to the entity
// entity can be used to update transforms, or add children to be stacked in the scene tree

for (int i : range(0, 100)) {
        ecs.updateSystems(mainlist, 0.1f);
	ecs.updateSystems(rendering, 0.1f);
}
``` 

Sceene Tree:

- the entities in the sceene can have children. Children, sofar, inherit all transforms of its parents and add their own onto that.
```c++
void addChild(Entity* e); // adds child to 'this' entity, further on it inherits its transform
```
Physics:

- by adding a 'RigidBody' and / or a 'Collider', a entity gets added to the physics world.
   it then gets updated every physics frame with the specified parameters.
    
   - Collider
        - Bounding Box, specified by a `vec3 halfExtends`
        - Sphere, specified by a radius
        - Capsule, specified by a radius and a height
     
   - RigidBody
        - btRigidBodyConstructionInfo    
    
Events:

- Create Dispatcher, add Subscriber to Dispatcher with String as an EventType on which it gets updated.
  A Subscriber is a owner, which will be used to call on data, and a function which gets called once the EventType gets dispatched
   
   ```c++
    void TestFunc(void* sender, void* data) {
        LOG("CALLED");
    }

    EVENT::Dispatcher dispatcher;    

    EVENT::Subscriber* sub = new EVENT::Subscriber(this, EVENT_FUNC_CAST(&TestFunc));
    dispatcher.AddEventSubscriber("TEST", sub);
    dispatcher.AddEventSubscriber("COLLISION", this, &TestFunc);         

    dispatcher.DispatchEvent("TEST", this);  
   ```
      
  Window:
   
   ```c++
   Window window("NAME", WIDTH, HEIGHT);`
   ```
 
   Resource Manager:
  - stores Textures, Models, Aniamtions
  - releases all Data after nothing uses the Data anymore
  
  - loading Models using Assimp 
   
Render:

  	- Hotloading Shaders
  
  	Deffered
  		rendering
  		shading
  		shadows
  
  	lights
  		directional
  		point
  		cone
  		static / dynamic / no shadow mapping
  
  	Optimisation
  		Frustum culling
		LOD
		
Sound:
	loading of wav files
	emitter component updating sound
	reciever component
	
Assets:
	loading with assimp
	static Assets
	Animations
		interpolation between animations
		interpolation between keyframes
		
   	     
RoadMap:

Complete refactoring
	- Assets
	- ECS
	
ECS
	- https://www.youtube.com/watch?v=FmPZG4ETcMc after 1.05h
	
	
Use new Event System from one of my repos

Post Processing

	- Gamma correction
	- Object motion blur
	- Bloom
	- SSAO
	- Antialiasing
	- Lens flare
	- God ray's
	- Screen Space Reflections

Shadows

	- Point Light shadow mapping

Extras

	- Terrain (CDLOD)
	- Water
	- Grass

Fixes

	- Particle systems
	- Cascaded shadow mapping
	- Normal mapping
	- Parallax mapping
	- Texture Free
	- Frustum Culling


Dependencies:

	assimp
	Bullet
	GLFW
	glm
	ImGui
	stb_image
