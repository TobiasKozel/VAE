#include <stdio.h>
#define TKLB_ASSERT_SEGFAULT
#include "core/device/vae_portaudio.hpp"

using namespace vae;
using namespace core;

int main() {
	DeviceInfo devices[100];
	auto& backend = BackendPortAudio::instance();
	for (int i = 0; i < backend.getDeviceCount(); i++) {
		devices[i] = backend.getDevice(i);
	}

	DevicePortaudio inputDevice; // async input device
	inputDevice.openDevice(devices[5]);

	Device::SyncCallback callback =
	[&](const Device::AudioBuffer& fromDevice, Device::AudioBuffer& toDevice) {
		inputDevice.pop(toDevice);
	};
	DevicePortaudio outputDevice(callback); // sync output device
	outputDevice.openDevice(devices[7]);
	getchar();
	return 0;
}
