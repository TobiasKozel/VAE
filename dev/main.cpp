#include <stdio.h>
#define TKLB_ASSERT_SEGFAULT
#include "core/device/vae_portaudio.hpp"

#include "core/filesystem/vae_filesystem_virtual.hpp"

using namespace vae;
using namespace core;


int pool() {
	strpool_config_t conf = strpool_default_config;
	//conf.ignore_case = true;

	strpool_t pool;
	strpool_init(&pool, &conf);

	STRPOOL_U64 str_a = strpool_inject(&pool, "This is a test string", (int) strlen("This is a test string"));
	STRPOOL_U64 str_b = strpool_inject(&pool, "THIS IS A TEST STRING", (int) strlen("THIS IS A TEST STRING"));

	printf("%s\n", strpool_cstr(&pool, str_a));
	printf("%s\n", strpool_cstr(&pool, str_b));
	printf("%s\n", str_a == str_b ? "Strings are the same" : "Strings are different");

	strpool_term(&pool);
	return 0;
	return 0;
}

void list_assets(assetsys_t* assetsys, char const* path, int indent) {
	// Print folder names and recursively list assets
	for (int i = 0; i < assetsys_subdir_count(assetsys, path); i++) {
		char const* subdir_name = assetsys_subdir_name(assetsys, path, i);
		for(int j = 0; j < indent; ++j) printf("  ");
		printf("%s/\n", subdir_name);

		char const* subdir_path = assetsys_subdir_path(assetsys, path, i);
		list_assets(assetsys, subdir_path, indent + 1);
	}

	// Print file names
	for (int i = 0; i < assetsys_file_count(assetsys, path); i++) {
		char const* file_name = assetsys_file_name(assetsys, path, i);
		for(int j = 0; j < indent; ++j) printf("  ");
		printf("%s\n", file_name);
	}
}

int asset() {
	assetsys_t* assetsys = assetsys_create(0);

	// Mount current working folder as a virtual "/data" path
	assetsys_mount(assetsys, "./test.zip", "/data");

	// Print all files and subfolders
	list_assets(assetsys, "/", 0); // Start at root

	// Load a file
	assetsys_file_t file;
	assetsys_file(assetsys, "/data/text.txt", &file);
	int size = assetsys_file_size(assetsys, file);
	char* content = (char*) malloc(size + 1); // extra space for '\0'
	int test = 0;
	assetsys_file_load(assetsys, file, nullptr, content, size + 1);
	content[ size ] = '\0'; // zero terminate the text file
	printf("%s\n", content);
	free(content);

	assetsys_destroy(assetsys);
	return 0;
}

int sine() {
	DeviceInfo devices[100];
	auto& backend = BackendPortAudio::instance();
	for (unsigned int i = 0; i < backend.getDeviceCount(); i++) {
		devices[i] = backend.getDevice(i);
	}

	// DevicePortaudio inputDevice; // async input device
	// inputDevice.openDevice(devices[5]);

	size_t time = 0;
	Device::SyncCallback callback =
	[&](const Device::AudioBuffer& fromDevice, Device::AudioBuffer& toDevice) {
		for (int c = 0; c < toDevice.channels(); c++) {
			for (unsigned int i = 0; i < toDevice.validSize(); i++) {
				toDevice[c][i] = sin(double(time + i) * 0.1) * 0.8;
			}
		}
		time += toDevice.validSize();
	};
	DevicePortaudio outputDevice(callback); // sync output device
	outputDevice.openDevice();
	getchar();
}

int main() {
	return asset();
	return sine();
	return pool();
}
