#ifndef _VAE_GEN_EVENT_NAMES
#define _VAE_GEN_EVENT_NAMES
namespace vaeb { 
struct Bank1 {
	static constexpr vae::BankHandle id = 0;
	static constexpr vae::GlobalEventHandle Event1 = 0 | (0 << (sizeof(vae::EventHandle) * 8));
	static constexpr vae::GlobalEventHandle Event2 = 1 | (0 << (sizeof(vae::EventHandle) * 8));
	static constexpr vae::GlobalEventHandle StartEvent3 = 2 | (0 << (sizeof(vae::EventHandle) * 8));
	static constexpr vae::GlobalEventHandle EndEvent4 = 3 | (0 << (sizeof(vae::EventHandle) * 8));
	static constexpr vae::GlobalEventHandle Event5 = 4 | (0 << (sizeof(vae::EventHandle) * 8));
};
} // namespace vaeb
#endif // _VAE_GEN_EVENT_NAMES
