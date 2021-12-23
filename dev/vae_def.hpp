#ifndef _VAE_GEN_EVENT_NAMES
#define _VAE_GEN_EVENT_NAMES
namespace vaeb { 
struct Bank1 {
	static constexpr vae::BankHandle id = 0;
	static constexpr vae::GlobalEventHandle JumpRand = 0 | (0 << (sizeof(vae::EventHandle) * 8));
	static constexpr vae::GlobalEventHandle Jump1 = 1 | (0 << (sizeof(vae::EventHandle) * 8));
	static constexpr vae::GlobalEventHandle Jump2 = 2 | (0 << (sizeof(vae::EventHandle) * 8));
	static constexpr vae::GlobalEventHandle Jump3 = 3 | (0 << (sizeof(vae::EventHandle) * 8));
	static constexpr vae::GlobalEventHandle Ambient = 4 | (0 << (sizeof(vae::EventHandle) * 8));
	static constexpr vae::GlobalEventHandle ShortSineLoop = 5 | (0 << (sizeof(vae::EventHandle) * 8));
};
} // namespace vaeb
#endif // _VAE_GEN_EVENT_NAMES
