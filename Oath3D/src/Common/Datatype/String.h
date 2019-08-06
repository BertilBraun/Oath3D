#pragma once

#include <string>
#include <sstream>
#include <iostream>

#include "Vector.h"
#include "glm/glm.hpp"

namespace Oath {

	class String : public std::string {
	public:
		String() : std::string() { }
		String(const char* o) : std::string(o) { }
		String(const String& o) : std::string(o) { }
		String(const std::basic_string<char, std::char_traits<char>, std::allocator<char>>& o) : std::string(o) { }
		String(std::basic_string<char, std::char_traits<char>, std::allocator<char>>&& o) : std::string(o) { }

		friend std::ostream& operator<<(std::ostream& os, const String& s);

		String operator = (const String& o);
		String operator += (const String& o);

		Vector<String> split(const String& delim);
		Vector<String> split(char delim);

		String Trim(char c = ' ');
		String TrimTail(char c);

		String remove(char c);

		template<typename T>
		static String ToString(T val) {
			std::ostringstream s;
			s << val;
			return String(s.str());
		}

		static String Trim(const String& str, char c = ' ');
		static String TrimTail(const String& str, char c);

		static String remove(const String& str, char c);

		static String ToString(glm::vec3 val);
		static String ToString(glm::vec2 val);

		static String ToUpper(const String& str);
		static String ToLower(const String& str);

		static Vector<String> split(const String& str, const String& delim);
		static Vector<String> split(const String& str, char delim);

		static String tail(const String &in);
		static String folder(const String &in);
		static String firstToken(const String &in);

		static bool isWhitespace(char c);
		static bool isWhitespace(const String & c);
	};
}