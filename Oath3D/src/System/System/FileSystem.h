#pragma once

#include "Common/Common.h"

namespace Oath {
	namespace File {

		String ReadAll(const String& path);
		Vector<String> ReadAllLines(const String& path);

		void Append(const String& path, const String& data);
		void Append(const String& path, const Vector<String>& data);

		void Write(const String& path, const String& data);
		void Write(const String& path, const Vector<String>& data);

		bool FileExists(const String& path);

		void CreateDirectorys(const String& path);
		void RemoveDirectorys(const String& path);

		Vector<String> AllInDirectory(const String& path);

		long long SinceLastMod(const String& path);
		long long LastMod(const String& path);

		uint GetFileSize(const String& path);
		String GetCurrentPath();

		void DeleteAFile(const String& path);
		void ClearFile(const String& path);
		void CopyFileTo(const String& from, const String& to);
	}
}