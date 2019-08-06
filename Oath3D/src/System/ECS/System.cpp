#include "Common/Common.h"
#include "System.h"

#include "FLAGS.h"

namespace Oath {

	namespace ECS {
		const Vector<uint32>& System::getComponentTypes()
		{
			return componentTypes;
		}

		const Vector<uint32>& System::getComponentFlags()
		{
			return componentFlags;
		}

		bool System::isValid()
		{
			for (auto& flag : componentFlags)
				if ((flag & FLAGS::FLAG_OPTIONAL) == 0)
					return true;

			return false;
		}

		void System::addComponent(uint32 componentType, uint32 componentFlag)
		{
			if (std::find(componentTypes.begin(), componentTypes.end(), componentType) != componentTypes.end())
			{
				LOGERROR("CANNOT ADD COMPONENT OF THIS TYPE : ", componentType, " ITS ALREADY DEFINED!");
				return;
			}

			componentTypes.push_back(componentType);
			componentFlags.push_back(componentFlag);
		}

	}
}