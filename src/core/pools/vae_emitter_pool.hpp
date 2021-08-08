#ifndef VAEX_EMITTER_POOL
#define VAEX_EMITTER_POOL

#include "../../../external/tklb/src/types/THeapBuffer.hpp"
#include "../vae_emitter.hpp"

namespace vae { namespace core {

	class HandleBuffer : public tklb::HeapBuffer<Emitter, true> {
		using Handle = unsigned int;
		using Size = tklb::HeapBuffer<Emitter, true>::Size;

		static constexpr int MaskSplit = 16; // at which bit to split the mask
		static constexpr Handle MaskIndex = (1 << (MaskSplit)) - 1;
		static constexpr Handle MaskId = ~MaskIndex;

		Handle mStart = 0;
		Size mLastFree = 0;

		Emitter* at(const Handle& handle) {
			const Handle id = handle & MaskId;
			const Handle index = handle & MaskIndex;
			if (index == mLastFree) { return nullptr; } // element deleted
			if (size() <= index) { return nullptr; } // out of range
			Emitter* element = data() + index;

			// element->
			return element;
		}
	};
} }

#endif // VAEX_EMITTER_POOL
