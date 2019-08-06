#pragma once

#include <vector>

namespace Oath {

	template<typename T>
	class Vector : public std::vector<T> {
	public:
		Vector()							: std::vector<T>( ) { }
		Vector(size_t o)					: std::vector<T>(o) { }
		Vector(std::vector<T>&& o)			: std::vector<T>(o) { }
		Vector(const std::vector<T>& o)		: std::vector<T>(o) { }
		Vector(std::initializer_list<T> o)	: std::vector<T>(o) { }

		void swapRemove(int index) {
			std::swap((*this)[index], (*this)[this->size() - 1]);
			this->pop_back();
		}

		static void insertEnd(Vector<T>& toInsert, const Vector<T>& data) {

			toInsert.insert(toInsert.end(), data.begin(), data.end());
		}

		void insertEnd(const Vector<T>& data) {

			this->insert(this->end(), data.begin(), data.end());
		}

		bool containsAll(const std::vector<T>& o) {
	
			if (o.size() > this->size())
				return false;

			for (int i = 0; i < this->size(); i++)
				if (o[i] != (*this)[i])
					return false;

			return true;
		}

		bool contains(const T& val) {
			return std::find(this->begin(), this->end(), val) != this->end();
		}

		template<class... _Valty>
		auto emplace_back(_Valty&&... _Val) {

			if (this->size() == this->capacity())
				this->reserve(this->capacity() * 2);

			return std::vector<T>::emplace_back(std::forward<_Valty>(_Val)...);
		}

		void push_back(const T& _Val)
		{
			emplace_back(_Val);
		}

		void push_back(T&& _Val)
		{
			emplace_back(std::move(_Val));
		}
	};
}