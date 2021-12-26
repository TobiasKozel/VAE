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
#include "../../../external/headeronly/json.hpp"

namespace vae { namespace core {
	class BankLoader {
		SourceLoader mSourceLoader;
	public:
		/**
		 * @brief Load a bank.json and all wav files referenced in it.
		 * @param path The path to the bank folder. Folder must contain a bank.json. Wav files are relative to this path
		 * @param bank The bank object to populae
		 * @return Result
		 */
		Result load(const char* path, const char* rootPath, Bank& bank) {
			VAE_PROFILER_SCOPE()
			/**
			 *					Open file and decode json
			 */
			PathString folder = path;
			folder = rootPath + folder + "/";
			PathString json = folder + "bank.json";
			std::ifstream file(json);

			if (!file.is_open()) { return Result::FileOpenError; }

			auto data = nlohmann::basic_json<
				std::map, std::vector, std::string, bool, int, Size, Sample, AllocatorFS
			>::parse(file);

			bank.name	= data["name"];
			bank.id		= data["id"];
			bank.path	= path;

			/**
			 * 			Deserialize Source and preload signal data
			 */
			auto sources = data["sources"];
			if (!sources.empty()) {
				VAE_PROFILER_SCOPE_NAMED("Deserialize sources")
				bank.sources.resize(sources.size());
				for (auto& i : sources) {
					SourceHandle id = i["id"];

					if (sources.size() <= id) { return Result::BankFormatIndexError; }

					Source& s = bank.sources[id];
					if (s.id != InvalidSourceHandle) {
						VAE_ERROR("Duplicate Source id %i in bank %i", s.id, bank.id)
						return Result::BankFormatDuplicateIndex;
					}
					s.id		= id;
					s.name		= i["name"];
					s.path		= i["path"];

					if (i.contains("gain"))		s.gain		= i["gain"];
					if (i.contains("resample"))	s.resample	= (bool) i["resample"];
					if (i.contains("stream"))	s.stream	= (bool) i["stream"];

					PathString format = i["format"];
					if (format == "wav")		s.format = Source::Format::wav;
					if (format == "ogg")		s.format = Source::Format::ogg;
					if (format == "generator")	s.format = Source::Format::generator;

					auto result = mSourceLoader.load(s, folder);
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
				VAE_PROFILER_SCOPE_NAMED("Deserialize mixers")
				const auto mixerCount = mixers.size();
				bank.mixers.resize(mixerCount);
				for (auto& i : mixers) {
					MixerHandle id = i["id"];

					if (mixerCount <= id) {
						VAE_ERROR("Mixer %i:%i id out of bounds.", id, bank.id);
						return Result::BankFormatIndexError;
					}

					auto& m = bank.mixers[id];

					if (m.id != InvalidMixerHandle) {
						VAE_ERROR("Duplicate Mixer id %i in bank %i", m.id, bank.id)
						return Result::BankFormatDuplicateIndex;
					}

					m.name	= i["name"];
					m.id 	= id;

					if (i.contains("parent"))	m.parent = i["parent"];
					if (i.contains("gain"))		m.gain = i["gain"];

					if (m.id != Mixer::MasterMixerHandle && m.id <= m.parent) {
						// Mixer can only write to mixers with lower ids than themselves
						// this avoids recursion and makes mixing easier
						VAE_ERROR("Mixer %i:%i tried to mix to %i. ", id, bank.id, m.parent);
						return Result::BankFormatBadMixHirarchy;
					}

					if (!i.contains("effects")) { continue; }

					auto effects = i["effects"];

					for (auto& i : effects) {
						int index = i["index"];
						auto& e	= m.effects[index];
						e.bypassed = i["bypassed"];

						{
							std::string name = i["name"];
							e.name	= name.c_str();
						}

						int paramIndex = 0;
						for (auto& j : i["parameters"]) {
							std::string name = j[0];
							e.parameters[paramIndex].name = name.c_str();
							e.parameters[paramIndex].value = j[1];
							paramIndex++;
						}
					}
				}
			}

			auto events = data["events"];
			if (!events.empty()) {
				VAE_PROFILER_SCOPE_NAMED("Deserialize events")
				bank.events.resize(events.size());
				for (auto& i : events) {
					EventHandle id = i["id"];

					if (events.size() <= id) {
						VAE_ERROR("Event %i:%i id out of bounds.", id, bank.id)
						return Result::BankFormatIndexError;
					}

					Event& e = bank.events[id];

					if (e.id != InvalidEventHandle) {
						VAE_ERROR("Duplicate Event id %i in bank %i", e.id, bank.id)
						return Result::BankFormatDuplicateIndex;
					}

					e.id	= id;
					e.name	= i["name"];

					PathString action = i["action"];
					if (action == "start_rand")		e.action		= Event::Action::random;
					if (action == "emit")			e.action		= Event::Action::emit;
					if (action == "stop")			e.action		= Event::Action::stop;
					if (action == "start")			e.action		= Event::Action::start;

					if (i.contains("force_mixer"))	e.force_mixer	= (bool) i["force_mixer"];
					if (i.contains("spatial"))		e.spatial		= (bool) i["spatial"];
					if (i.contains("critical"))		e.critical		= (bool) i["critical"];
					if (i.contains("loop"))			e.loop			= (bool) i["loop"];
					if (i.contains("HRTF"))			e.HRTF			= (bool) i["hrtf"];
					if (i.contains("attenuate"))	e.attenuate		= (bool) i["attenuate"];
					if (i.contains("mixer"))		e.mixer			= i["mixer"];
					if (i.contains("gain"))			e.gain			= i["gain"];

					if (i.contains("source") && !i["source"].is_null()) {
						e.source = i["source"]; // source an also be null
					}

					if (i.contains("on_start")) {
						auto onStart = i["on_start"];
						if (StaticConfig::MaxChainedEvents < onStart.size()) {
							VAE_ERROR("Event %i:%i has too many chained on_start events.", id, bank.id)
							return Result::TooManyRecords;
						}
						for (size_t j = 0; j < onStart.size(); j++) {
							e.on_start[j] = onStart[j];
						}
					}

					if (i.contains("on_end")) {
						auto onEnd = i["on_end"];
						if (StaticConfig::MaxChainedEvents < onEnd.size()) {
							VAE_ERROR("Event %i:%i has too many chained on_end events.", id, bank.id)
							return Result::TooManyRecords;
						}
						for (size_t j = 0; j < onEnd.size(); j++) {
							e.on_end[j] = onEnd[j];
						}
					}

				}
			}
			return Result::Success;
		}
	};
} } // namespace vae::vore

#endif // _VAE_BANK_LOADER
