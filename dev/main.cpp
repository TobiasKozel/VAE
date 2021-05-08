#include <stdio.h>

#include "core/device/vae_portaudio.hpp"

using namespace vae;
using namespace core;

int main() {
	Device::SyncCallback callback =
	[&](const Device::AudioBuffer& fromDevice, Device::AudioBuffer& toDevice) {
		int i = 0;
		toDevice.set(fromDevice);
	};
	DeviceInfo devices[100];
	auto& backend = BackendPortAudio::instance();
	for (int i = 0; i < backend.getDeviceCount(); i++) {
		devices[i] = backend.getDevice(i);
	}
	DevicePortaudio device(callback);
	device.openDevice(devices[0]);
	getchar();
	return 0;
}
