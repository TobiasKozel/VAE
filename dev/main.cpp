#include <stdio.h>
#define TKLB_ASSERT_SEGFAULT
#include "core/device/vae_portaudio.hpp"

using namespace vae;
using namespace core;

int main() {
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
	return 0;
}
