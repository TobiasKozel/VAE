using System;
using System.Runtime.InteropServices;

namespace vae {
	using Sample = System.Single;
	using Position = System.Single;
	using Size = System.UInt32;
	using Time = System.Double;
	using Channel = System.Byte;
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
	using CString = System.String;

	static class vae_c_api
	{
		public const string dll = "vae_c_api";
	}

	public enum Result {
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
	public enum LogLevel {
		Debug = 0,
		Info = 1,
		Warn = 2,
		Error = 3,
		Ciritical = 4
	}
	public enum SpeakerConfiguration {
		Mono = 0,
		Headphones = 1,
		Stereo = 2,
		HRTF = 3,
		Quadrophonic = 4,
		Suround = 5
	}

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
			if (_owned) { vae_destroy_EventCallbackData(_ptr); }
			Dispose();
			GC.SuppressFinalize(this);
		}

		// ------------------------------------------
		// 			PROPERTIES
		// ------------------------------------------
		// property EventCallbackData context
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Pointer vae_EventCallbackData_get_context(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_EventCallbackData_set_context(Pointer obj, Pointer value);
		public Pointer Context {
			get => vae_EventCallbackData_get_context(_ptr);
			set => vae_EventCallbackData_set_context(_ptr, value);
		}

		// property EventCallbackData bank
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern BankHandle vae_EventCallbackData_get_bank(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_EventCallbackData_set_bank(Pointer obj, BankHandle value);
		public BankHandle Bank {
			get => vae_EventCallbackData_get_bank(_ptr);
			set => vae_EventCallbackData_set_bank(_ptr, value);
		}

		// property EventCallbackData event
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern EventHandle vae_EventCallbackData_get_event(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_EventCallbackData_set_event(Pointer obj, EventHandle value);
		public EventHandle Event {
			get => vae_EventCallbackData_get_event(_ptr);
			set => vae_EventCallbackData_set_event(_ptr, value);
		}

		// property EventCallbackData emitter
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern EmitterHandle vae_EventCallbackData_get_emitter(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_EventCallbackData_set_emitter(Pointer obj, EmitterHandle value);
		public EmitterHandle Emitter {
			get => vae_EventCallbackData_get_emitter(_ptr);
			set => vae_EventCallbackData_set_emitter(_ptr, value);
		}


		// ------------------------------------------
		// 			FUNCTIONS
		// ------------------------------------------
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
			if (_owned) { vae_destroy_EngineConfig(_ptr); }
			Dispose();
			GC.SuppressFinalize(this);
		}

		// ------------------------------------------
		// 			PROPERTIES
		// ------------------------------------------
		// property EngineConfig rootPath
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern CString vae_EngineConfig_get_rootPath(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_EngineConfig_set_rootPath(Pointer obj, CString value);
		public CString RootPath {
			get => vae_EngineConfig_get_rootPath(_ptr);
			set => vae_EngineConfig_set_rootPath(_ptr, value);
		}

		// property EngineConfig internalSampleRate
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Size vae_EngineConfig_get_internalSampleRate(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_EngineConfig_set_internalSampleRate(Pointer obj, Size value);
		public Size InternalSampleRate {
			get => vae_EngineConfig_get_internalSampleRate(_ptr);
			set => vae_EngineConfig_set_internalSampleRate(_ptr, value);
		}

		// property EngineConfig eventCallbackContext
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Pointer vae_EngineConfig_get_eventCallbackContext(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_EngineConfig_set_eventCallbackContext(Pointer obj, Pointer value);
		public Pointer EventCallbackContext {
			get => vae_EngineConfig_get_eventCallbackContext(_ptr);
			set => vae_EngineConfig_set_eventCallbackContext(_ptr, value);
		}

		// property EngineConfig preAllocatedEmitters
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Size vae_EngineConfig_get_preAllocatedEmitters(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_EngineConfig_set_preAllocatedEmitters(Pointer obj, Size value);
		public Size PreAllocatedEmitters {
			get => vae_EngineConfig_get_preAllocatedEmitters(_ptr);
			set => vae_EngineConfig_set_preAllocatedEmitters(_ptr, value);
		}

		// property EngineConfig voices
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Size vae_EngineConfig_get_voices(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_EngineConfig_set_voices(Pointer obj, Size value);
		public Size Voices {
			get => vae_EngineConfig_get_voices(_ptr);
			set => vae_EngineConfig_set_voices(_ptr, value);
		}

		// property EngineConfig hrtfVoices
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Size vae_EngineConfig_get_hrtfVoices(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_EngineConfig_set_hrtfVoices(Pointer obj, Size value);
		public Size HrtfVoices {
			get => vae_EngineConfig_get_hrtfVoices(_ptr);
			set => vae_EngineConfig_set_hrtfVoices(_ptr, value);
		}

		// property EngineConfig virtualVoices
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Size vae_EngineConfig_get_virtualVoices(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_EngineConfig_set_virtualVoices(Pointer obj, Size value);
		public Size VirtualVoices {
			get => vae_EngineConfig_get_virtualVoices(_ptr);
			set => vae_EngineConfig_set_virtualVoices(_ptr, value);
		}

		// property EngineConfig finishedVoiceQueueSize
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Size vae_EngineConfig_get_finishedVoiceQueueSize(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_EngineConfig_set_finishedVoiceQueueSize(Pointer obj, Size value);
		public Size FinishedVoiceQueueSize {
			get => vae_EngineConfig_get_finishedVoiceQueueSize(_ptr);
			set => vae_EngineConfig_set_finishedVoiceQueueSize(_ptr, value);
		}

		// property EngineConfig preferredBufferSize
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Size vae_EngineConfig_get_preferredBufferSize(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_EngineConfig_set_preferredBufferSize(Pointer obj, Size value);
		public Size PreferredBufferSize {
			get => vae_EngineConfig_get_preferredBufferSize(_ptr);
			set => vae_EngineConfig_set_preferredBufferSize(_ptr, value);
		}

		// property EngineConfig bufferPeriods
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Size vae_EngineConfig_get_bufferPeriods(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_EngineConfig_set_bufferPeriods(Pointer obj, Size value);
		public Size BufferPeriods {
			get => vae_EngineConfig_get_bufferPeriods(_ptr);
			set => vae_EngineConfig_set_bufferPeriods(_ptr, value);
		}

		// property EngineConfig updateInAudioThread
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern bool vae_EngineConfig_get_updateInAudioThread(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_EngineConfig_set_updateInAudioThread(Pointer obj, bool value);
		public bool UpdateInAudioThread {
			get => vae_EngineConfig_get_updateInAudioThread(_ptr);
			set => vae_EngineConfig_set_updateInAudioThread(_ptr, value);
		}

		// property EngineConfig processInBufferSwitch
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern bool vae_EngineConfig_get_processInBufferSwitch(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_EngineConfig_set_processInBufferSwitch(Pointer obj, bool value);
		public bool ProcessInBufferSwitch {
			get => vae_EngineConfig_get_processInBufferSwitch(_ptr);
			set => vae_EngineConfig_set_processInBufferSwitch(_ptr, value);
		}


		// ------------------------------------------
		// 			FUNCTIONS
		// ------------------------------------------
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
			if (_owned) { vae_destroy_DeviceInfo(_ptr); }
			Dispose();
			GC.SuppressFinalize(this);
		}

		// ------------------------------------------
		// 			PROPERTIES
		// ------------------------------------------
		// property DeviceInfo id
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern int vae_DeviceInfo_get_id(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_DeviceInfo_set_id(Pointer obj, int value);
		public int Id {
			get => vae_DeviceInfo_get_id(_ptr);
			set => vae_DeviceInfo_set_id(_ptr, value);
		}

		// property DeviceInfo sampleRate
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Size vae_DeviceInfo_get_sampleRate(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_DeviceInfo_set_sampleRate(Pointer obj, Size value);
		public Size SampleRate {
			get => vae_DeviceInfo_get_sampleRate(_ptr);
			set => vae_DeviceInfo_set_sampleRate(_ptr, value);
		}

		// property DeviceInfo bufferSize
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Size vae_DeviceInfo_get_bufferSize(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_DeviceInfo_set_bufferSize(Pointer obj, Size value);
		public Size BufferSize {
			get => vae_DeviceInfo_get_bufferSize(_ptr);
			set => vae_DeviceInfo_set_bufferSize(_ptr, value);
		}

		// property DeviceInfo channelsIn
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Channel vae_DeviceInfo_get_channelsIn(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_DeviceInfo_set_channelsIn(Pointer obj, Channel value);
		public Channel ChannelsIn {
			get => vae_DeviceInfo_get_channelsIn(_ptr);
			set => vae_DeviceInfo_set_channelsIn(_ptr, value);
		}

		// property DeviceInfo channelsOut
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Channel vae_DeviceInfo_get_channelsOut(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_DeviceInfo_set_channelsOut(Pointer obj, Channel value);
		public Channel ChannelsOut {
			get => vae_DeviceInfo_get_channelsOut(_ptr);
			set => vae_DeviceInfo_set_channelsOut(_ptr, value);
		}


		// ------------------------------------------
		// 			FUNCTIONS
		// ------------------------------------------
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
			if (_owned) { vae_destroy_Vector3(_ptr); }
			Dispose();
			GC.SuppressFinalize(this);
		}

		// ------------------------------------------
		// 			PROPERTIES
		// ------------------------------------------
		// property Vector3 x
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Position vae_Vector3_get_x(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_Vector3_set_x(Pointer obj, Position value);
		public Position X {
			get => vae_Vector3_get_x(_ptr);
			set => vae_Vector3_set_x(_ptr, value);
		}

		// property Vector3 y
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Position vae_Vector3_get_y(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_Vector3_set_y(Pointer obj, Position value);
		public Position Y {
			get => vae_Vector3_get_y(_ptr);
			set => vae_Vector3_set_y(_ptr, value);
		}

		// property Vector3 z
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Position vae_Vector3_get_z(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_Vector3_set_z(Pointer obj, Position value);
		public Position Z {
			get => vae_Vector3_get_z(_ptr);
			set => vae_Vector3_set_z(_ptr, value);
		}


		// ------------------------------------------
		// 			FUNCTIONS
		// ------------------------------------------
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
			if (_owned) { vae_destroy_LocationDirection(_ptr); }
			Dispose();
			GC.SuppressFinalize(this);
		}

		// ------------------------------------------
		// 			PROPERTIES
		// ------------------------------------------
		// property LocationDirection position
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Pointer vae_LocationDirection_get_position(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_LocationDirection_set_position(Pointer obj, Pointer value);
		public Vector3 Position {
			get => new Vector3(vae_LocationDirection_get_position(_ptr));
			set => vae_LocationDirection_set_position(_ptr, value._ptr);
		}

		// property LocationDirection direction
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Pointer vae_LocationDirection_get_direction(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_LocationDirection_set_direction(Pointer obj, Pointer value);
		public Vector3 Direction {
			get => new Vector3(vae_LocationDirection_get_direction(_ptr));
			set => vae_LocationDirection_set_direction(_ptr, value._ptr);
		}


		// ------------------------------------------
		// 			FUNCTIONS
		// ------------------------------------------
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
			if (_owned) { vae_destroy_LocationOrientation(_ptr); }
			Dispose();
			GC.SuppressFinalize(this);
		}

		// ------------------------------------------
		// 			PROPERTIES
		// ------------------------------------------

		// ------------------------------------------
		// 			FUNCTIONS
		// ------------------------------------------
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
			if (_owned) { vae_destroy_Engine(_ptr); }
			Dispose();
			GC.SuppressFinalize(this);
		}

		// ------------------------------------------
		// 			PROPERTIES
		// ------------------------------------------

		// ------------------------------------------
		// 			FUNCTIONS
		// ------------------------------------------
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Result vae_Engine_init(
			Pointer obj,
			Pointer Config
		);

		public Result Init(
			EngineConfig Config
		) {
			return
			vae_Engine_init(
				_ptr,
				Config._ptr);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Result vae_Engine_start(
			Pointer obj
		);

		public Result Start(
		) {
			return
			vae_Engine_start(
				_ptr);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Result vae_Engine_stop(
			Pointer obj
		);

		public Result Stop(
		) {
			return
			vae_Engine_stop(
				_ptr);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_Engine_update(
			Pointer obj
		);

		public void Update(
		) {
			vae_Engine_update(
				_ptr);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Result vae_Engine_fireEvent(
			Pointer obj,
			BankHandle BankHandle,
			EventHandle EventHandle,
			EmitterHandle EmitterHandle,
			Sample Gain,
			MixerHandle MixerHandle,
			ListenerHandle ListenerHandle
		);

		public Result FireEvent(
			BankHandle BankHandle,
			EventHandle EventHandle,
			EmitterHandle EmitterHandle,
			Sample Gain,
			MixerHandle MixerHandle,
			ListenerHandle ListenerHandle
		) {
			return
			vae_Engine_fireEvent(
				_ptr,
				BankHandle,
				EventHandle,
				EmitterHandle,
				Gain,
				MixerHandle,
				ListenerHandle);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Result vae_Engine_fireGlobalEvent(
			Pointer obj,
			GlobalEventHandle GlobalHandle,
			EmitterHandle EmitterHandle,
			Sample Gain,
			MixerHandle MixerHandle,
			ListenerHandle ListenerHandle
		);

		public Result FireGlobalEvent(
			GlobalEventHandle GlobalHandle,
			EmitterHandle EmitterHandle,
			Sample Gain,
			MixerHandle MixerHandle,
			ListenerHandle ListenerHandle
		) {
			return
			vae_Engine_fireGlobalEvent(
				_ptr,
				GlobalHandle,
				EmitterHandle,
				Gain,
				MixerHandle,
				ListenerHandle);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Size vae_Engine_getActiveVoiceCount(
			Pointer obj
		);

		public Size GetActiveVoiceCount(
		) {
			return
			vae_Engine_getActiveVoiceCount(
				_ptr);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Size vae_Engine_getInactiveVoiceCount(
			Pointer obj
		);

		public Size GetInactiveVoiceCount(
		) {
			return
			vae_Engine_getInactiveVoiceCount(
				_ptr);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Size vae_Engine_getStreamTime(
			Pointer obj
		);

		public Size GetStreamTime(
		) {
			return
			vae_Engine_getStreamTime(
				_ptr);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_Engine_setMasterVolume(
			Pointer obj,
			Sample Volume
		);

		public void SetMasterVolume(
			Sample Volume
		) {
			vae_Engine_setMasterVolume(
				_ptr,
				Volume);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern bool vae_Engine_checkVersion(
			Pointer obj,
			int Major,
			int Minor,
			int Patch
		);

		public bool CheckVersion(
			int Major,
			int Minor,
			int Patch
		) {
			return
			vae_Engine_checkVersion(
				_ptr,
				Major,
				Minor,
				Patch);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern EmitterHandle vae_Engine_createEmitter(
			Pointer obj
		);

		public EmitterHandle CreateEmitter(
		) {
			return
			vae_Engine_createEmitter(
				_ptr);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern EmitterHandle vae_Engine_createAutoEmitter(
			Pointer obj,
			BankHandle Bank,
			EventHandle Event,
			Position MaxDist,
			Pointer LocDir,
			Sample Spread
		);

		public EmitterHandle CreateAutoEmitter(
			BankHandle Bank,
			EventHandle Event,
			Position MaxDist,
			LocationDirection LocDir,
			Sample Spread
		) {
			return
			vae_Engine_createAutoEmitter(
				_ptr,
				Bank,
				Event,
				MaxDist,
				LocDir._ptr,
				Spread);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Result vae_Engine_addEmitter(
			Pointer obj,
			EmitterHandle H
		);

		public Result AddEmitter(
			EmitterHandle H
		) {
			return
			vae_Engine_addEmitter(
				_ptr,
				H);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Result vae_Engine_removeEmitter(
			Pointer obj,
			EmitterHandle H
		);

		public Result RemoveEmitter(
			EmitterHandle H
		) {
			return
			vae_Engine_removeEmitter(
				_ptr,
				H);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Result vae_Engine_setEmitter(
			Pointer obj,
			EmitterHandle Emitter,
			Pointer LocDir,
			Sample Spread
		);

		public Result SetEmitter(
			EmitterHandle Emitter,
			LocationDirection LocDir,
			Sample Spread
		) {
			return
			vae_Engine_setEmitter(
				_ptr,
				Emitter,
				LocDir._ptr,
				Spread);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Result vae_Engine_stopEmitter(
			Pointer obj,
			EmitterHandle Emitter
		);

		public Result StopEmitter(
			EmitterHandle Emitter
		) {
			return
			vae_Engine_stopEmitter(
				_ptr,
				Emitter);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_Engine_setVolume(
			Pointer obj,
			EmitterHandle Emitter,
			Sample Gain
		);

		public void SetVolume(
			EmitterHandle Emitter,
			Sample Gain
		) {
			vae_Engine_setVolume(
				_ptr,
				Emitter,
				Gain);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_Engine_seek(
			Pointer obj,
			EmitterHandle Emitter,
			Size Time
		);

		public void Seek(
			EmitterHandle Emitter,
			Size Time
		) {
			vae_Engine_seek(
				_ptr,
				Emitter,
				Time);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_Engine_setSpeed(
			Pointer obj,
			EmitterHandle Emitter,
			Sample Speed
		);

		public void SetSpeed(
			EmitterHandle Emitter,
			Sample Speed
		) {
			vae_Engine_setSpeed(
				_ptr,
				Emitter,
				Speed);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_Engine_setLowpass(
			Pointer obj,
			EmitterHandle Emitter,
			Sample Cutoff
		);

		public void SetLowpass(
			EmitterHandle Emitter,
			Sample Cutoff
		) {
			vae_Engine_setLowpass(
				_ptr,
				Emitter,
				Cutoff);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_Engine_setHighpass(
			Pointer obj,
			EmitterHandle Emitter,
			Sample Cutoff
		);

		public void SetHighpass(
			EmitterHandle Emitter,
			Sample Cutoff
		) {
			vae_Engine_setHighpass(
				_ptr,
				Emitter,
				Cutoff);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern ListenerHandle vae_Engine_createListener(
			Pointer obj
		);

		public ListenerHandle CreateListener(
		) {
			return
			vae_Engine_createListener(
				_ptr);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Result vae_Engine_removeListener(
			Pointer obj,
			ListenerHandle Listener
		);

		public Result RemoveListener(
			ListenerHandle Listener
		) {
			return
			vae_Engine_removeListener(
				_ptr,
				Listener);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Result vae_Engine_setListener(
			Pointer obj,
			ListenerHandle Listener,
			Pointer LocOr
		);

		public Result SetListener(
			ListenerHandle Listener,
			LocationOrientation LocOr
		) {
			return
			vae_Engine_setListener(
				_ptr,
				Listener,
				LocOr._ptr);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Result vae_Engine_loadHRTF(
			Pointer obj,
			CString Path,
			Size Size
		);

		public Result LoadHRTF(
			CString Path,
			Size Size
		) {
			return
			vae_Engine_loadHRTF(
				_ptr,
				Path,
				Size);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Result vae_Engine_loadBank(
			Pointer obj,
			CString Path,
			Size Size
		);

		public Result LoadBank(
			CString Path,
			Size Size
		) {
			return
			vae_Engine_loadBank(
				_ptr,
				Path,
				Size);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Result vae_Engine_unloadBankFromId(
			Pointer obj,
			BankHandle BankHandle
		);

		public Result UnloadBankFromId(
			BankHandle BankHandle
		) {
			return
			vae_Engine_unloadBankFromId(
				_ptr,
				BankHandle);
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_Engine_unloadAllBanks(
			Pointer obj
		);

		public void UnloadAllBanks(
		) {
			vae_Engine_unloadAllBanks(
				_ptr);
		}

	}

}