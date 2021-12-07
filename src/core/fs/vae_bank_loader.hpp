#ifndef _VAE_BANK_LOADER
#define _VAE_BANK_LOADER

#include "../vae_types.hpp"
#include "../vae_config.hpp"
#include "../vae_util.hpp"
#include "../pod/vae_source.hpp"
#include "../pod/vae_event.hpp"
#include "../pod/vae_bank.hpp"

#include "./vae_source_loader.hpp"

#include <fstream>
#include "../../../external/json.hpp"

namespace vae { namespace core {
	struct BankLoader {
		/**
		 * @brief Load a bank.json and all wav files referenced in it.
		 * @param path The path to the bank folder. Folder must contain a bank.json. Wav files are relative to this path
		 * @param bank The bank object to populae
		 * @return Result
		 */
		static Result load(const char* path, const char* rootPath, Bank& bank) {
			VAE_PROFILER_SCOPE
			/**
			 *					Open file and decode json
			 */
			std::string folder = path;
			folder = rootPath + folder + "/";
			std::string json = folder + "bank.json";
			std::ifstream file(json);

			if (!file.is_open()) { return Result::FileOpenError; }

			auto data = nlohmann::json::parse(file);

			bank.name	= data["name"];
			bank.id		= data["id"];
			bank.path	= path;

			/**
			 * 			Deserialize Source and preload signal data
			 */
			auto sources = data["sources"];
			if (!sources.empty()) {
				bank.sources.resize(sources.size());
				for (auto& i : sources) {
					SourceHandle id = i["id"];

					if (sources.size() <= id) { return Result::BankFormatIndexError; }

					Source& s = bank.sources[id];
					s.id		= id;
					s.name		= i["name"];
					s.path		= i["path"];
					s.gain		= i["gain"];

					std::string format	= i["format"];
					std::string	type	= i["type"];
					s.flags[Source::Flags::generator]	= type == "generator";
					s.flags[Source::Flags::preload]		= type == "preload";
					s.flags[Source::Flags::stream]		= type == "stream";
					s.flags[Source::Flags::vorbis]	= format == "vorbis";
					s.flags[Source::Flags::wav]		= format == "wav";

					auto result = SourceLoader::load(s, folder);
					if (result != Result::Success) {
						VAE_ERROR("Failed to load source %s", s.path.c_str())
					}
				}
			}

			/**
			 * 			Deserialize Mixers. Don't initialize anything yet
			 */
			auto mixers = data["mixers"];
			if (!mixers.empty()) {
				const auto mixerCount = mixers.size();
				bank.mixers.resize(mixerCount);
				for (auto& i : mixers) {
					MixerHandle id = i["id"];

					if (mixerCount <= id) {
						VAE_ERROR("Mixer %i:%i id out of bounds.", id, bank.id);
						return Result::BankFormatIndexError;
					}

					auto& m = bank.mixers[id];
					m.parent = i["parent"];

					if (m.id != Mixer::MasterMixerHandle && m.id <= m.parent) {
						// Mixer can only write to mixers with lower ids than themselves
						// this avoids recursion and makes mixing easier
						VAE_ERROR("Mixer %i:%i tried to mix to %i. ", id, bank.id, m.parent);
						return Result::BankFormatBadMixHirarchy;
					}

					m.id 			= id;
					m.name			= i["name"];
					m.gain			= i["gain"];
					auto effects	= i["effects"];
					for (size_t j = 0; j < Config::MaxMixerEffects; j++) {
						m.effects[j] = { };
					}
				}
			}

			auto events = data["events"];
			if (!events.empty()) {
				bank.events.resize(events.size());
				for (auto& i : events) {
					EventHandle id = i["id"];

					if (events.size() <= id) {
						VAE_ERROR("Event %i:%i id out of bounds.", id, bank.id)
						return Result::BankFormatIndexError;
					}

					Event& e = bank.events[id];

					e.id	= id;
					e.name	= i["name"];

					std::string type = i["type"];
					e.flags[size_t(Event::Flags::random)] 		= type == "start_rand";
					e.flags[size_t(Event::Flags::start)]		= type == "start" || e.flags[size_t(Event::Flags::random)];
					e.flags[size_t(Event::Flags::stop)]			= type == "stop";
					e.flags[size_t(Event::Flags::emit)]			= type == "emit";
					e.flags[size_t(Event::Flags::force_mixer)]	= bool(i["force_mixer"]);
					e.flags[size_t(Event::Flags::HRTF)] 		= bool(i["hrtf"]);

					if (i["source"].is_null()) {
						e.source = InvalidSourceHandle;
					} else {
						e.source = i["source"];
					}

					auto onStart = i["on_start"];
					if (Config::MaxChainedEvents < onStart.size()) {
						VAE_ERROR("Event %i:%i has too many chained on_start events.", id, bank.id)
						return Result::TooManyRecords;
					}
					for (size_t j = 0; j < onStart.size(); j++) {
						e.on_start[j] = onStart[j];
					}

					auto onEnd = i["on_end"];
					if (Config::MaxChainedEvents < onEnd.size()) {
						VAE_ERROR("Event %i:%i has too many chained on_end events.", id, bank.id)
						return Result::TooManyRecords;
					}
					for (size_t j = 0; j < onEnd.size(); j++) {
						e.on_end[j] = onEnd[j];
					}

					e.mixer = i["mixer"];
				}
			}
			return Result::Success;
		}
	};
} } // namespace vae::vore

#endif // _VAE_BANK_LOADER
