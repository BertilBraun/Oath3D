#pragma once

#include "Common/Common.h"
#include "Subscriber.h"

namespace Oath {

	using EventType = String;

	namespace Event {

		class Dispatcher {
		private:
			std::map<EventType, Vector<Subscriber*>> subscriber;

			Dispatcher(const Dispatcher&);            // disallow copying
			Dispatcher& operator=(const Dispatcher&); // disallow copying
		public:
			Dispatcher() {}
			~Dispatcher();

			void AddEventSubscriber(const EventType& ID, void* owner, SubscriptionFunction func);
			void AddEventSubscriber(const EventType& ID, Subscriber* requestor);

			template<typename T>
			void DispatchEvent(const EventType& ID, T* eventData) {
				if (!subscriber.size() || !subscriber[ID].size() || subscriber.find(ID) == subscriber.end())
					return;

				for (auto sub : subscriber[ID])
					(*sub)((void*)eventData);
			}
		};

	}
}