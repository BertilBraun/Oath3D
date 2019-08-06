#include "Common/Common.h"
#include "Listener.h"

namespace Oath {

	const Vector<uint32>& ECS::Listener::getComponentIDs() {
		return componentIDs;
	}

	const Vector<uint32>& ECS::Listener::getComponentFlags() {
		return componentFlags;
	}

	void ECS::Listener::addComponent(uint32 id, uint32 flag) {
		
		if (componentIDs.contains(id)) {
			LOGERROR("CANNOT ADD COMPONENT OF THIS TYPE : ", id, " ITS ALREADY DEFINED!");
			return;
		}

		componentIDs.push_back(id);
		componentFlags.push_back(flag);
	}
}