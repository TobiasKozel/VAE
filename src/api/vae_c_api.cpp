#include "../core/vae_engine.hpp"
// #include "../../../src/core/vae_engine.hpp"

#if defined(_WIN32) || defined(__CYGWIN__)
	#define _VAE_API_EXPORT __declspec(dllexport)
#else
	#define _VAE_API_EXPORT __attribute__((visibility("default")))
#endif

using namespace vae;
using namespace core;

extern "C"
{
	////////////// EventCallbackData
	_VAE_API_EXPORT Pointer vae_create_EventCallbackData() {
		return new EventCallbackData();
	}

	_VAE_API_EXPORT void vae_destroy_EventCallbackData(Pointer obj) {
		delete reinterpret_cast<EventCallbackData*>(obj);
	}

		///// EventCallbackData.context
		_VAE_API_EXPORT void vae_EventCallbackData_set_context(Pointer obj, Pointer value) {
			reinterpret_cast<EventCallbackData*>(obj)->context = value;
		}
		_VAE_API_EXPORT Pointer vae_EventCallbackData_get_context(Pointer obj) {
			return reinterpret_cast<EventCallbackData*>(obj)->context;
		}
		///// EventCallbackData.bank
		_VAE_API_EXPORT void vae_EventCallbackData_set_bank(Pointer obj, BankHandle value) {
			reinterpret_cast<EventCallbackData*>(obj)->bank = value;
		}
		_VAE_API_EXPORT BankHandle vae_EventCallbackData_get_bank(Pointer obj) {
			return reinterpret_cast<EventCallbackData*>(obj)->bank;
		}
		///// EventCallbackData.event
		_VAE_API_EXPORT void vae_EventCallbackData_set_event(Pointer obj, EventHandle value) {
			reinterpret_cast<EventCallbackData*>(obj)->event = value;
		}
		_VAE_API_EXPORT EventHandle vae_EventCallbackData_get_event(Pointer obj) {
			return reinterpret_cast<EventCallbackData*>(obj)->event;
		}
		///// EventCallbackData.emitter
		_VAE_API_EXPORT void vae_EventCallbackData_set_emitter(Pointer obj, EmitterHandle value) {
			reinterpret_cast<EventCallbackData*>(obj)->emitter = value;
		}
		_VAE_API_EXPORT EmitterHandle vae_EventCallbackData_get_emitter(Pointer obj) {
			return reinterpret_cast<EventCallbackData*>(obj)->emitter;
		}

	////////////// EngineConfig
	_VAE_API_EXPORT Pointer vae_create_EngineConfig() {
		return new EngineConfig();
	}

	_VAE_API_EXPORT void vae_destroy_EngineConfig(Pointer obj) {
		delete reinterpret_cast<EngineConfig*>(obj);
	}

		///// EngineConfig.rootPath
		_VAE_API_EXPORT void vae_EngineConfig_set_rootPath(Pointer obj, CString value) {
			reinterpret_cast<EngineConfig*>(obj)->rootPath = value;
		}
		_VAE_API_EXPORT CString vae_EngineConfig_get_rootPath(Pointer obj) {
			return reinterpret_cast<EngineConfig*>(obj)->rootPath;
		}
		///// EngineConfig.internalSampleRate
		_VAE_API_EXPORT void vae_EngineConfig_set_internalSampleRate(Pointer obj, Size value) {
			reinterpret_cast<EngineConfig*>(obj)->internalSampleRate = value;
		}
		_VAE_API_EXPORT Size vae_EngineConfig_get_internalSampleRate(Pointer obj) {
			return reinterpret_cast<EngineConfig*>(obj)->internalSampleRate;
		}
		///// EngineConfig.eventCallbackContext
		_VAE_API_EXPORT void vae_EngineConfig_set_eventCallbackContext(Pointer obj, Pointer value) {
			reinterpret_cast<EngineConfig*>(obj)->eventCallbackContext = value;
		}
		_VAE_API_EXPORT Pointer vae_EngineConfig_get_eventCallbackContext(Pointer obj) {
			return reinterpret_cast<EngineConfig*>(obj)->eventCallbackContext;
		}
		///// EngineConfig.preAllocatedEmitters
		_VAE_API_EXPORT void vae_EngineConfig_set_preAllocatedEmitters(Pointer obj, Size value) {
			reinterpret_cast<EngineConfig*>(obj)->preAllocatedEmitters = value;
		}
		_VAE_API_EXPORT Size vae_EngineConfig_get_preAllocatedEmitters(Pointer obj) {
			return reinterpret_cast<EngineConfig*>(obj)->preAllocatedEmitters;
		}
		///// EngineConfig.voices
		_VAE_API_EXPORT void vae_EngineConfig_set_voices(Pointer obj, Size value) {
			reinterpret_cast<EngineConfig*>(obj)->voices = value;
		}
		_VAE_API_EXPORT Size vae_EngineConfig_get_voices(Pointer obj) {
			return reinterpret_cast<EngineConfig*>(obj)->voices;
		}
		///// EngineConfig.hrtfVoices
		_VAE_API_EXPORT void vae_EngineConfig_set_hrtfVoices(Pointer obj, Size value) {
			reinterpret_cast<EngineConfig*>(obj)->hrtfVoices = value;
		}
		_VAE_API_EXPORT Size vae_EngineConfig_get_hrtfVoices(Pointer obj) {
			return reinterpret_cast<EngineConfig*>(obj)->hrtfVoices;
		}
		///// EngineConfig.virtualVoices
		_VAE_API_EXPORT void vae_EngineConfig_set_virtualVoices(Pointer obj, Size value) {
			reinterpret_cast<EngineConfig*>(obj)->virtualVoices = value;
		}
		_VAE_API_EXPORT Size vae_EngineConfig_get_virtualVoices(Pointer obj) {
			return reinterpret_cast<EngineConfig*>(obj)->virtualVoices;
		}
		///// EngineConfig.finishedVoiceQueueSize
		_VAE_API_EXPORT void vae_EngineConfig_set_finishedVoiceQueueSize(Pointer obj, Size value) {
			reinterpret_cast<EngineConfig*>(obj)->finishedVoiceQueueSize = value;
		}
		_VAE_API_EXPORT Size vae_EngineConfig_get_finishedVoiceQueueSize(Pointer obj) {
			return reinterpret_cast<EngineConfig*>(obj)->finishedVoiceQueueSize;
		}
		///// EngineConfig.preferredBufferSize
		_VAE_API_EXPORT void vae_EngineConfig_set_preferredBufferSize(Pointer obj, Size value) {
			reinterpret_cast<EngineConfig*>(obj)->preferredBufferSize = value;
		}
		_VAE_API_EXPORT Size vae_EngineConfig_get_preferredBufferSize(Pointer obj) {
			return reinterpret_cast<EngineConfig*>(obj)->preferredBufferSize;
		}
		///// EngineConfig.bufferPeriods
		_VAE_API_EXPORT void vae_EngineConfig_set_bufferPeriods(Pointer obj, Size value) {
			reinterpret_cast<EngineConfig*>(obj)->bufferPeriods = value;
		}
		_VAE_API_EXPORT Size vae_EngineConfig_get_bufferPeriods(Pointer obj) {
			return reinterpret_cast<EngineConfig*>(obj)->bufferPeriods;
		}
		///// EngineConfig.updateInAudioThread
		_VAE_API_EXPORT void vae_EngineConfig_set_updateInAudioThread(Pointer obj, Pointer value) {
			reinterpret_cast<EngineConfig*>(obj)->updateInAudioThread =
				*reinterpret_cast<bool*>(value);
		}
		_VAE_API_EXPORT Pointer vae_EngineConfig_get_updateInAudioThread(Pointer obj) {
			return &reinterpret_cast<EngineConfig*>(obj)->updateInAudioThread;
		}
		///// EngineConfig.processInBufferSwitch
		_VAE_API_EXPORT void vae_EngineConfig_set_processInBufferSwitch(Pointer obj, Pointer value) {
			reinterpret_cast<EngineConfig*>(obj)->processInBufferSwitch =
				*reinterpret_cast<bool*>(value);
		}
		_VAE_API_EXPORT Pointer vae_EngineConfig_get_processInBufferSwitch(Pointer obj) {
			return &reinterpret_cast<EngineConfig*>(obj)->processInBufferSwitch;
		}

	////////////// DeviceInfo
	_VAE_API_EXPORT Pointer vae_create_DeviceInfo() {
		return new DeviceInfo();
	}

	_VAE_API_EXPORT void vae_destroy_DeviceInfo(Pointer obj) {
		delete reinterpret_cast<DeviceInfo*>(obj);
	}

		///// DeviceInfo.id
		_VAE_API_EXPORT void vae_DeviceInfo_set_id(Pointer obj, Pointer value) {
			reinterpret_cast<DeviceInfo*>(obj)->id =
				*reinterpret_cast<int*>(value);
		}
		_VAE_API_EXPORT Pointer vae_DeviceInfo_get_id(Pointer obj) {
			return &reinterpret_cast<DeviceInfo*>(obj)->id;
		}
		///// DeviceInfo.sampleRate
		_VAE_API_EXPORT void vae_DeviceInfo_set_sampleRate(Pointer obj, Size value) {
			reinterpret_cast<DeviceInfo*>(obj)->sampleRate = value;
		}
		_VAE_API_EXPORT Size vae_DeviceInfo_get_sampleRate(Pointer obj) {
			return reinterpret_cast<DeviceInfo*>(obj)->sampleRate;
		}
		///// DeviceInfo.bufferSize
		_VAE_API_EXPORT void vae_DeviceInfo_set_bufferSize(Pointer obj, Size value) {
			reinterpret_cast<DeviceInfo*>(obj)->bufferSize = value;
		}
		_VAE_API_EXPORT Size vae_DeviceInfo_get_bufferSize(Pointer obj) {
			return reinterpret_cast<DeviceInfo*>(obj)->bufferSize;
		}
		///// DeviceInfo.channelsIn
		_VAE_API_EXPORT void vae_DeviceInfo_set_channelsIn(Pointer obj, Pointer value) {
			reinterpret_cast<DeviceInfo*>(obj)->channelsIn =
				*reinterpret_cast<unsigned char*>(value);
		}
		_VAE_API_EXPORT Pointer vae_DeviceInfo_get_channelsIn(Pointer obj) {
			return &reinterpret_cast<DeviceInfo*>(obj)->channelsIn;
		}
		///// DeviceInfo.channelsOut
		_VAE_API_EXPORT void vae_DeviceInfo_set_channelsOut(Pointer obj, Pointer value) {
			reinterpret_cast<DeviceInfo*>(obj)->channelsOut =
				*reinterpret_cast<unsigned char*>(value);
		}
		_VAE_API_EXPORT Pointer vae_DeviceInfo_get_channelsOut(Pointer obj) {
			return &reinterpret_cast<DeviceInfo*>(obj)->channelsOut;
		}

	////////////// Vector3
	_VAE_API_EXPORT Pointer vae_create_Vector3() {
		return new Vector3();
	}

	_VAE_API_EXPORT void vae_destroy_Vector3(Pointer obj) {
		delete reinterpret_cast<Vector3*>(obj);
	}

		///// Vector3.x
		_VAE_API_EXPORT void vae_Vector3_set_x(Pointer obj, Pointer value) {
			reinterpret_cast<Vector3*>(obj)->x =
				*reinterpret_cast<float*>(value);
		}
		_VAE_API_EXPORT Pointer vae_Vector3_get_x(Pointer obj) {
			return &reinterpret_cast<Vector3*>(obj)->x;
		}
		///// Vector3.y
		_VAE_API_EXPORT void vae_Vector3_set_y(Pointer obj, Pointer value) {
			reinterpret_cast<Vector3*>(obj)->y =
				*reinterpret_cast<float*>(value);
		}
		_VAE_API_EXPORT Pointer vae_Vector3_get_y(Pointer obj) {
			return &reinterpret_cast<Vector3*>(obj)->y;
		}
		///// Vector3.z
		_VAE_API_EXPORT void vae_Vector3_set_z(Pointer obj, Pointer value) {
			reinterpret_cast<Vector3*>(obj)->z =
				*reinterpret_cast<float*>(value);
		}
		_VAE_API_EXPORT Pointer vae_Vector3_get_z(Pointer obj) {
			return &reinterpret_cast<Vector3*>(obj)->z;
		}

	////////////// LocationDirection
	_VAE_API_EXPORT Pointer vae_create_LocationDirection() {
		return new LocationDirection();
	}

	_VAE_API_EXPORT void vae_destroy_LocationDirection(Pointer obj) {
		delete reinterpret_cast<LocationDirection*>(obj);
	}

		///// LocationDirection.position
		_VAE_API_EXPORT void vae_LocationDirection_set_position(Pointer obj, Pointer value) {
			reinterpret_cast<LocationDirection*>(obj)->position =
				*reinterpret_cast<Vector3*>(value);
		}
		_VAE_API_EXPORT Pointer vae_LocationDirection_get_position(Pointer obj) {
			return &reinterpret_cast<LocationDirection*>(obj)->position;
		}
		///// LocationDirection.direction
		_VAE_API_EXPORT void vae_LocationDirection_set_direction(Pointer obj, Pointer value) {
			reinterpret_cast<LocationDirection*>(obj)->direction =
				*reinterpret_cast<Vector3*>(value);
		}
		_VAE_API_EXPORT Pointer vae_LocationDirection_get_direction(Pointer obj) {
			return &reinterpret_cast<LocationDirection*>(obj)->direction;
		}

	////////////// LocationOrientation
	_VAE_API_EXPORT Pointer vae_create_LocationOrientation() {
		return new LocationOrientation();
	}

	_VAE_API_EXPORT void vae_destroy_LocationOrientation(Pointer obj) {
		delete reinterpret_cast<LocationOrientation*>(obj);
	}


	////////////// Engine
	_VAE_API_EXPORT Pointer vae_create_Engine() {
		return new Engine();
	}

	_VAE_API_EXPORT void vae_destroy_Engine(Pointer obj) {
		delete reinterpret_cast<Engine*>(obj);
	}


		///// Engine.init
		_VAE_API_EXPORT Result vae_Engine_init(
			Pointer obj,
			Pointer config

		) {
			return reinterpret_cast<Engine*>(obj)->init(
				*reinterpret_cast<EngineConfig*>(config)
			);
		}

		///// Engine.start
		_VAE_API_EXPORT Result vae_Engine_start(
			Pointer obj
		) {
			return reinterpret_cast<Engine*>(obj)->start(
			);
		}

		///// Engine.stop
		_VAE_API_EXPORT Result vae_Engine_stop(
			Pointer obj
		) {
			return reinterpret_cast<Engine*>(obj)->stop(
			);
		}

		///// Engine.update
		_VAE_API_EXPORT void vae_Engine_update(
			Pointer obj
		) {
			return reinterpret_cast<Engine*>(obj)->update(
			);
		}

		///// Engine.fireEvent
		_VAE_API_EXPORT Result vae_Engine_fireEvent(
			Pointer obj,
			BankHandle bankHandle,
			EventHandle eventHandle,
			EmitterHandle emitterHandle,
			Sample gain,
			MixerHandle mixerHandle,
			ListenerHandle listenerHandle
		) {
			return reinterpret_cast<Engine*>(obj)->fireEvent(
				bankHandle,
				eventHandle,
				emitterHandle,
				gain,
				mixerHandle,
				listenerHandle
			);
		}

		///// Engine.fireGlobalEvent
		_VAE_API_EXPORT Result vae_Engine_fireGlobalEvent(
			Pointer obj,
			GlobalEventHandle globalHandle,
			EmitterHandle emitterHandle,
			Sample gain,
			MixerHandle mixerHandle,
			ListenerHandle listenerHandle
		) {
			return reinterpret_cast<Engine*>(obj)->fireGlobalEvent(
				globalHandle,
				emitterHandle,
				gain,
				mixerHandle,
				listenerHandle
			);
		}

		///// Engine.getActiveVoiceCount
		_VAE_API_EXPORT Size vae_Engine_getActiveVoiceCount(
			Pointer obj
		) {
			return reinterpret_cast<Engine*>(obj)->getActiveVoiceCount(
			);
		}

		///// Engine.getStreamTime
		_VAE_API_EXPORT Size vae_Engine_getStreamTime(
			Pointer obj
		) {
			return reinterpret_cast<Engine*>(obj)->getStreamTime(
			);
		}

		///// Engine.setMasterVolume
		_VAE_API_EXPORT void vae_Engine_setMasterVolume(
			Pointer obj,
			Sample volume
		) {
			return reinterpret_cast<Engine*>(obj)->setMasterVolume(
				volume
			);
		}

		///// Engine.checkVersion
		_VAE_API_EXPORT bool vae_Engine_checkVersion(
			Pointer obj,
			Pointer major
,
			Pointer minor
,
			Pointer patch

		) {
			return reinterpret_cast<Engine*>(obj)->checkVersion(
				*reinterpret_cast<int*>(major),
				*reinterpret_cast<int*>(minor),
				*reinterpret_cast<int*>(patch)
			);
		}

		///// Engine.createEmitter
		_VAE_API_EXPORT EmitterHandle vae_Engine_createEmitter(
			Pointer obj
		) {
			return reinterpret_cast<Engine*>(obj)->createEmitter(
			);
		}

		///// Engine.createAutoEmitter
		_VAE_API_EXPORT EmitterHandle vae_Engine_createAutoEmitter(
			Pointer obj,
			BankHandle bank,
			EventHandle event,
			Pointer maxDist
,
			Pointer locDir
,
			Pointer spread

		) {
			return reinterpret_cast<Engine*>(obj)->createAutoEmitter(
				bank,
				event,
				*reinterpret_cast<float*>(maxDist),
				*reinterpret_cast<LocationDirection*>(locDir),
				*reinterpret_cast<float*>(spread)
			);
		}

		///// Engine.addEmitter
		_VAE_API_EXPORT Result vae_Engine_addEmitter(
			Pointer obj,
			EmitterHandle h
		) {
			return reinterpret_cast<Engine*>(obj)->addEmitter(
				h
			);
		}

		///// Engine.removeEmitter
		_VAE_API_EXPORT Result vae_Engine_removeEmitter(
			Pointer obj,
			EmitterHandle h
		) {
			return reinterpret_cast<Engine*>(obj)->removeEmitter(
				h
			);
		}

		///// Engine.setEmitter
		_VAE_API_EXPORT Result vae_Engine_setEmitter(
			Pointer obj,
			EmitterHandle emitter,
			Pointer locDir
,
			Pointer spread

		) {
			return reinterpret_cast<Engine*>(obj)->setEmitter(
				emitter,
				*reinterpret_cast<LocationDirection*>(locDir),
				*reinterpret_cast<float*>(spread)
			);
		}

		///// Engine.stopEmitter
		_VAE_API_EXPORT Result vae_Engine_stopEmitter(
			Pointer obj,
			EmitterHandle emitter
		) {
			return reinterpret_cast<Engine*>(obj)->stopEmitter(
				emitter
			);
		}

		///// Engine.setVolume
		_VAE_API_EXPORT void vae_Engine_setVolume(
			Pointer obj,
			EmitterHandle emitter,
			Sample gain
		) {
			return reinterpret_cast<Engine*>(obj)->setVolume(
				emitter,
				gain
			);
		}

		///// Engine.seek
		_VAE_API_EXPORT void vae_Engine_seek(
			Pointer obj,
			EmitterHandle emitter,
			Size time
		) {
			return reinterpret_cast<Engine*>(obj)->seek(
				emitter,
				time
			);
		}

		///// Engine.setSpeed
		_VAE_API_EXPORT void vae_Engine_setSpeed(
			Pointer obj,
			EmitterHandle emitter,
			Pointer speed

		) {
			return reinterpret_cast<Engine*>(obj)->setSpeed(
				emitter,
				*reinterpret_cast<float*>(speed)
			);
		}

		///// Engine.setLowpass
		_VAE_API_EXPORT void vae_Engine_setLowpass(
			Pointer obj,
			EmitterHandle emitter,
			Pointer cutoff

		) {
			return reinterpret_cast<Engine*>(obj)->setLowpass(
				emitter,
				*reinterpret_cast<float*>(cutoff)
			);
		}

		///// Engine.setHighpass
		_VAE_API_EXPORT void vae_Engine_setHighpass(
			Pointer obj,
			EmitterHandle emitter,
			Pointer cutoff

		) {
			return reinterpret_cast<Engine*>(obj)->setHighpass(
				emitter,
				*reinterpret_cast<float*>(cutoff)
			);
		}

		///// Engine.createListener
		_VAE_API_EXPORT ListenerHandle vae_Engine_createListener(
			Pointer obj
		) {
			return reinterpret_cast<Engine*>(obj)->createListener(
			);
		}

		///// Engine.removeListener
		_VAE_API_EXPORT Result vae_Engine_removeListener(
			Pointer obj,
			ListenerHandle listener
		) {
			return reinterpret_cast<Engine*>(obj)->removeListener(
				listener
			);
		}

		///// Engine.setListener
		_VAE_API_EXPORT Result vae_Engine_setListener(
			Pointer obj,
			ListenerHandle listener,
			Pointer locOr

		) {
			return reinterpret_cast<Engine*>(obj)->setListener(
				listener,
				*reinterpret_cast<LocationOrientation*>(locOr)
			);
		}

		///// Engine.loadHRTF
		_VAE_API_EXPORT Result vae_Engine_loadHRTF(
			Pointer obj,
			CString path,
			Size size
		) {
			return reinterpret_cast<Engine*>(obj)->loadHRTF(
				path,
				size
			);
		}

		///// Engine.loadBank
		_VAE_API_EXPORT Result vae_Engine_loadBank(
			Pointer obj,
			CString path,
			Size size
		) {
			return reinterpret_cast<Engine*>(obj)->loadBank(
				path,
				size
			);
		}

		///// Engine.unloadBankFromId
		_VAE_API_EXPORT Result vae_Engine_unloadBankFromId(
			Pointer obj,
			BankHandle bankHandle
		) {
			return reinterpret_cast<Engine*>(obj)->unloadBankFromId(
				bankHandle
			);
		}

		///// Engine.unloadAllBanks
		_VAE_API_EXPORT void vae_Engine_unloadAllBanks(
			Pointer obj
		) {
			return reinterpret_cast<Engine*>(obj)->unloadAllBanks(
			);
		}


} // extern "C"