using System;
using System.Runtime.InteropServices;

namespace vae {
	static class vae_c_api
	{
		#if DEBUG
			public const string dll = "vae_c_apid.dll";
		#else
			public const string dll = "vae_c_api.dll";
		#endif
	}

	enum Result {
		Success = 0,
		GenericFailure = 1,
		BankFormatError = 2,
		BankFormatIndexError = 3,
		BankFormatDuplicateIndex = 4,
		FileOpenError = 5,
		VoiceStarvation = 6,
		BankFormatBadMixHirarchy = 7,
		ElementNotFound = 8,
		ValidHandleRequired = 9,
		TooManyRecords = 10,
		DeviceError = 11,
		DuplicateEmitter = 12,
		InvalidBank = 13,
		InvalidEmitter = 14
	}
	enum LogLevel {
		Debug = 0,
		Info = 1,
		Warn = 2,
		Error = 3,
		Ciritical = 4
	}
	enum SpeakerConfiguration {
		Mono = 0,
		Headphones = 1,
		Stereo = 2,
		HRTF = 3,
		Quadrophonic = 4,
		Suround = 5
	}

	using Sample = System.Single;
	using Size = System.UInt32;
	using Time = System.Double;
	using BankHandle = System.Byte;
	using EventHandle = System.UInt16;
	using SourceHandle = System.UInt16;
	using EmitterHandle = System.UInt32;
	using MixerHandle = System.Byte;
	using ListenerHandle = System.Byte;
	using GlobalEventHandle = System.UInt32;
	using GlobalMixerHandle = System.UInt16;
	using GlobalParameterHandle = System.UInt32;
	using Pointer = System.IntPtr;
	using CString = string;

	public class EventCallbackData : IDisposable {
		public Pointer _ptr;
		public bool _owned = false;

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Pointer vae_create_EventCallbackData();
		public EventCallbackData() {
			_ptr = vae_create_EventCallbackData();
			_owned = true;
		}

		// Temporary reference object
		public EventCallbackData(Pointer p) {
			_ptr = p;
			_owned = false;
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_destroy_EventCallbackData(Pointer obj);
		public void Dispose() {
			if (_owned) { vae_destroy_EventCallbackData(ptr); }
			Dispose();
			GC.SuppressFinalize(this);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Pointer vae_EventCallbackData_get_context(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_EventCallbackData_set_context(Pointer obj, Pointer value);
		public Pointer context {
			get => vae_EventCallbackData_get_context(_ptr);
			set => vae_EventCallbackData_set_context(_ptr, value);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern BankHandle vae_EventCallbackData_get_bank(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_EventCallbackData_set_bank(Pointer obj, BankHandle value);
		public BankHandle bank {
			get => vae_EventCallbackData_get_bank(_ptr);
			set => vae_EventCallbackData_set_bank(_ptr, value);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern EventHandle vae_EventCallbackData_get_event(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_EventCallbackData_set_event(Pointer obj, EventHandle value);
		public EventHandle event {
			get => vae_EventCallbackData_get_event(_ptr);
			set => vae_EventCallbackData_set_event(_ptr, value);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern EmitterHandle vae_EventCallbackData_get_emitter(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_EventCallbackData_set_emitter(Pointer obj, EmitterHandle value);
		public EmitterHandle emitter {
			get => vae_EventCallbackData_get_emitter(_ptr);
			set => vae_EventCallbackData_set_emitter(_ptr, value);
		}


	}

	public class EngineConfig : IDisposable {
		public Pointer _ptr;
		public bool _owned = false;

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Pointer vae_create_EngineConfig();
		public EngineConfig() {
			_ptr = vae_create_EngineConfig();
			_owned = true;
		}

		// Temporary reference object
		public EngineConfig(Pointer p) {
			_ptr = p;
			_owned = false;
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_destroy_EngineConfig(Pointer obj);
		public void Dispose() {
			if (_owned) { vae_destroy_EngineConfig(ptr); }
			Dispose();
			GC.SuppressFinalize(this);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern CString vae_EngineConfig_get_rootPath(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_EngineConfig_set_rootPath(Pointer obj, CString value);
		public CString rootPath {
			get => vae_EngineConfig_get_rootPath(_ptr);
			set => vae_EngineConfig_set_rootPath(_ptr, value);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Size vae_EngineConfig_get_internalSampleRate(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_EngineConfig_set_internalSampleRate(Pointer obj, Size value);
		public Size internalSampleRate {
			get => vae_EngineConfig_get_internalSampleRate(_ptr);
			set => vae_EngineConfig_set_internalSampleRate(_ptr, value);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Pointer vae_EngineConfig_get_eventCallbackContext(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_EngineConfig_set_eventCallbackContext(Pointer obj, Pointer value);
		public Pointer eventCallbackContext {
			get => vae_EngineConfig_get_eventCallbackContext(_ptr);
			set => vae_EngineConfig_set_eventCallbackContext(_ptr, value);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Size vae_EngineConfig_get_preAllocatedEmitters(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_EngineConfig_set_preAllocatedEmitters(Pointer obj, Size value);
		public Size preAllocatedEmitters {
			get => vae_EngineConfig_get_preAllocatedEmitters(_ptr);
			set => vae_EngineConfig_set_preAllocatedEmitters(_ptr, value);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Size vae_EngineConfig_get_voices(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_EngineConfig_set_voices(Pointer obj, Size value);
		public Size voices {
			get => vae_EngineConfig_get_voices(_ptr);
			set => vae_EngineConfig_set_voices(_ptr, value);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Size vae_EngineConfig_get_hrtfVoices(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_EngineConfig_set_hrtfVoices(Pointer obj, Size value);
		public Size hrtfVoices {
			get => vae_EngineConfig_get_hrtfVoices(_ptr);
			set => vae_EngineConfig_set_hrtfVoices(_ptr, value);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Size vae_EngineConfig_get_virtualVoices(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_EngineConfig_set_virtualVoices(Pointer obj, Size value);
		public Size virtualVoices {
			get => vae_EngineConfig_get_virtualVoices(_ptr);
			set => vae_EngineConfig_set_virtualVoices(_ptr, value);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Size vae_EngineConfig_get_finishedVoiceQueueSize(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_EngineConfig_set_finishedVoiceQueueSize(Pointer obj, Size value);
		public Size finishedVoiceQueueSize {
			get => vae_EngineConfig_get_finishedVoiceQueueSize(_ptr);
			set => vae_EngineConfig_set_finishedVoiceQueueSize(_ptr, value);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Size vae_EngineConfig_get_preferredBufferSize(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_EngineConfig_set_preferredBufferSize(Pointer obj, Size value);
		public Size preferredBufferSize {
			get => vae_EngineConfig_get_preferredBufferSize(_ptr);
			set => vae_EngineConfig_set_preferredBufferSize(_ptr, value);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Size vae_EngineConfig_get_bufferPeriods(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_EngineConfig_set_bufferPeriods(Pointer obj, Size value);
		public Size bufferPeriods {
			get => vae_EngineConfig_get_bufferPeriods(_ptr);
			set => vae_EngineConfig_set_bufferPeriods(_ptr, value);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Pointer vae_EngineConfig_get_updateInAudioThread(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_EngineConfig_set_updateInAudioThread(Pointer obj, Pointer value);
		public bool updateInAudioThread {
			get => new bool(vae_EngineConfig_get_updateInAudioThread(_ptr));
			set => vae_EngineConfig_set_updateInAudioThread(_ptr, value._ptr);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Pointer vae_EngineConfig_get_processInBufferSwitch(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_EngineConfig_set_processInBufferSwitch(Pointer obj, Pointer value);
		public bool processInBufferSwitch {
			get => new bool(vae_EngineConfig_get_processInBufferSwitch(_ptr));
			set => vae_EngineConfig_set_processInBufferSwitch(_ptr, value._ptr);
		}


	}

	public class DeviceInfo : IDisposable {
		public Pointer _ptr;
		public bool _owned = false;

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Pointer vae_create_DeviceInfo();
		public DeviceInfo() {
			_ptr = vae_create_DeviceInfo();
			_owned = true;
		}

		// Temporary reference object
		public DeviceInfo(Pointer p) {
			_ptr = p;
			_owned = false;
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_destroy_DeviceInfo(Pointer obj);
		public void Dispose() {
			if (_owned) { vae_destroy_DeviceInfo(ptr); }
			Dispose();
			GC.SuppressFinalize(this);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Pointer vae_DeviceInfo_get_id(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_DeviceInfo_set_id(Pointer obj, Pointer value);
		public int id {
			get => new int(vae_DeviceInfo_get_id(_ptr));
			set => vae_DeviceInfo_set_id(_ptr, value._ptr);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Size vae_DeviceInfo_get_sampleRate(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_DeviceInfo_set_sampleRate(Pointer obj, Size value);
		public Size sampleRate {
			get => vae_DeviceInfo_get_sampleRate(_ptr);
			set => vae_DeviceInfo_set_sampleRate(_ptr, value);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Size vae_DeviceInfo_get_bufferSize(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_DeviceInfo_set_bufferSize(Pointer obj, Size value);
		public Size bufferSize {
			get => vae_DeviceInfo_get_bufferSize(_ptr);
			set => vae_DeviceInfo_set_bufferSize(_ptr, value);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Pointer vae_DeviceInfo_get_channelsIn(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_DeviceInfo_set_channelsIn(Pointer obj, Pointer value);
		public unsigned char channelsIn {
			get => new unsigned char(vae_DeviceInfo_get_channelsIn(_ptr));
			set => vae_DeviceInfo_set_channelsIn(_ptr, value._ptr);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Pointer vae_DeviceInfo_get_channelsOut(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_DeviceInfo_set_channelsOut(Pointer obj, Pointer value);
		public unsigned char channelsOut {
			get => new unsigned char(vae_DeviceInfo_get_channelsOut(_ptr));
			set => vae_DeviceInfo_set_channelsOut(_ptr, value._ptr);
		}


	}

	public class Vector3 : IDisposable {
		public Pointer _ptr;
		public bool _owned = false;

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Pointer vae_create_Vector3();
		public Vector3() {
			_ptr = vae_create_Vector3();
			_owned = true;
		}

		// Temporary reference object
		public Vector3(Pointer p) {
			_ptr = p;
			_owned = false;
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_destroy_Vector3(Pointer obj);
		public void Dispose() {
			if (_owned) { vae_destroy_Vector3(ptr); }
			Dispose();
			GC.SuppressFinalize(this);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Pointer vae_Vector3_get_x(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_Vector3_set_x(Pointer obj, Pointer value);
		public float x {
			get => new float(vae_Vector3_get_x(_ptr));
			set => vae_Vector3_set_x(_ptr, value._ptr);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Pointer vae_Vector3_get_y(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_Vector3_set_y(Pointer obj, Pointer value);
		public float y {
			get => new float(vae_Vector3_get_y(_ptr));
			set => vae_Vector3_set_y(_ptr, value._ptr);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Pointer vae_Vector3_get_z(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_Vector3_set_z(Pointer obj, Pointer value);
		public float z {
			get => new float(vae_Vector3_get_z(_ptr));
			set => vae_Vector3_set_z(_ptr, value._ptr);
		}


	}

	public class LocationDirection : IDisposable {
		public Pointer _ptr;
		public bool _owned = false;

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Pointer vae_create_LocationDirection();
		public LocationDirection() {
			_ptr = vae_create_LocationDirection();
			_owned = true;
		}

		// Temporary reference object
		public LocationDirection(Pointer p) {
			_ptr = p;
			_owned = false;
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_destroy_LocationDirection(Pointer obj);
		public void Dispose() {
			if (_owned) { vae_destroy_LocationDirection(ptr); }
			Dispose();
			GC.SuppressFinalize(this);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Pointer vae_LocationDirection_get_position(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_LocationDirection_set_position(Pointer obj, Pointer value);
		public Vector3 position {
			get => new Vector3(vae_LocationDirection_get_position(_ptr));
			set => vae_LocationDirection_set_position(_ptr, value._ptr);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Pointer vae_LocationDirection_get_direction(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_LocationDirection_set_direction(Pointer obj, Pointer value);
		public Vector3 direction {
			get => new Vector3(vae_LocationDirection_get_direction(_ptr));
			set => vae_LocationDirection_set_direction(_ptr, value._ptr);
		}


	}

	public class LocationOrientation : IDisposable {
		public Pointer _ptr;
		public bool _owned = false;

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Pointer vae_create_LocationOrientation();
		public LocationOrientation() {
			_ptr = vae_create_LocationOrientation();
			_owned = true;
		}

		// Temporary reference object
		public LocationOrientation(Pointer p) {
			_ptr = p;
			_owned = false;
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_destroy_LocationOrientation(Pointer obj);
		public void Dispose() {
			if (_owned) { vae_destroy_LocationOrientation(ptr); }
			Dispose();
			GC.SuppressFinalize(this);
		}


	}

	public class Engine : IDisposable {
		public Pointer _ptr;
		public bool _owned = false;

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Pointer vae_create_Engine();
		public Engine() {
			_ptr = vae_create_Engine();
			_owned = true;
		}

		// Temporary reference object
		public Engine(Pointer p) {
			_ptr = p;
			_owned = false;
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_destroy_Engine(Pointer obj);
		public void Dispose() {
			if (_owned) { vae_destroy_Engine(ptr); }
			Dispose();
			GC.SuppressFinalize(this);
		}


		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		Result vae_Engine_init(
			Pointer obj,
			Pointer config
		);

		Result init(
			Pointer config
		) {
			return vae_Engine_init(
				_ptr,
				config._ptr
			);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		Result vae_Engine_start(
			Pointer obj
		);

		Result start(
		) {
			return vae_Engine_start(
				_ptr
			);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		Result vae_Engine_stop(
			Pointer obj
		);

		Result stop(
		) {
			return vae_Engine_stop(
				_ptr
			);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		void vae_Engine_update(
			Pointer obj
		);

		void update(
		) {
			return vae_Engine_update(
				_ptr
			);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		Result vae_Engine_fireEvent(
			Pointer obj,
			BankHandle bankHandle,
			EventHandle eventHandle,
			EmitterHandle emitterHandle,
			Sample gain,
			MixerHandle mixerHandle,
			ListenerHandle listenerHandle
		);

		Result fireEvent(
			BankHandle bankHandle,
			EventHandle eventHandle,
			EmitterHandle emitterHandle,
			Sample gain,
			MixerHandle mixerHandle,
			ListenerHandle listenerHandle
		) {
			return vae_Engine_fireEvent(
				_ptr,
				bankHandle,
				eventHandle,
				emitterHandle,
				gain,
				mixerHandle,
				listenerHandle
			);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		Result vae_Engine_fireGlobalEvent(
			Pointer obj,
			GlobalEventHandle globalHandle,
			EmitterHandle emitterHandle,
			Sample gain,
			MixerHandle mixerHandle,
			ListenerHandle listenerHandle
		);

		Result fireGlobalEvent(
			GlobalEventHandle globalHandle,
			EmitterHandle emitterHandle,
			Sample gain,
			MixerHandle mixerHandle,
			ListenerHandle listenerHandle
		) {
			return vae_Engine_fireGlobalEvent(
				_ptr,
				globalHandle,
				emitterHandle,
				gain,
				mixerHandle,
				listenerHandle
			);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		Size vae_Engine_getActiveVoiceCount(
			Pointer obj
		);

		Size getActiveVoiceCount(
		) {
			return vae_Engine_getActiveVoiceCount(
				_ptr
			);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		Size vae_Engine_getStreamTime(
			Pointer obj
		);

		Size getStreamTime(
		) {
			return vae_Engine_getStreamTime(
				_ptr
			);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		void vae_Engine_setMasterVolume(
			Pointer obj,
			Sample volume
		);

		void setMasterVolume(
			Sample volume
		) {
			return vae_Engine_setMasterVolume(
				_ptr,
				volume
			);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		bool vae_Engine_checkVersion(
			Pointer obj,
			Pointer major,
			Pointer minor,
			Pointer patch
		);

		bool checkVersion(
			Pointer major,
			Pointer minor,
			Pointer patch
		) {
			return vae_Engine_checkVersion(
				_ptr,
				major._ptr,
				minor._ptr,
				patch._ptr
			);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		EmitterHandle vae_Engine_createEmitter(
			Pointer obj
		);

		EmitterHandle createEmitter(
		) {
			return vae_Engine_createEmitter(
				_ptr
			);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		EmitterHandle vae_Engine_createAutoEmitter(
			Pointer obj,
			BankHandle bank,
			EventHandle event,
			Pointer maxDist,
			Pointer locDir,
			Pointer spread
		);

		EmitterHandle createAutoEmitter(
			BankHandle bank,
			EventHandle event,
			Pointer maxDist,
			Pointer locDir,
			Pointer spread
		) {
			return vae_Engine_createAutoEmitter(
				_ptr,
				bank,
				event,
				maxDist._ptr,
				locDir._ptr,
				spread._ptr
			);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		Result vae_Engine_addEmitter(
			Pointer obj,
			EmitterHandle h
		);

		Result addEmitter(
			EmitterHandle h
		) {
			return vae_Engine_addEmitter(
				_ptr,
				h
			);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		Result vae_Engine_removeEmitter(
			Pointer obj,
			EmitterHandle h
		);

		Result removeEmitter(
			EmitterHandle h
		) {
			return vae_Engine_removeEmitter(
				_ptr,
				h
			);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		Result vae_Engine_setEmitter(
			Pointer obj,
			EmitterHandle emitter,
			Pointer locDir,
			Pointer spread
		);

		Result setEmitter(
			EmitterHandle emitter,
			Pointer locDir,
			Pointer spread
		) {
			return vae_Engine_setEmitter(
				_ptr,
				emitter,
				locDir._ptr,
				spread._ptr
			);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		Result vae_Engine_stopEmitter(
			Pointer obj,
			EmitterHandle emitter
		);

		Result stopEmitter(
			EmitterHandle emitter
		) {
			return vae_Engine_stopEmitter(
				_ptr,
				emitter
			);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		void vae_Engine_setVolume(
			Pointer obj,
			EmitterHandle emitter,
			Sample gain
		);

		void setVolume(
			EmitterHandle emitter,
			Sample gain
		) {
			return vae_Engine_setVolume(
				_ptr,
				emitter,
				gain
			);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		void vae_Engine_seek(
			Pointer obj,
			EmitterHandle emitter,
			Size time
		);

		void seek(
			EmitterHandle emitter,
			Size time
		) {
			return vae_Engine_seek(
				_ptr,
				emitter,
				time
			);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		void vae_Engine_setSpeed(
			Pointer obj,
			EmitterHandle emitter,
			Pointer speed
		);

		void setSpeed(
			EmitterHandle emitter,
			Pointer speed
		) {
			return vae_Engine_setSpeed(
				_ptr,
				emitter,
				speed._ptr
			);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		void vae_Engine_setLowpass(
			Pointer obj,
			EmitterHandle emitter,
			Pointer cutoff
		);

		void setLowpass(
			EmitterHandle emitter,
			Pointer cutoff
		) {
			return vae_Engine_setLowpass(
				_ptr,
				emitter,
				cutoff._ptr
			);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		void vae_Engine_setHighpass(
			Pointer obj,
			EmitterHandle emitter,
			Pointer cutoff
		);

		void setHighpass(
			EmitterHandle emitter,
			Pointer cutoff
		) {
			return vae_Engine_setHighpass(
				_ptr,
				emitter,
				cutoff._ptr
			);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		ListenerHandle vae_Engine_createListener(
			Pointer obj
		);

		ListenerHandle createListener(
		) {
			return vae_Engine_createListener(
				_ptr
			);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		Result vae_Engine_removeListener(
			Pointer obj,
			ListenerHandle listener
		);

		Result removeListener(
			ListenerHandle listener
		) {
			return vae_Engine_removeListener(
				_ptr,
				listener
			);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		Result vae_Engine_setListener(
			Pointer obj,
			ListenerHandle listener,
			Pointer locOr
		);

		Result setListener(
			ListenerHandle listener,
			Pointer locOr
		) {
			return vae_Engine_setListener(
				_ptr,
				listener,
				locOr._ptr
			);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		Result vae_Engine_loadHRTF(
			Pointer obj,
			CString path,
			Size size
		);

		Result loadHRTF(
			CString path,
			Size size
		) {
			return vae_Engine_loadHRTF(
				_ptr,
				path,
				size
			);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		Result vae_Engine_loadBank(
			Pointer obj,
			CString path,
			Size size
		);

		Result loadBank(
			CString path,
			Size size
		) {
			return vae_Engine_loadBank(
				_ptr,
				path,
				size
			);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		Result vae_Engine_unloadBankFromId(
			Pointer obj,
			BankHandle bankHandle
		);

		Result unloadBankFromId(
			BankHandle bankHandle
		) {
			return vae_Engine_unloadBankFromId(
				_ptr,
				bankHandle
			);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		void vae_Engine_unloadAllBanks(
			Pointer obj
		);

		void unloadAllBanks(
		) {
			return vae_Engine_unloadAllBanks(
				_ptr
			);
		}

	}

}