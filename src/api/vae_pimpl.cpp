
#include "../../include/vae/vae_pimpl.hpp"
#include "../core/vae_engine.hpp"
using namespace vae;
EnginePimpl* EnginePimpl::create() {
	auto e = new core::Engine();
	return reinterpret_cast<EnginePimpl*>(e);
}

EnginePimpl* EnginePimpl::create(const EngineConfig& config) {
	auto e = new core::Engine(config);
	return reinterpret_cast<EnginePimpl*>(e);
}

void EnginePimpl::destroy() {
	auto e = reinterpret_cast<core::Engine*>(this);
	delete e;
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
	BankHandle bank,
	EventHandle eventHandle,
	EmitterHandle emitterHandle,
	MixerHandle mixerHandle
) {
	auto& e = *reinterpret_cast<core::Engine*>(this);
	return e.fireEvent(
		bank,
		eventHandle,
		emitterHandle,
		mixerHandle
	);
}

Result EnginePimpl::fireGlobalEvent (
	GlobalEventHandle globalHandle,
	EmitterHandle emitterHandle,
	MixerHandle mixerHandle
) {
	auto& e = *reinterpret_cast<core::Engine*>(this);
	return e.fireGlobalEvent(
		globalHandle,
		emitterHandle,
		mixerHandle
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

Result EnginePimpl::addEmitter (
	EmitterHandle h
) {
	auto& e = *reinterpret_cast<core::Engine*>(this);
	return e.addEmitter(
		h
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
	const char* path
) {
	auto& e = *reinterpret_cast<core::Engine*>(this);
	return e.loadHRTF(
		path
	);
}

Result EnginePimpl::loadBank (
	const char* path
) {
	auto& e = *reinterpret_cast<core::Engine*>(this);
	return e.loadBank(
		path
	);
}

Result EnginePimpl::unloadBankFromPath (
	const char* path
) {
	auto& e = *reinterpret_cast<core::Engine*>(this);
	return e.unloadBankFromPath(
		path
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

void EnginePimpl::unloadAllBanks () {
	auto& e = *reinterpret_cast<core::Engine*>(this);
	return e.unloadAllBanks();
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

