#ifndef VAEZ_FILESYSTEM_PROXY
#define VAEZ_FILESYSTEM_PROXY

#include "./vae_filesystem.hpp"

namespace vae { namespace core {

	class FileSystemProxy final : public FileSystem {
	public:
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

#endif // VAEZ_FILESYSTEM_PROXY
