
#include "../core/vae_engine.hpp"

#if defined(_WIN32) || defined(__CYGWIN__)
	#define _VAE_API_EXPORT __declspec(dllexport)
#else
	#define _VAE_API_EXPORT __attribute__((visibility("default")))
#endif

extern "C"
{

	_VAE_API_EXPORT void* vae_create_EventCallbackData() {
		return new vae::EventCallbackData();
	}

	_VAE_API_EXPORT void vae_destroy_EventCallbackData(void* obj) {
		delete reinterpret_cast<vae::EventCallbackData*>(obj);
	}
	
	_VAE_API_EXPORT void vae_EventCallbackData_set_context(void* obj, void* value) {
		reinterpret_cast<vae::EventCallbackData*>(obj)->context = value;
	}

	_VAE_API_EXPORT void* vae_EventCallbackData_get_context(void* obj) {
		return reinterpret_cast<vae::EventCallbackData*>(obj)->context;
	}
		
	_VAE_API_EXPORT void vae_EventCallbackData_set_bank(void* obj, unsigned char value) {
		reinterpret_cast<vae::EventCallbackData*>(obj)->bank = value;
	}

	_VAE_API_EXPORT unsigned char vae_EventCallbackData_get_bank(void* obj) {
		return reinterpret_cast<vae::EventCallbackData*>(obj)->bank;
	}
		
	_VAE_API_EXPORT void vae_EventCallbackData_set_event(void* obj, unsigned short value) {
		reinterpret_cast<vae::EventCallbackData*>(obj)->event = value;
	}

	_VAE_API_EXPORT unsigned short vae_EventCallbackData_get_event(void* obj) {
		return reinterpret_cast<vae::EventCallbackData*>(obj)->event;
	}
		
	_VAE_API_EXPORT void vae_EventCallbackData_set_emitter(void* obj, unsigned int value) {
		reinterpret_cast<vae::EventCallbackData*>(obj)->emitter = value;
	}

	_VAE_API_EXPORT unsigned int vae_EventCallbackData_get_emitter(void* obj) {
		return reinterpret_cast<vae::EventCallbackData*>(obj)->emitter;
	}
		
	_VAE_API_EXPORT void* vae_create_EngineConfig() {
		return new vae::EngineConfig();
	}

	_VAE_API_EXPORT void vae_destroy_EngineConfig(void* obj) {
		delete reinterpret_cast<vae::EngineConfig*>(obj);
	}
	
	_VAE_API_EXPORT void vae_EngineConfig_set_rootPath(void* obj, const char* value) {
		reinterpret_cast<vae::EngineConfig*>(obj)->rootPath = value;
	}

	_VAE_API_EXPORT const char* vae_EngineConfig_get_rootPath(void* obj) {
		return reinterpret_cast<vae::EngineConfig*>(obj)->rootPath;
	}
		
	_VAE_API_EXPORT void vae_EngineConfig_set_internalSampleRate(void* obj, unsigned int value) {
		reinterpret_cast<vae::EngineConfig*>(obj)->internalSampleRate = value;
	}

	_VAE_API_EXPORT unsigned int vae_EngineConfig_get_internalSampleRate(void* obj) {
		return reinterpret_cast<vae::EngineConfig*>(obj)->internalSampleRate;
	}
		
	_VAE_API_EXPORT void vae_EngineConfig_set_eventCallbackContext(void* obj, void* value) {
		reinterpret_cast<vae::EngineConfig*>(obj)->eventCallbackContext = value;
	}

	_VAE_API_EXPORT void* vae_EngineConfig_get_eventCallbackContext(void* obj) {
		return reinterpret_cast<vae::EngineConfig*>(obj)->eventCallbackContext;
	}
		
	_VAE_API_EXPORT void vae_EngineConfig_set_preAllocatedEmitters(void* obj, unsigned int value) {
		reinterpret_cast<vae::EngineConfig*>(obj)->preAllocatedEmitters = value;
	}

	_VAE_API_EXPORT unsigned int vae_EngineConfig_get_preAllocatedEmitters(void* obj) {
		return reinterpret_cast<vae::EngineConfig*>(obj)->preAllocatedEmitters;
	}
		
	_VAE_API_EXPORT void vae_EngineConfig_set_voices(void* obj, unsigned int value) {
		reinterpret_cast<vae::EngineConfig*>(obj)->voices = value;
	}

	_VAE_API_EXPORT unsigned int vae_EngineConfig_get_voices(void* obj) {
		return reinterpret_cast<vae::EngineConfig*>(obj)->voices;
	}
		
	_VAE_API_EXPORT void vae_EngineConfig_set_hrtfVoices(void* obj, unsigned int value) {
		reinterpret_cast<vae::EngineConfig*>(obj)->hrtfVoices = value;
	}

	_VAE_API_EXPORT unsigned int vae_EngineConfig_get_hrtfVoices(void* obj) {
		return reinterpret_cast<vae::EngineConfig*>(obj)->hrtfVoices;
	}
		
	_VAE_API_EXPORT void vae_EngineConfig_set_virtualVoices(void* obj, unsigned int value) {
		reinterpret_cast<vae::EngineConfig*>(obj)->virtualVoices = value;
	}

	_VAE_API_EXPORT unsigned int vae_EngineConfig_get_virtualVoices(void* obj) {
		return reinterpret_cast<vae::EngineConfig*>(obj)->virtualVoices;
	}
		
	_VAE_API_EXPORT void vae_EngineConfig_set_finishedVoiceQueueSize(void* obj, unsigned int value) {
		reinterpret_cast<vae::EngineConfig*>(obj)->finishedVoiceQueueSize = value;
	}

	_VAE_API_EXPORT unsigned int vae_EngineConfig_get_finishedVoiceQueueSize(void* obj) {
		return reinterpret_cast<vae::EngineConfig*>(obj)->finishedVoiceQueueSize;
	}
		
	_VAE_API_EXPORT void vae_EngineConfig_set_preferredBufferSize(void* obj, unsigned int value) {
		reinterpret_cast<vae::EngineConfig*>(obj)->preferredBufferSize = value;
	}

	_VAE_API_EXPORT unsigned int vae_EngineConfig_get_preferredBufferSize(void* obj) {
		return reinterpret_cast<vae::EngineConfig*>(obj)->preferredBufferSize;
	}
		
	_VAE_API_EXPORT void vae_EngineConfig_set_bufferPeriods(void* obj, unsigned int value) {
		reinterpret_cast<vae::EngineConfig*>(obj)->bufferPeriods = value;
	}

	_VAE_API_EXPORT unsigned int vae_EngineConfig_get_bufferPeriods(void* obj) {
		return reinterpret_cast<vae::EngineConfig*>(obj)->bufferPeriods;
	}
		
	_VAE_API_EXPORT void vae_EngineConfig_set_updateInAudioThread(void* obj, bool value) {
		reinterpret_cast<vae::EngineConfig*>(obj)->updateInAudioThread = value;
	}

	_VAE_API_EXPORT bool vae_EngineConfig_get_updateInAudioThread(void* obj) {
		return reinterpret_cast<vae::EngineConfig*>(obj)->updateInAudioThread;
	}
		
	_VAE_API_EXPORT void vae_EngineConfig_set_processInBufferSwitch(void* obj, bool value) {
		reinterpret_cast<vae::EngineConfig*>(obj)->processInBufferSwitch = value;
	}

	_VAE_API_EXPORT bool vae_EngineConfig_get_processInBufferSwitch(void* obj) {
		return reinterpret_cast<vae::EngineConfig*>(obj)->processInBufferSwitch;
	}
		
	_VAE_API_EXPORT void* vae_create_DeviceInfo() {
		return new vae::DeviceInfo();
	}

	_VAE_API_EXPORT void vae_destroy_DeviceInfo(void* obj) {
		delete reinterpret_cast<vae::DeviceInfo*>(obj);
	}
	
	_VAE_API_EXPORT void vae_DeviceInfo_set_id(void* obj, int value) {
		reinterpret_cast<vae::DeviceInfo*>(obj)->id = value;
	}

	_VAE_API_EXPORT int vae_DeviceInfo_get_id(void* obj) {
		return reinterpret_cast<vae::DeviceInfo*>(obj)->id;
	}
		
	_VAE_API_EXPORT void vae_DeviceInfo_set_sampleRate(void* obj, unsigned int value) {
		reinterpret_cast<vae::DeviceInfo*>(obj)->sampleRate = value;
	}

	_VAE_API_EXPORT unsigned int vae_DeviceInfo_get_sampleRate(void* obj) {
		return reinterpret_cast<vae::DeviceInfo*>(obj)->sampleRate;
	}
		
	_VAE_API_EXPORT void vae_DeviceInfo_set_bufferSize(void* obj, unsigned int value) {
		reinterpret_cast<vae::DeviceInfo*>(obj)->bufferSize = value;
	}

	_VAE_API_EXPORT unsigned int vae_DeviceInfo_get_bufferSize(void* obj) {
		return reinterpret_cast<vae::DeviceInfo*>(obj)->bufferSize;
	}
		
	_VAE_API_EXPORT void vae_DeviceInfo_set_channelsIn(void* obj, unsigned char value) {
		reinterpret_cast<vae::DeviceInfo*>(obj)->channelsIn = value;
	}

	_VAE_API_EXPORT unsigned char vae_DeviceInfo_get_channelsIn(void* obj) {
		return reinterpret_cast<vae::DeviceInfo*>(obj)->channelsIn;
	}
		
	_VAE_API_EXPORT void vae_DeviceInfo_set_channelsOut(void* obj, unsigned char value) {
		reinterpret_cast<vae::DeviceInfo*>(obj)->channelsOut = value;
	}

	_VAE_API_EXPORT unsigned char vae_DeviceInfo_get_channelsOut(void* obj) {
		return reinterpret_cast<vae::DeviceInfo*>(obj)->channelsOut;
	}
		
	_VAE_API_EXPORT void* vae_create_Vector3() {
		return new vae::Vector3();
	}

	_VAE_API_EXPORT void vae_destroy_Vector3(void* obj) {
		delete reinterpret_cast<vae::Vector3*>(obj);
	}
	
	_VAE_API_EXPORT void vae_Vector3_set_x(void* obj, float value) {
		reinterpret_cast<vae::Vector3*>(obj)->x = value;
	}

	_VAE_API_EXPORT float vae_Vector3_get_x(void* obj) {
		return reinterpret_cast<vae::Vector3*>(obj)->x;
	}
		
	_VAE_API_EXPORT void vae_Vector3_set_y(void* obj, float value) {
		reinterpret_cast<vae::Vector3*>(obj)->y = value;
	}

	_VAE_API_EXPORT float vae_Vector3_get_y(void* obj) {
		return reinterpret_cast<vae::Vector3*>(obj)->y;
	}
		
	_VAE_API_EXPORT void vae_Vector3_set_z(void* obj, float value) {
		reinterpret_cast<vae::Vector3*>(obj)->z = value;
	}

	_VAE_API_EXPORT float vae_Vector3_get_z(void* obj) {
		return reinterpret_cast<vae::Vector3*>(obj)->z;
	}
		
	_VAE_API_EXPORT void* vae_create_LocationDirection() {
		return new vae::LocationDirection();
	}

	_VAE_API_EXPORT void vae_destroy_LocationDirection(void* obj) {
		delete reinterpret_cast<vae::LocationDirection*>(obj);
	}
	
	_VAE_API_EXPORT void vae_LocationDirection_set_position(void* obj, void* value) {
		reinterpret_cast<vae::LocationDirection*>(obj)->position = *reinterpret_cast<vae::Vector3*>(value);
	}

	_VAE_API_EXPORT void* vae_LocationDirection_get_position(void* obj) {
		return &reinterpret_cast<vae::LocationDirection*>(obj)->position;
	}
		
	_VAE_API_EXPORT void vae_LocationDirection_set_direction(void* obj, void* value) {
		reinterpret_cast<vae::LocationDirection*>(obj)->direction = *reinterpret_cast<vae::Vector3*>(value);
	}

	_VAE_API_EXPORT void* vae_LocationDirection_get_direction(void* obj) {
		return &reinterpret_cast<vae::LocationDirection*>(obj)->direction;
	}
		
	_VAE_API_EXPORT void* vae_create_LocationOrientation() {
		return new vae::LocationOrientation();
	}

	_VAE_API_EXPORT void vae_destroy_LocationOrientation(void* obj) {
		delete reinterpret_cast<vae::LocationOrientation*>(obj);
	}
	
	_VAE_API_EXPORT void* vae_create_Engine() {
		return new vae::core::Engine();
	}

	_VAE_API_EXPORT void vae_destroy_Engine(void* obj) {
		delete reinterpret_cast<vae::core::Engine*>(obj);
	}
	
	_VAE_API_EXPORT int vae_Engine_init(
		void* obj,
		void* config
	) {
		return (int) reinterpret_cast<vae::core::Engine*>(obj)->init(
			*reinterpret_cast<vae::EngineConfig*>(config)
		);
	}

	_VAE_API_EXPORT int vae_Engine_start(
		void* obj
	) {
		return (int) reinterpret_cast<vae::core::Engine*>(obj)->start();
	}

	_VAE_API_EXPORT int vae_Engine_stop(
		void* obj
	) {
		return (int) reinterpret_cast<vae::core::Engine*>(obj)->stop();
	}

	_VAE_API_EXPORT void vae_Engine_update(
		void* obj
	) {
		return reinterpret_cast<vae::core::Engine*>(obj)->update();
	}

	_VAE_API_EXPORT int vae_Engine_fireEvent(
		void* obj,
		unsigned char bankHandle,
		unsigned short eventHandle,
		unsigned int emitterHandle,
		float gain,
		unsigned char mixerHandle,
		unsigned char listenerHandle
	) {
		return (int) reinterpret_cast<vae::core::Engine*>(obj)->fireEvent(
			bankHandle,
			eventHandle,
			emitterHandle,
			gain,
			mixerHandle,
			listenerHandle
		);
	}

	_VAE_API_EXPORT int vae_Engine_fireGlobalEvent(
		void* obj,
		unsigned int globalHandle,
		unsigned int emitterHandle,
		float gain,
		unsigned char mixerHandle,
		unsigned char listenerHandle
	) {
		return (int) reinterpret_cast<vae::core::Engine*>(obj)->fireGlobalEvent(
			globalHandle,
			emitterHandle,
			gain,
			mixerHandle,
			listenerHandle
		);
	}

	_VAE_API_EXPORT unsigned int vae_Engine_getActiveVoiceCount(
		void* obj
	) {
		return reinterpret_cast<vae::core::Engine*>(obj)->getActiveVoiceCount();
	}

	_VAE_API_EXPORT unsigned int vae_Engine_getStreamTime(
		void* obj
	) {
		return reinterpret_cast<vae::core::Engine*>(obj)->getStreamTime();
	}

	_VAE_API_EXPORT void vae_Engine_setMasterVolume(
		void* obj,
		float volume
	) {
		return reinterpret_cast<vae::core::Engine*>(obj)->setMasterVolume(
			volume
		);
	}

	_VAE_API_EXPORT bool vae_Engine_checkVersion(
		void* obj,
		int major,
		int minor,
		int patch
	) {
		return reinterpret_cast<vae::core::Engine*>(obj)->checkVersion(
			major,
			minor,
			patch
		);
	}

	_VAE_API_EXPORT unsigned int vae_Engine_createEmitter(
		void* obj
	) {
		return reinterpret_cast<vae::core::Engine*>(obj)->createEmitter();
	}

	_VAE_API_EXPORT unsigned int vae_Engine_createAutoEmitter(
		void* obj,
		unsigned char bank,
		unsigned short event,
		float maxDist,
		void* locDir,
		float spread
	) {
		return reinterpret_cast<vae::core::Engine*>(obj)->createAutoEmitter(
			bank,
			event,
			maxDist,
			*reinterpret_cast<vae::LocationDirection*>(locDir),
			spread
		);
	}

	_VAE_API_EXPORT int vae_Engine_addEmitter(
		void* obj,
		unsigned int h
	) {
		return (int) reinterpret_cast<vae::core::Engine*>(obj)->addEmitter(
			h
		);
	}

	_VAE_API_EXPORT int vae_Engine_removeEmitter(
		void* obj,
		unsigned int h
	) {
		return (int) reinterpret_cast<vae::core::Engine*>(obj)->removeEmitter(
			h
		);
	}

	_VAE_API_EXPORT int vae_Engine_setEmitter(
		void* obj,
		unsigned int emitter,
		void* locDir,
		float spread
	) {
		return (int) reinterpret_cast<vae::core::Engine*>(obj)->setEmitter(
			emitter,
			*reinterpret_cast<vae::LocationDirection*>(locDir),
			spread
		);
	}

	_VAE_API_EXPORT int vae_Engine_stopEmitter(
		void* obj,
		unsigned int emitter
	) {
		return (int) reinterpret_cast<vae::core::Engine*>(obj)->stopEmitter(
			emitter
		);
	}

	_VAE_API_EXPORT void vae_Engine_setVolume(
		void* obj,
		unsigned int emitter,
		float gain
	) {
		return reinterpret_cast<vae::core::Engine*>(obj)->setVolume(
			emitter,
			gain
		);
	}

	_VAE_API_EXPORT void vae_Engine_seek(
		void* obj,
		unsigned int emitter,
		unsigned int time
	) {
		return reinterpret_cast<vae::core::Engine*>(obj)->seek(
			emitter,
			time
		);
	}

	_VAE_API_EXPORT void vae_Engine_setSpeed(
		void* obj,
		unsigned int emitter,
		float speed
	) {
		return reinterpret_cast<vae::core::Engine*>(obj)->setSpeed(
			emitter,
			speed
		);
	}

	_VAE_API_EXPORT void vae_Engine_setLowpass(
		void* obj,
		unsigned int emitter,
		float cutoff
	) {
		return reinterpret_cast<vae::core::Engine*>(obj)->setLowpass(
			emitter,
			cutoff
		);
	}

	_VAE_API_EXPORT void vae_Engine_setHighpass(
		void* obj,
		unsigned int emitter,
		float cutoff
	) {
		return reinterpret_cast<vae::core::Engine*>(obj)->setHighpass(
			emitter,
			cutoff
		);
	}

	_VAE_API_EXPORT unsigned char vae_Engine_createListener(
		void* obj
	) {
		return reinterpret_cast<vae::core::Engine*>(obj)->createListener();
	}

	_VAE_API_EXPORT int vae_Engine_removeListener(
		void* obj,
		unsigned char listener
	) {
		return (int) reinterpret_cast<vae::core::Engine*>(obj)->removeListener(
			listener
		);
	}

	_VAE_API_EXPORT int vae_Engine_setListener(
		void* obj,
		unsigned char listener,
		void* locOr
	) {
		return (int) reinterpret_cast<vae::core::Engine*>(obj)->setListener(
			listener,
			*reinterpret_cast<vae::LocationOrientation*>(locOr)
		);
	}

	_VAE_API_EXPORT int vae_Engine_loadHRTF(
		void* obj,
		char* path,
		unsigned int size
	) {
		return (int) reinterpret_cast<vae::core::Engine*>(obj)->loadHRTF(
			path,
			size
		);
	}

	_VAE_API_EXPORT int vae_Engine_loadBank(
		void* obj,
		char* path,
		unsigned int size
	) {
		return (int) reinterpret_cast<vae::core::Engine*>(obj)->loadBank(
			path,
			size
		);
	}

	_VAE_API_EXPORT int vae_Engine_unloadBankFromId(
		void* obj,
		unsigned char bankHandle
	) {
		return (int) reinterpret_cast<vae::core::Engine*>(obj)->unloadBankFromId(
			bankHandle
		);
	}

	_VAE_API_EXPORT void vae_Engine_unloadAllBanks(
		void* obj
	) {
		return reinterpret_cast<vae::core::Engine*>(obj)->unloadAllBanks();
	}

} // extern "C"
