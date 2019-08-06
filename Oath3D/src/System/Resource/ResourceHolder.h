#pragma once

#include "Common/Common.h"

namespace Oath {
	namespace Comp {
			   
		template<typename T>
		class ResourceHolder {
		public:
			typedef T(*LoadFunc)(const String& path);
			typedef void(*FreeFunc)(T instance);

			ResourceHolder(LoadFunc loadFunc, FreeFunc freeFunc = [](T instance) {}) :
				loadFunc(loadFunc), freeFunc(freeFunc) {}

			~ResourceHolder() { 

				for (auto it = data.begin(); it != data.end(); it++)
					freeFunc(it->second.first);
			}

			T getData(const std::string& path) {

				auto it = data.find(path);

				if (it == data.end()) {
					data.emplace(path, std::make_pair(loadFunc(path), 1));

					return data.at(path).first;
				}

				it->second.second++;
				return it->second.first;
			}

			void freeData(const std::string& path) {

				if (data.size() <= 0)
					return;

				auto it = data.find(path);

				if (it == data.end())
					return;

				if (--it->second.second <= 0)
					return;

				freeFunc(it->second.first);
				data.erase(it);
			}

		private:
			std::unordered_map<std::string, std::pair<T, int>> data;

			LoadFunc loadFunc;
			FreeFunc freeFunc;
		};

	}
}
