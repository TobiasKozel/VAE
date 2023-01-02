
#define VAE_IMPL
#include "../../include/vae/vae_pimpl.hpp"
#include "../core/vae_engine.hpp"
using namespace vae;
EnginePimpl* EnginePimpl::create() {
	auto e = new core::Engine();
	return reinterpret_cast<EnginePimpl*>(e);
}

void EnginePimpl::destroy() {
	auto e = reinterpret_cast<core::Engine*>(this);
	delete e;
}

Result EnginePimpl::init (
	const EngineConfig& config
) {
	auto& e = *reinterpret_cast<core::Engine*>(this);
	return e.init(
		config
	);
}

Result EnginePimpl::start () {
	auto& e = *reinterpret_cast<core::Engine*>(this);
	return e.start();
}

Result EnginePimpl::stop () {
	auto& e = *reinterpret_cast<core::Engine*>(this);
	return e.stop();
}

void EnginePimpl::update () {
	auto& e = *reinterpret_cast<core::Engine*>(this);
	return e.update();
}

Result EnginePimpl::fireEvent (
	BankHandle bankHandle,
	EventHandle eventHandle,
	EmitterHandle emitterHandle,
	Sample gain,
	MixerHandle mixerHandle,
	ListenerHandle listenerHandle
) {
	auto& e = *reinterpret_cast<core::Engine*>(this);
	return e.fireEvent(
		bankHandle,
		eventHandle,
		emitterHandle,
		gain,
		mixerHandle,
		listenerHandle
	);
}

Result EnginePimpl::fireGlobalEvent (
	GlobalEventHandle globalHandle,
	EmitterHandle emitterHandle,
	Sample gain,
	MixerHandle mixerHandle,
	ListenerHandle listenerHandle
) {
	auto& e = *reinterpret_cast<core::Engine*>(this);
	return e.fireGlobalEvent(
		globalHandle,
		emitterHandle,
		gain,
		mixerHandle,
		listenerHandle
	);
}

Size EnginePimpl::getActiveVoiceCount () {
	auto& e = *reinterpret_cast<core::Engine*>(this);
	return e.getActiveVoiceCount();
}

Size EnginePimpl::getInactiveVoiceCount () {
	auto& e = *reinterpret_cast<core::Engine*>(this);
	return e.getInactiveVoiceCount();
}

Size EnginePimpl::getStreamTime () {
	auto& e = *reinterpret_cast<core::Engine*>(this);
	return e.getStreamTime();
}

void EnginePimpl::setMasterVolume (
	Sample volume
) {
	auto& e = *reinterpret_cast<core::Engine*>(this);
	return e.setMasterVolume(
		volume
	);
}

bool EnginePimpl::checkVersion (
	int major,
	int minor,
	int patch
) {
	auto& e = *reinterpret_cast<core::Engine*>(this);
	return e.checkVersion(
		major,
		minor,
		patch
	);
}

EmitterHandle EnginePimpl::createEmitter () {
	auto& e = *reinterpret_cast<core::Engine*>(this);
	return e.createEmitter();
}

EmitterHandle EnginePimpl::createAutoEmitter (
	BankHandle bank,
	EventHandle event,
	float maxDist,
	const LocationDirection& locDir,
	float spread
) {
	auto& e = *reinterpret_cast<core::Engine*>(this);
	return e.createAutoEmitter(
		bank,
		event,
		maxDist,
		locDir,
		spread
	);
}

Result EnginePimpl::removeEmitter (
	EmitterHandle h
) {
	auto& e = *reinterpret_cast<core::Engine*>(this);
	return e.removeEmitter(
		h
	);
}

Result EnginePimpl::setEmitter (
	EmitterHandle emitter,
	const LocationDirection& locDir,
	float spread
) {
	auto& e = *reinterpret_cast<core::Engine*>(this);
	return e.setEmitter(
		emitter,
		locDir,
		spread
	);
}

Result EnginePimpl::stopEmitter (
	EmitterHandle emitter
) {
	auto& e = *reinterpret_cast<core::Engine*>(this);
	return e.stopEmitter(
		emitter
	);
}

void EnginePimpl::setVolume (
	EmitterHandle emitter,
	Sample gain
) {
	auto& e = *reinterpret_cast<core::Engine*>(this);
	return e.setVolume(
		emitter,
		gain
	);
}

void EnginePimpl::seek (
	EmitterHandle emitter,
	Size time
) {
	auto& e = *reinterpret_cast<core::Engine*>(this);
	return e.seek(
		emitter,
		time
	);
}

void EnginePimpl::setSpeed (
	EmitterHandle emitter,
	float speed
) {
	auto& e = *reinterpret_cast<core::Engine*>(this);
	return e.setSpeed(
		emitter,
		speed
	);
}

void EnginePimpl::setLowpass (
	EmitterHandle emitter,
	float cutoff
) {
	auto& e = *reinterpret_cast<core::Engine*>(this);
	return e.setLowpass(
		emitter,
		cutoff
	);
}

void EnginePimpl::setHighpass (
	EmitterHandle emitter,
	float cutoff
) {
	auto& e = *reinterpret_cast<core::Engine*>(this);
	return e.setHighpass(
		emitter,
		cutoff
	);
}

ListenerHandle EnginePimpl::createListener () {
	auto& e = *reinterpret_cast<core::Engine*>(this);
	return e.createListener();
}

Result EnginePimpl::removeListener (
	ListenerHandle listener
) {
	auto& e = *reinterpret_cast<core::Engine*>(this);
	return e.removeListener(
		listener
	);
}

Result EnginePimpl::setListener (
	ListenerHandle listener,
	const LocationOrientation& locOr
) {
	auto& e = *reinterpret_cast<core::Engine*>(this);
	return e.setListener(
		listener,
		locOr
	);
}

Result EnginePimpl::loadHRTF (
	CString path,
	Size size
) {
	auto& e = *reinterpret_cast<core::Engine*>(this);
	return e.loadHRTF(
		path,
		size
	);
}

Result EnginePimpl::loadBank (
	CString path,
	Size size
) {
	auto& e = *reinterpret_cast<core::Engine*>(this);
	return e.loadBank(
		path,
		size
	);
}

Result EnginePimpl::unloadBankFromId (
	BankHandle bankHandle
) {
	auto& e = *reinterpret_cast<core::Engine*>(this);
	return e.unloadBankFromId(
		bankHandle
	);
}

Result EnginePimpl::unloadBank (
	CString path
) {
	auto& e = *reinterpret_cast<core::Engine*>(this);
	return e.unloadBank(
		path
	);
}

void EnginePimpl::unloadAllBanks () {
	auto& e = *reinterpret_cast<core::Engine*>(this);
	return e.unloadAllBanks();
}


extern "C" EnginePimpl* create() {
	return EnginePimpl::create();
}
