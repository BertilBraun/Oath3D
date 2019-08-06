
#include "Common/Common.h"
#include "System/ECS/Manager.h"
#include "System/System/ToggleKey.h"

namespace Oath {

	namespace Comp {

		struct FreeMove : public ECS::Component<FreeMove> {

			FreeMove(float Speed = 1, Key::KeyboardKey sprintKey = Key::LEFT_SHIFT) : 
				Speed(Speed), sprintKey(sprintKey) {}

			float Speed;
			bool useGravity = false;
			Key::KeyboardKey sprintKey;
		};
	}
}