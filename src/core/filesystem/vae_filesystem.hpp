#ifndef VAEZ_FILESYSTEM
#define VAEZ_FILESYSTEM

#include <functional>
#include "../../../external/tklb/src/types/THeapBuffer.hpp"

namespace vae { namespace core {
	/**
	 * @brief Filesystem interface
	 */
	class FileSystem {
	public:
		using Size = unsigned int;
		using Callback = std::function<void(char* data, Size offset, Size size)>;

		/**
		 * @brief Check if file/directory exists.
		 */
		virtual bool exists(const char* path) = 0;

		/**
		 * @brief returns size of the file or zero if file not found.
		 * @param path File path
		 * @return size in bytes or 0 for error.
		 */
		virtual Size getSize(const char* path) = 0;

		virtual bool list(const char* path, tklb::HeapBuffer<const char*>& result) = 0;

		/**
		 * @brief Sync file loading
		 * @param path File path
		 * @param buf Output buffer
		 * @param bufferSize Allocated size for buf to prevent overruns
		 * @return Bytes read and stored in buf, 0 for error
		 */
		virtual Size readFile(const char* path, char* buf, Size bufferSize) = 0;

		/**
		 * @brief Async file loading, will call callback function with data when it's loaded
		 * @param path File path
		 * @param callback Callback function data is passed to. Will be called at least once, even on error.
		 * @param chunkSize Size of chunks. 0 for the entire file any other value to stream the data
		 */
		virtual void readFile(const char* path, Callback& callback, Size chunkSize = 0) = 0;

		/**
		 * @brief Delete file/folder
		 * @return True on success
		 */
		virtual bool unlink(const char* path) = 0;

		/**
		 * @brief Write file from buffer
		 * @param path Filepath, can already exist
		 * @param buf data
		 * @param size Size of data. If file exists, this can not exceed original file size!
		 * @param offset if the file exists, this allows writing to a specific region.
		 */
		virtual bool writeFile(const char* path, const char* buf, Size size, Size offset = 0) = 0;
	};

} } // namespace vae::core

#endif // VAEZ_FILESYSTEM
