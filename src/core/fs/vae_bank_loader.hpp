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
	NLOHMANN_JSON_SERIALIZE_ENUM(Event::EventType, {
		{Event::EventType::undefined, nullptr},
		{Event::EventType::start, "start"},
		{Event::EventType::stop, "stop"},
		{Event::EventType::emit, "emit"},
	})

	NLOHMANN_JSON_SERIALIZE_ENUM(Source::SourceType, {
		{Source::SourceType::undefined, nullptr},
		{Source::SourceType::generator, "generator"},
		{Source::SourceType::preload, "preload"},
		{Source::SourceType::stream, "stream"},
	})

	NLOHMANN_JSON_SERIALIZE_ENUM(Source::SourceFormat, {
		{Source::SourceFormat::undefined, nullptr},
		{Source::SourceFormat::wav, "wav"},
		{Source::SourceFormat::vorbis, "vorbis"},
	})

	struct BankLoader {
		/**
		 * @brief Load a bank.json and all wav files referenced in it.
		 * @param path The path to the bank folder. Folder must contain a bank.json. Wav files are relative to this path
		 * @param bank The bank object to populae
		 * @return Result
		 */
		static Result load(const char* path, Bank& bank) {
			/**
			 *					Open file and decode json
			 */
			std::string folder = path;
			folder += "/";
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
					s.format	= i["format"];
					s.type		= i["type"];
					auto result = SourceLoader::load(s, folder);
					if (result != Result::Success) {
						VAE_ERROR("Failed to load source %s", s.path.c_str());
					}
				}
			}

			/**
			 * 			Deserialize Mixers. Don't initialize anything yet
			 */
			auto mixers = data["mixers"];
			if (!mixers.empty()) {
				bank.mixers.resize(mixers.size());
				for (auto& i : mixers) {
					MixerHandle id = i["id"];

					if (mixers.size() <= id) { return Result::BankFormatIndexError; }

					auto& m = bank.mixers[id];
					if (m.id != Mixer::MasterMixerHandle && m.id <= m.parent) {
						// Mixer can only write to mixers with lower ids than themselves
						// this avoids recursion and makes mixing easier
						return Result::BankFormatBadMixHirarchy;
					}

					m.id 			= id;
					m.name			= i["name"];
					m.parent		= i["parent"];
					m.gain			= i["gain"];
					auto effects	= i["effects"];
					m.effects.reserve(effects.size());
					for (auto& j : effects) {
						m.effects.push_back({ });
					}
				}
			}

			auto events = data["events"];
			if (!events.empty()) {
				bank.events.resize(events.size());
				for (auto& i : events) {
					EventHandle id = i["id"];

					if (events.size() <= id) { return Result::BankFormatIndexError; }

					Event& e = bank.events[id];
					e.id			= id;
					e.name			= i["name"];
					e.type			= i["type"];
					e.force_mixer	= i["force_mixer"];

					if (i["source"].is_null()) {
						e.source = InvalidHandle;
					} else {
						e.source = i["source"];
					}

					auto onStart = i["on_start"];
					e.on_start.reserve(onStart.size());
					for (auto& j : onStart) {
						e.on_start.push_back(j);
					}

					auto onEnd = i["on_end"];
					e.on_end.reserve(onEnd.size());
					for (auto& j : onEnd) {
						e.on_end.push_back(j);
					}

					e.mixer = i["mixer"];
				}
			}
			return Result::Success;
		}
	};
} } // namespace vae::vore

#endif // _VAE_BANK_LOADER
