#ifndef VAEZ_FILESYSTEM_VIRTUAL
#define VAEZ_FILESYSTEM_VIRTUAL

#include "../../../external/tklb/src/memory/TMemory.hpp"
#include "../../../external/tklb/src/util/TAssert.h"
#include "./vae_filesystem.hpp"


#define STRPOOL_MALLOC(ctx, size) (TKLB_DEFAULT_POOL.allocate(size))
#define STRPOOL_FREE(ctx, ptr)    (TKLB_DEFAULT_POOL.deallocate(ptr))
// #define STRPOOL_ASSERT(condition) (TKLB_ASSERT(condition))

// #define STRPOOL_MEMSET( ptr, val, cnt ) ( my_memset_func( ptr, val, cnt ) )
// #define STRPOOL_MEMCPY( dst, src, cnt ) ( my_memcpy_func( dst, src, cnt ) )
// #define STRPOOL_MEMCMP( pr1, pr2, cnt ) ( my_memcmp_func( pr1, pr2, cnt ) )
// #define STRPOOL_STRNICMP( s1, s2, len ) ( my_strnicmp_func( s1, s2, len ) )
#include "../../../external/strpool.h"

#define ASSETSYS_MALLOC(ctx, size) (TKLB_DEFAULT_POOL.allocate(size))
#define ASSETSYS_FREE(ctx, ptr)    (TKLB_DEFAULT_POOL.deallocate(ptr))
// #define ASSETSYS_ASSERT(condition) (TKLB_ASSERT(condition))
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
