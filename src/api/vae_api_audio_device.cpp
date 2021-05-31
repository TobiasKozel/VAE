#include "../../include/vae/vae_api_audio_device.hpp"
#include "../core/device/vae_portaudio.hpp"

using namespace vae;

bool PublicDevice::valid() {
	return true;
}

bool PublicDevice::openDevice(int output, int input) {

	auto& device = *reinterpret_cast<core::Device*>(this);
	return device.openDevice(output, input);
}

bool PublicDevice::openDevice(const DeviceInfo& deviceInfo) {
	auto& device = *reinterpret_cast<core::Device*>(this);
	return device.openDevice(deviceInfo);
}

bool PublicDevice::closeDevice() {
	auto& device = *reinterpret_cast<core::Device*>(this);
	return device.closeDevice();
}

int PublicDevice::getChannelsOut() {
	auto& device = *reinterpret_cast<core::Device*>(this);
	return device.getChannelsOut();
}

int PublicDevice::getChannelsIn() {
	auto& device = *reinterpret_cast<core::Device*>(this);
	return device.getChannelsIn();
}

int PublicDevice::getStreamTime() {
	auto& device = *reinterpret_cast<core::Device*>(this);
	return device.getStreamTime();
}


