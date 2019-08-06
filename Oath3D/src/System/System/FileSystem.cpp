#include "Common/Common.h"
#include "FileSystem.h"
#include <experimental/filesystem>  

namespace Oath {
	namespace File {
		namespace fs = std::experimental::filesystem;
		namespace ch = std::chrono;

		String ReadAll(const String & path)
		{
			std::ifstream f(path, std::ios::ate);

			if (!f.good())
				return "";

			std::streampos size = f.tellg();
			char * memblock = new char[size];
			f.seekg(0, std::ios::beg);
			f.read(memblock, size);
			f.close();
			
			String s = std::move(memblock);

			delete[] memblock;

			return s;
		}

		Vector<String> ReadAllLines(const String & path)
		{
			String line;
			Vector<String> data;
			std::ifstream f(path, std::ios::beg);

			if (!f.good())
				return data;

			while (getline(f, line))
				data.emplace_back(line);

			f.close();

			return data;
		}

		void Append(const String & path, const String & data)
		{
			Vector<String> vec;
			vec.emplace_back(data);
			File::Append(path, vec);
		}

		void Append(const String & path, const Vector<String>& data)
		{
			std::ofstream f(path, std::ios::app);

			if (!f.good())
				return;

			for (const String& s : data)
				f << s.c_str() << "\n";

			f.close();
		}

		void Write(const String & path, const String & data)
		{
			Vector<String> vec;
			vec.emplace_back(data);
			File::Write(path, vec);
		}

		void Write(const String & path, const Vector<String>& data)
		{
			std::ofstream f(path, std::ios::trunc);

			if (!f.good())
				return;

			for (const String& s : data)
				f << s.c_str() << "\n";

			f.close();
		}

		bool FileExists(const String & path)
		{
			return fs::exists(path);
		}

		void CreateDirectorys(const String & path)
		{
			fs::create_directories(path);
		}

		void RemoveDirectorys(const String & path)
		{
			fs::remove_all(path);
		}

		Vector<String> AllInDirectory(const String & path)
		{
			Vector<String> paths;

			if (!fs::is_directory(path))
				return paths;

			for (auto const & entry : fs::directory_iterator(path))
				if (fs::is_directory(entry.status()))
					paths.insertEnd(paths, AllInDirectory(entry.path().string()));

				else if (fs::is_regular_file(entry.status()))
					paths.emplace_back(entry.path().string());
			
			return paths;
		}

		long long SinceLastMod(const String & path)
		{
			if (!fs::exists(path))
				return (long long)-1;

			time_t cftime = ch::system_clock::to_time_t(fs::last_write_time(path));

			time_t nowtime = ch::system_clock::to_time_t(ch::system_clock::now());

			return ch::system_clock::duration(nowtime - cftime).count();
		}

		long long LastMod(const String & path)
		{
			if (!fs::exists(path))
				return (long long) - 1;

			return ch::system_clock::duration(ch::system_clock::to_time_t(fs::last_write_time(path))).count();
		}

		uint GetFileSize(const String & path)
		{
			return uint(fs::file_size(path));
		}

		String GetCurrentPath()
		{
			return String(fs::current_path().string());
		}

		void DeleteAFile(const String& path)
		{
			fs::remove(path);
		}

		void ClearFile(const String & path)
		{
			std::ofstream f(path, std::ios::trunc);

			if (f.good())
				f.close();
		}

		void CopyFileTo(const String & from, const String & to)
		{
			fs::copy(from, to);
		}
	}
}