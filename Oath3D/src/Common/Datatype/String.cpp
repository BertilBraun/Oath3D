#include "String.h"
#include "Common/Common.h"

#include <fstream>
#include <algorithm>

#include "System/Logger/Logger.h"

namespace Oath {

	String String::operator=(const String & o)
	{
		return std::string::operator=(o);
	}

	String String::operator+=(const String & o)
	{
		return std::string::operator+=(o);
	}

	Vector<String> String::split(const String & delim)
	{
		return split(*this, delim);
	}

	Vector<String> String::split(char delim)
	{
		return split(*this, delim);
	}

	String String::Trim(char c)
	{
		*this = Trim(*this, c);
		return *this;
	}

	String String::TrimTail(char c)
	{
		*this = TrimTail(*this, c);
		return *this;
	}

	String String::remove(char c)
	{
		*this = remove(*this, c);
		return *this;
	}

	String String::Trim(const String & str, char c)
	{
		int trims = 0;
		for (int i = (int)str.length() - 1; i >= 0; i--)
			if (str.at(i) == c)
				trims++;
			else
				break;

		return str.substr(0, str.length() - trims);
	}

	String String::TrimTail(const String & str, char c)
	{
		auto parts = String::split(str, c);
		String ret = "";

		for (int i = 0; i < (int)parts.size() - 1; i++)
			ret += parts[i] + c;

		return ret;
	}

	String String::remove(const String & str, char c)
	{
		String ret = "";

		for (char s : str)
			if (s != c)
				ret = ret + s;
		
		return ret;
	}

	String String::ToString(glm::vec3 val) {
		std::ostringstream s;
		s << "( " << val.x << " x : " << val.y << " y : " << val.z << " z )";
		return String(s.str());
	}

	String String::ToString(glm::vec2 val) {
		std::ostringstream s;
		s << "( " << val.x << " x : " << val.y << " y )";
		return String(s.str());
	}

	String String::ToUpper(const String & str)
	{
		String res = str;
		for (char& c : res)
			c = (char)toupper(c);
		return res;
	}

	String String::ToLower(const String & str)
	{
		String res = str;
		for (char& c : res)
			c = (char)tolower(c);
		return res;
	}
	
	Vector<String> String::split(const String & str, const String & delim)
	{
		Vector<String> tokens;
		size_t prev = 0, pos = 0;
		do
		{
			pos = str.find(delim, prev);

			if (pos == String::npos)
				pos = str.length();

			std::string token = str.substr(prev, pos - prev);

			if (!token.empty())
				tokens.push_back(token);
			prev = pos + delim.length();

		} while (pos < str.length() && prev < str.length());

		return tokens;
	}

	Vector<String> String::split(const String & str, char delim)
	{
		Vector<String> tokens;
		size_t prev = 0, pos = 0;
		do
		{
			pos = str.find(delim, prev);

			if (pos == String::npos)
				pos = str.length();

			std::string token = str.substr(prev, pos - prev);

			if (!token.empty() && token[0] != delim)
				tokens.push_back(token);
			prev = pos + 1;

		} while (pos < str.length() && prev < str.length());

		return tokens;
	}

	String String::folder(const String & in)
	{
		if (!in.empty())
		{
			String out;
			auto contents = split(in, "/");

			for (int i = (int)contents.size() - 2; i >= 0; i--)
				out += contents[i] + "/";

			return out;
		}
		return "";
	}

	String String::firstToken(const String & in)
	{
		if (!in.empty())
		{
			size_t token_start = in.find_first_not_of(" \t");
			size_t token_end = in.find_first_of(" \t", token_start);

			if (token_start != String::npos && token_end != String::npos)
				return in.substr(token_start, token_end - token_start);

			else if (token_start != String::npos)
				return in.substr(token_start);
		}
		return "";
	}

	bool String::isWhitespace(char c)
	{
		return c == ' ' || c == '\t' || c == '\n';
	}

	bool String::isWhitespace(const String & c)
	{
		for (char ch : c)
			if (!isWhitespace(ch))
				return false;
		return true;
	}

	String String::tail(const String & in)
	{
		size_t token_start = in.find_first_not_of(" \t");
		size_t space_start = in.find_first_of(" \t", token_start);
		size_t tail_start = in.find_first_not_of(" \t", space_start);
		size_t tail_end = in.find_last_not_of(" \t");

		if (tail_start != String::npos && tail_end != String::npos)
			return in.substr(tail_start, tail_end - tail_start + 1);

		else if (tail_start != String::npos)
			return in.substr(tail_start);
		return "";
	}

	std::ostream & operator<<(std::ostream & os, const String & s) {
		os << s.c_str();
		return os;
	}
}