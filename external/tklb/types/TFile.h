#ifndef TKLB_STACK
#define TKLB_STACK

#ifdef _WIN32
	#include "../external/dirent.h"
#else
	#include "dirent.h"
	#include <sys/stat.h>
#endif

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iomanip>
#include "../util/TAssert.h"
#include "../util/TPrint.h"

namespace tklb {

const std::string PATH_DELIMITER =
#ifdef _WIN32
	"\\";
#else
	"/";
#endif

struct FileInfo {
	std::string name;
	std::string relative;
	std::string absolute;
	bool isFolder = false;
	std::vector<FileInfo> children;

	FileInfo(const char* path) {
		if (isRelative(path)) {
			relative = path;
			absolute = path;
			// TKLB_ASSERT(false)
			// TODO figure out the absolute path
		} else {
			absolute = path;
			relative = "./";
		}
		toUnixPath(absolute);
		toUnixPath(relative);
		recursiveScan(*this, false);
	}

	FileInfo() = default;


	/**
	 * @brief Recursively scans the directory and its subfolder
	 */
	void scan() {
		recursiveScan(*this, true);
	}

	/**
	 * @brief check if the file has a .wav extension
	 */
	bool isWave() {
		return name.length() - name.find_last_of(".WAV") == 4
			|| name.length() - name.find_last_of(".wav") == 4;
	}

	/**
	 * @brief check if the file has a .json extension
	 */
	bool isJSON() {
		return name.length() - name.find_last_of(".JSON") == 5
			|| name.length() - name.find_last_of(".json") == 5;
	}

	/**
	 * @brief Will delete the file, does not work with folders
	 */
	bool remove() {
		const char* path = absolute.c_str();
		if (!isFolder) {
			return std::remove(path) == 0;
		}
		// TODO Folder deleteion
		TKLB_ASSERT(false);
		return false;
	}

	/**
	 * @brief Simple hash operation
	 */
	std::string hashFile() {
		const char* path = absolute.c_str();

		if (isFolder) {
			return ""; // Can't has folder
		}
		std::ifstream fp(path);
		std::stringstream ss;
		// Unable to hash file, return an empty hash.
		if (!fp.is_open()) {
			return "";
		}

		// Hashing
		uint32_t magic = 5381;
		char c;
		while (fp.get(c)) {
			magic = ((magic << 5) + magic) + c; // magic * 33 + c
		}

		ss << std::hex << std::setw(8) << std::setfill('0') << magic;
		return ss.str();
	}

	/**
	 * @brief Print the directory tree
	 */
	void print() {
		recursivePrint(*this, 0);
	}

	/**
	 * @brief Write to a file
	 * @param path The path to the file
	 * @param data The data buffer
	 * @param length The length of the data buffer
	 */
	static bool write(const char* path, const char* data, const size_t length) {
		try {
			auto file = std::fstream(path, std::ios::out | std::ios::binary);
			file.write(data, length);
			file.close();
			return true;
		} catch (...) {
			return false;
		}
	}

	/**
	 * @brief Concats multiple paths
	 * TODO trim path dilimters
	 */
	static std::string joinPath(const std::initializer_list<std::string>& paths) {
		std::string joined;
		for (auto &i : paths) {
			joined += i;
		}
		return joined;
	}

	/**
	 * @brief Replaces all pathdelimters to the platform ones
	 */
	static std::string platformPath(std::string path) {
		for (size_t i = 1; i < path.size(); i++) {
			if ((path[i - 1] == '/' || path[i - 1] == '\\') && (path[i] == '/' || path[i] == '\\')) {
				// need to get rid of double slashes
				TKLB_ASSERT(false);
			}
		}

		for (size_t i = 0; i < path.size(); i++) {
			if (path[i] == '/' || path[i] == '\\') {
				path[i] = PATH_DELIMITER[0];
			}
		}
		return path;
	}

	static bool isRelative(const std::string& path) {
		return path[0] == '.';
	}

	static void toUnixPath(std::string& path) {
		for (size_t i = 1; i < path.size(); i++) {
			if (path[i] == '\\') {
				path[i] = '/';
			}
		}
	}

private:
	void recursivePrint(FileInfo& info, int depth) {
		char space[256];
		std::fill_n(space, 256, 0);
		std::fill_n(space, depth, ' ');
		TKLB_PRINT("%s%s\n", space, info.name.c_str())
		for (auto &i : info.children) {
			recursivePrint(i, depth + 1);
		}
	}

	void recursiveScan(FileInfo& root, bool recursive) {
		root.children.clear();
		struct dirent** files = nullptr;
		const int count = scandir(root.absolute.c_str(), &files, nullptr, alphasort);
		if (count >= 0) {
			root.isFolder = true;
			appendDelimiter(root);
			for (int i = 0; i < count; i++) {
				const struct dirent* ent = files[i];
				if (ent->d_name[0] != '.') {
					FileInfo info;
					info.isFolder = ent->d_type == DT_DIR;
					info.name = ent->d_name;
					info.relative = root.relative + info.name;
					info.absolute = root.absolute + info.name;
					if (recursive && info.isFolder) {
						recursiveScan(info, true);
					}
					root.children.push_back(info);
				}
				free(files[i]);
			}
		} else {
			root.isFolder = false;
		}
		free(files);
	}

	void appendDelimiter(FileInfo& info) {
		if (info.relative.length() - info.relative.find_last_of('/') != 1) {
			info.relative = info.relative + "/";
		}
		if (info.absolute.length() - info.absolute.find_last_of('/') != 1) {
			info.absolute = info.absolute + "/";
		}
	}
};

} // namespace

#endif // TKLB_STACK
