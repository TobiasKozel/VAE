#include "../../include/vae/vae_api_audio_device.hpp"
#include "../core/device/vae_portaudio.hpp"

using namespace vae;

bool PublicDevice::valid() {
	return true;
}

bool PublicDevice::openDevice(int output, int input) {

	auto& device = *reinterpret_cast<core::Device*>(this);
	device.openDevice(output, input);
}

bool PublicDevice::openDevice(const DeviceInfo& device) {

}

bool PublicDevice::closeDevice() {

}

int PublicDevice::getChannelsOut() {

}

int PublicDevice::getChannelsIn() {

}

int PublicDevice::getStreamTime() {

}


