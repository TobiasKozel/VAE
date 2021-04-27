#ifndef VAE_API_EMITTER
#define VAE_API_EMITTER

namespace VAE {
	namespace Impl {
		class EmitterImpl;
	}

	class Emitter {
		Impl::EmitterImpl* mPimpl = nullptr;

	public:
		Emitter() = delete;

		Emitter(Impl::EmitterImpl* pimpl);



		void destroy();
	};
}

#endif // VAE_API_EMITTER
