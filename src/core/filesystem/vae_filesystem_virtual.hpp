#ifndef VAEZ_FILESYSTEM_VIRTUAL
#define VAEZ_FILESYSTEM_VIRTUAL

#include "./vae_filesystem.hpp"
#include "../../../external/assetsys.h"

namespace vae { namespace core {

	class FileSystemVirtual final : public FileSystem {
		using String = tklb::HeapBuffer<char>;
		struct File {
			size_t offset;
			size_t size;
			String name;
			size_t parent;
		};

		struct Folder {
			String name;
			size_t parent;
		};

		tklb::HeapBuffer<char> mData;
		tklb::HeapBuffer<File> mFiles;
		tklb::HeapBuffer<Folder> mFolders;
	public:
		FileSystemVirtual() { }

		/**
		 * @brief Open existing filesystem read only
		 */
		bool open(const char* data, Size size = 0) {
			if (data == nullptr || size == 0) { return false; }
			mData.inject(data, size);
			return true;
		}

		/**
		 * @brief Get the in memory data
		 */
		char* data() {
			return mData.data();
		}

		bool exists(const char* path) override {
			return false;
		}

		Size getSize(const char* path) override {
			return 0;
		};

		bool list(const char* path, tklb::HeapBuffer<const char*>& result) override {
			return false;
		}

		Size readFile(const char* path, char* buf, Size bufferSize) override {
			return 0;
		}

		void readFile(const char* path, Callback& callback, Size chunkSize) override {

		}

		bool unlink(const char* path) override {
			return false;
		}

		bool writeFile(const char* path, const char* buf, Size size, Size offset = 0) override {
			return false;
		}
	};

} } // namespace vae::core

#endif // VAEZ_FILESYSTEM_VIRTUAL
