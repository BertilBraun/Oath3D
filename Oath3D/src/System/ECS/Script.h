#pragma once

namespace Oath {
	namespace ECS {

		class Entity;

		class Script {
		public:
			Script(Entity* e) : owner(e) {}

			virtual void Start() {}
			virtual void Update(float deltaTime) {}
			virtual void Render(float deltaTime) {}


		protected:
			Entity* owner = nullptr;
		};

	}
}