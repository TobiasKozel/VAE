/**
 * @file vae.hpp
 * @author Tobias Kozel (t.kozel@pm.me)
 * @brief Contains all public API types for VAE
 * @version 0.1
 * @date 2021-11-27
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef _VAE_API
#define _VAE_API

#include "./vae_enums.hpp"
#include "./vae_type_defs.hpp"
#include "./vae_structs.hpp"

// see https://semver.org/
#define VAE_VERSION_MAJOR 0		//< for incompatible API changes
#define VAE_VERSION_MINOR 0		//< for adding functionality in a backwards-compatible manner
#define VAE_VERSION_PATCH 1		//< for backwards-compatible bug fixes

/**
 * @brief Contains generated Event ids using generate_bank_defines.py
 * @details Header can be included and used for autocompletion when calling
 * vae::EnginePimpl::fireGlobalEvent or vae::core::Engine::fireGlobalEvent
 */
namespace vaeb { }

/**
 * @brief Contains Typedefinitions and basic structures use by the public API and internally
 * @details The types used internally like handles or static settings can be changed, but vae::EnginePimpl needs to be recompiled.
 */
namespace vae {

	/**
	 * @brief Since 0 is a valid handle, these are used to identify invalid ones.
	 * Seems a little odd but means they can be used to direcly address array elements.
	 */
	constexpr EventHandle InvalidEventHandle		= ~0;
	constexpr SourceHandle InvalidSourceHandle		= ~0;
	constexpr BankHandle InvalidBankHandle			= ~0;
	constexpr MixerHandle InvalidMixerHandle		= ~0;
	constexpr ListenerHandle InvalidListenerHandle	= ~0;
	constexpr ListenerHandle AllListeners			= (~0) - 1; ///< Will address all listeners
	constexpr EmitterHandle InvalidEmitterHandle	= ~0;
	constexpr GlobalEventHandle InvalidGlobalEventHandle
		= InvalidEventHandle | (InvalidBankHandle << (sizeof(EventHandle) * 8));

	static_assert(
		(sizeof(BankHandle) + sizeof(EventHandle)) <= sizeof(GlobalEventHandle),
		"BankHandle combined with EventHandle needs to fit in GlobalEventHandle"
	);


	/**
	 * @brief Contains some fundamental Configuration needed at compile time
	 * Dynamic settings are contained in the EngineSettings struct above.
	 * @details Changes made here need recompilation
	 */
	namespace StaticConfig {
		/**
		 * @brief Maximum channel count used to pre allocate buffers
		 * @attention 5.1 Suround needs six channels but hasn't been testet since the reverb effect is only stereo
		 */
		constexpr unsigned char MaxChannels = 2;

		/**
		 * @brief Maximum block size
		 * @details Used to preallocate buffers for mixers and dsp.
		 * Higher values need more memory might play better with instruction caches
		 * but uses more memory.
		 */
		constexpr Size MaxBlock = 512;

		/**
		 * @brief How many Samples to prefetch for streaming sources
		 * @attention TODO no streaming for now. might even be a runtime settings
		 */
		constexpr Size StreamPrefetch = 1024 * 8;

		/**
		 * @brief How many listeners can observe 3D voices
		 * @see vae::core::Listener::Listeners
		 */
		constexpr Size MaxListeners = 4;

		/**
		 * @brief How many effects a mixer channel can process
		 * @see vae::core::Mixer::effects
		 */
		constexpr Size MaxMixerEffects = 4;

		constexpr Size MaxEffectsParameter = 4;

		/**
		 * @brief How many chained events can fit in chain_events on the core::Event structure
		 * @see vae::core::Event::chained_events
		 */
		constexpr Size MaxChainedEvents = 4;

		/**
		 * @brief Minimum volume before sounds will skip rendering
		 */
		constexpr Sample MinVolume  = 0.01f;

		/**
		 * @brief Placement of the speakers around the listener used for SPCAP panning
		 * @details Distance to the listener is not taken into account.
		 * The subwoofer has no placement.
		 */
		namespace Speakers {
			constexpr Vector3 center		= {  0, 0, -1 };	///< Used for mono and souround setups (except quadrophonic)

			constexpr Vector3 left			= { -1, 0,  0 };	///< Used in 7.1 and Headphones
			constexpr Vector3 right			= { +1, 0,  0 };	///< Used in 7.1 and Headphones

			constexpr Vector3 frontLeft		= { -1, 0, +1 };	///< Stereo and suround setups @attention this should be negative but isn't, needs some investigation
			constexpr Vector3 frontRight	= { +1, 0, +1 };	///< Stereo and suround setups

			constexpr Vector3 rearLeft		= { -1, 0, -1 };	///< Sourund setups
			constexpr Vector3 rearRight		= { +1, 0, -1 };	///< Sourund setups
		}
	}
} // namespace vae

#endif // _VAE_API
