#include "Common/Common.h"
#include "System/Events/Dispatcher.h"
#include "System/Events/Subscriber.h"

namespace Oath {

	namespace Event {

		Dispatcher::~Dispatcher()
		{
			for (auto pair : subscriber)
				for (auto sub : pair.second)
					SAVE_DELETE(sub)
		}

		void Dispatcher::AddEventSubscriber(const EventType& ID, void * owner, SubscriptionFunction func)
		{
			AddEventSubscriber(ID, new Subscriber(owner, func));
		}

		void Dispatcher::AddEventSubscriber(const EventType& ID, Subscriber* requestor) {
			subscriber[ID].push_back(requestor);
		}
	}
}