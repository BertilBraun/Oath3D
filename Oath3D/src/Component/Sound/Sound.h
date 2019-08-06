#pragma once

#include "System/ECS/Include.h"

namespace Oath {
	namespace Comp {

		struct Emitter : public ECS::Component<Emitter> {
			bool looping;
			float pitch, gain, rolloff;
			uint source = 0;
			String path = "";

			Emitter(const String& p, bool l = false, float g = 1, float pi = 1, float ro = 1) :
				path(p), looping(l), gain(g), pitch(pi), rolloff(ro) {}

			void setState(int s);
		};

		struct Reciever : public ECS::Component<Reciever> {};
	}
}