#ifndef _VAE_FS
#define _VAE_FS

#include <stddef.h>

void* vae_file_open(const char* path, const char* mode);
/**
 * @brief Seek and return current position
 *
 * @param file
 * @param seek
 * @return size_t
 */
size_t vae_file_seek(void* file, size_t, int seek);
size_t vae_file_read(char* dest, size_t size, int, void* file);
size_t vae_file_close(void* file);


#ifndef VAE_NO_STDIO
	#include <stdio.h>
	void* vae_file_open(const char* path, const char* mode) {
		return fopen(path, mode);
	}

	size_t vae_file_seek(void* file, size_t offset, int seek) {
		fseek((FILE*) file, offset, seek);
		return ftell((FILE*) file);
	}

	size_t vae_file_read(char* dest, size_t size, int, void* file) {
		auto chunksRead = fread(dest, size, 1, (FILE*)file);
		if (!chunksRead) {
			if (feof((FILE*)file))
			{
				return 1;
			}
		}
		return chunksRead;
	}

	size_t vae_file_close(void* file) {
		return fclose((FILE*) file);
	}
#endif

namespace vae { namespace core { namespace fs {
	class File {
		void* mHandle = nullptr;
		size_t mSize = 0;
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
			mHandle = vae_file_open(path, MODES[(size_t) mode]);
			if (mHandle == 0) { return; }
			mSize = vae_file_seek(mHandle, 0L, (int) Seek::end);
			vae_file_seek(mHandle, 0L, (int) Seek::set);
		}

		bool valid() const { return mHandle != nullptr; }

		size_t size() {
			if (mHandle == nullptr) { return 0; }
			return mSize;
		}

		bool readAll(char* dest) {
			if (mHandle == nullptr) { return false; }
			return vae_file_read(dest, size(), 1, mHandle) == 1;
		}

		~File() {
			if (mHandle == nullptr) {return; }
			vae_file_close(mHandle);
		}
	};
} } } // vae::core::fs

#endif
