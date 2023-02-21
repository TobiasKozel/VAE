/**
 * @file vae_fs.hpp
 * @author Tobias Kozel
 * @brief Wrap stdlib file operations
 * @version 0.1
 * @date 2022-08-15
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef _VAE_FS
#define _VAE_FS

#include "./vae_tklb.hpp"

/**
 * @brief Open a file handle
 * @param path file path
 * @param mode Read mode "r", "w", "rw" like normal fopen
 * @return void* The handle to the file
 */
void* vae_file_open(const char* path, const char* mode);

/**
 * @brief Seek and return current position
 * @param file handle
 * @param offset offset used for set and cur
 * @param seek position set = 0, cur = 1, end = 1
 * @return SizeT TODO bytes read head moved?
 */
vae::core::SizeT vae_file_seek(void* file, long int offset, int seek);

/**
 * @brief Read number of bytes from current position into buffer
 * @param dest buffer to write into
 * @param size bytes to read
 * @param n Number of blocks of size to read
 * @param file File handle
 * @return SizeT TODO bytes read?
 */
vae::core::SizeT vae_file_read(char* dest, vae::core::SizeT size, int n, void* file);

/**
 * @brief Close file
 * @param file File Handle
 * @return SizeT TODO Success code
 */
vae::core::SizeT vae_file_close(void* file);

#if !defined(VAE_NO_STDIO)
	// Direct the calls to the cstd if allowed
	#include <stdio.h>
	#if defined(VAE_IMPL)
		void* vae_file_open(const char* path, const char* mode) {
			return fopen(path, mode);
		}

		vae::core::SizeT vae_file_seek(void* file, long int offset, int seek) {
			fseek((FILE*) file, offset, seek);
			return ftell((FILE*) file);
		}

		vae::core::SizeT vae_file_read(char* dest, vae::core::SizeT size, int n, void* file) {
			return fread(dest, size, n, (FILE*) file);
		}

		vae::core::SizeT vae_file_close(void* file) {
			return fclose((FILE*) file);
		}
	#endif // VAE_IMPL
#endif // VAE_NO_STDIO

namespace vae { namespace core { namespace fs {
	class File {
		void* mHandle = nullptr;
		SizeT mSize = 0;
		enum class Seek {
	#ifdef VAE_NO_STDIO
			set = 0, cur, end
	#else
			set = SEEK_SET, cur = SEEK_CUR, end = SEEK_END
	#endif
		};
	public:
		enum class Mode {
			Read,
			Write,
			ReadWrite
		};

		File(const char* path, Mode mode = Mode::Read) {
			const char* MODES[3] = { "r", "w", "rw" };
			mHandle = vae_file_open(path, MODES[(SizeT) mode]);
			if (mHandle == 0) { return; }
			mSize = vae_file_seek(mHandle, 0L, (int) Seek::end);
			vae_file_seek(mHandle, 0L, (int) Seek::set);
		}

		bool valid() const { return mHandle != nullptr; }

		SizeT size() {
			if (!valid()) { return 0; }
			return mSize;
		}

		bool readAll(char* dest) {
			if (!valid()) { return false; }
			return vae_file_read(dest, size(), 1, mHandle) == 1;
		}

		bool read(char* dest, SizeT bytes) {
			if (!valid()) { return false; }
			return vae_file_read(dest, bytes, 1, mHandle) == 1;
		}

		~File() {
			if (!valid()) {return; }
			vae_file_close(mHandle);
		}
	};
} } } // vae::core::fs

#endif
