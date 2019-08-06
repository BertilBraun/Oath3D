#pragma once

#include "Common/Common.h"

#include "System/Events/Dispatcher.h"

#define EVENT_CLASS_FUNC_CAST(func) this,	(Oath::Event::SubscriptionFunction) &func
#define EVENT_FUNC_CAST(func)				(Oath::Event::SubscriptionFunction) &func

namespace Oath {

	namespace Event {

		typedef void(*SubscriptionFunction)(void* sender, void* data);

		class Subscriber {
		public:
			Subscriber(void* owner, SubscriptionFunction func) : owner(owner), method(func) { }

			void operator()(void* arg) {
				ASSERT(owner != nullptr);

				(*method)(owner, arg);
			}

		protected:
			SubscriptionFunction method;
			void* owner;
		};
	}
}