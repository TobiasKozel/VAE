
#ifndef _VAE_GEN_EVENT_NAMES
#define _VAE_GEN_EVENT_NAMES
namespace vaeb {

	struct Bank2 {
		static constexpr vae::BankHandle id = 1;
		static constexpr vae::GlobalEventHandle Ambience1 = 0 | (1 << (sizeof(vae::EventHandle) * 8));
		static constexpr vae::GlobalEventHandle Ambience2 = 1 | (1 << (sizeof(vae::EventHandle) * 8));
		static constexpr vae::GlobalEventHandle Ambience3 = 2 | (1 << (sizeof(vae::EventHandle) * 8));
		static constexpr vae::GlobalEventHandle Ambience4 = 3 | (1 << (sizeof(vae::EventHandle) * 8));
		static constexpr vae::GlobalEventHandle Music = 4 | (1 << (sizeof(vae::EventHandle) * 8));
		static constexpr vae::GlobalEventHandle MusicStop = 5 | (1 << (sizeof(vae::EventHandle) * 8));

	};
	
	struct Bank1 {
		static constexpr vae::BankHandle id = 0;
		static constexpr vae::GlobalEventHandle JumpRand = 0 | (0 << (sizeof(vae::EventHandle) * 8));
		static constexpr vae::GlobalEventHandle Jump1 = 1 | (0 << (sizeof(vae::EventHandle) * 8));
		static constexpr vae::GlobalEventHandle Jump2 = 2 | (0 << (sizeof(vae::EventHandle) * 8));
		static constexpr vae::GlobalEventHandle Jump3 = 3 | (0 << (sizeof(vae::EventHandle) * 8));
		static constexpr vae::GlobalEventHandle Denied = 4 | (0 << (sizeof(vae::EventHandle) * 8));
		static constexpr vae::GlobalEventHandle ShortSineLoop = 5 | (0 << (sizeof(vae::EventHandle) * 8));
		static constexpr vae::GlobalEventHandle LongSineSelfTirgger = 6 | (0 << (sizeof(vae::EventHandle) * 8));
		static constexpr vae::GlobalEventHandle StepRand = 7 | (0 << (sizeof(vae::EventHandle) * 8));
		static constexpr vae::GlobalEventHandle Step1 = 8 | (0 << (sizeof(vae::EventHandle) * 8));
		static constexpr vae::GlobalEventHandle Step2 = 9 | (0 << (sizeof(vae::EventHandle) * 8));
		static constexpr vae::GlobalEventHandle Step3 = 10 | (0 << (sizeof(vae::EventHandle) * 8));

	};
	
} // namespace vaeb
#endif // _VAE_GEN_EVENT_NAMES
