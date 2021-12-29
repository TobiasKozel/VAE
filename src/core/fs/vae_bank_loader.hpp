#ifndef _VAE_BANK_LOADER
#define _VAE_BANK_LOADER

#include "../vae_types.hpp"
#include "../vae_config.hpp"
#include "../vae_util.hpp"
#include "../pod/vae_source.hpp"
#include "../pod/vae_event.hpp"
#include "../pod/vae_bank.hpp"

#include "./vae_source_loader.hpp"
#include "../../wrapped/vae_fs.hpp"

#ifndef _JSON_H
	#include "../../../external/headeronly/json.h"
	#include "../../../external/headeronly/json.c"
#endif


namespace vae { namespace core {
	class BankLoader {
		static void* allocate(size_t size, int zero, void* context) {
			memory::AllocatorFS<char> allocator;
			void* ptr = reinterpret_cast<void*>(allocator.allocate(size));
			if (zero) {
				memset(ptr, 0, size);
			}
			return ptr;
		}

		static void deallocate(void* ptr, void* context) {
			memory::AllocatorFS<char> allocator;
			allocator.deallocate(reinterpret_cast<char*>(ptr), 0);
		}

		SourceLoader mSourceLoader;
	public:
		/**
		 * @brief Load a bank.json and all wav files referenced in it.
		 * @param path The path to the bank folder. Folder must contain a bank.json. Wav files are relative to this path
		 * @param bank The bank object to populae
		 * @return Result
		 */
		Result load(const char* path, Size length, const char* rootPath, Bank& bank) {
			VAE_PROFILER_SCOPE()

			/**
			 *					Open file and decode json
			 */
			PathString folder;			// Folder to the bank
			const char* encoded = path;	// The plain json text

			String jsonText;

			if (length == 0) { // length 0 indicates the file is on disk
				VAE_DEBUG("Started loading bank %s", path)
				VAE_PROFILER_SCOPE_NAMED("BankFile IO")
				folder = rootPath;
				folder.append(path);
				folder.append("/");
				PathString json = folder;
				json.append("bank.json");
				fs::File file(json.c_str());
				jsonText.resize(file.size());
				if (!file.readAll(jsonText.data())) {
					VAE_DEBUG("Failed to read file")
					return Result::FileOpenError;
				}
				length = jsonText.size();
				encoded = jsonText.c_str();
			}



			json_settings settings = { };
			settings.mem_alloc = allocate;
			settings.mem_free = deallocate;

			json_value* json;
			{
				VAE_PROFILER_SCOPE_NAMED("BankFile Parsing")
				json = json_parse_ex(&settings, encoded, length, 0);
			}
			if (json == nullptr) { return Result::BankFormatError; }
			json_value& data = (*json);

			bank.name	= (const char*) data["name"];
			bank.id		= (json_int_t) data["id"];
			bank.path	= path;

			/**
			 * 			Deserialize Source and preload signal data
			 */
			auto sources = data["sources"];
			if (sources.type == json_array) {
				VAE_PROFILER_SCOPE_NAMED("Deserialize sources")
				bank.sources.resize(sources.u.array.length);
				for (auto& iraw : sources.u.array) {
					auto& i = *iraw;
					SourceHandle id = (json_int_t) i["id"];

					if (sources.u.array.length <= id) { return Result::BankFormatIndexError; }

					String asd	= (const char*) i["name"];

					Source& s = bank.sources[id];
					if (s.id != InvalidSourceHandle) {
						VAE_ERROR("Duplicate Source id %i in bank %i", s.id, bank.id)
						return Result::BankFormatDuplicateIndex;
					}
					s.id		= id;
					s.name		= (const char*) i["name"];
					s.path		= (const char*) i["path"];

					if (i["gain"].type == json_double)		s.gain		= (double) i["gain"];
					if (i["resample"].type == json_boolean)	s.resample	= i["resample"];
					if (i["stream"].type  == json_boolean)	s.stream	= i["stream"];

					IdString format = { i["format"] };
					if (format == "wav")		s.format = Source::Format::wav;
					if (format == "ogg")		s.format = Source::Format::ogg;
					if (format == "generator")	s.format = Source::Format::generator;

					auto result = mSourceLoader.load(s, folder.c_str());
					if (result != Result::Success) {
						VAE_ERROR("Failed to load source %s", s.path.c_str())
					}
				}
			}

			/**
			 * 			Deserialize Mixers. Don't initialize anything yet
			 */
			auto mixers = data["mixers"];
			if (mixers.type == json_array) {
				VAE_PROFILER_SCOPE_NAMED("Deserialize mixers")
				bank.mixers.resize(mixers.u.array.length);
				for (auto& iraw : mixers.u.array) {
					auto& i = *iraw;

					MixerHandle id = (json_int_t) i["id"];

					if (mixers.u.array.length <= id) {
						VAE_ERROR("Mixer %i:%i id out of bounds.", id, bank.id);
						return Result::BankFormatIndexError;
					}

					auto& m = bank.mixers[id];

					if (m.id != InvalidMixerHandle) {
						VAE_ERROR("Duplicate Mixer id %i in bank %i", m.id, bank.id)
						return Result::BankFormatDuplicateIndex;
					}

					m.name	= (const char*) i["name"];
					m.id 	= id;

					if (i["parent"].type == json_integer)	m.parent = (json_int_t) i["parent"];
					if (i["gain"].type == json_double)		m.gain = (double) i["gain"];

					if (m.id != Mixer::MasterMixerHandle && m.id <= m.parent) {
						// Mixer can only write to mixers with lower ids than themselves
						// this avoids recursion and makes mixing easier
						VAE_ERROR("Mixer %i:%i tried to mix to %i. ", id, bank.id, m.parent);
						return Result::BankFormatBadMixHirarchy;
					}

					auto effects = i["effects"];
					if (effects.type != json_array || effects.u.array.length == 0) { continue; }


					for (auto& iraw : effects.u.array) {
						auto& i = *iraw;

						json_int_t index = i["index"];
						auto& e	= m.effects[index];
						e.bypassed = i["bypassed"];
						e.name	= i["name"];

						int paramIndex = 0;
						for (auto& j : i["parameters"].u.array) {

							e.parameters[paramIndex].name = (const char*) (*j)[0];
							e.parameters[paramIndex].value = (double) (*j)[1];
							paramIndex++;
						}
					}
				}
			}

			auto events = data["events"];
			{
				VAE_PROFILER_SCOPE_NAMED("Deserialize events")
				bank.events.resize(events.u.array.length);
				for (auto& iraw : events.u.array) {
					auto& i = *iraw;
					EventHandle id = (json_int_t) i["id"];

					if (events.u.array.length <= id) {
						VAE_ERROR("Event %i:%i id out of bounds.", id, bank.id)
						return Result::BankFormatIndexError;
					}

					Event& e = bank.events[id];

					if (e.id != InvalidEventHandle) {
						VAE_ERROR("Duplicate Event id %i in bank %i", e.id, bank.id)
						return Result::BankFormatDuplicateIndex;
					}

					e.id	= id;
					e.name	= (const char*) i["name"];

					IdString action = { i["action"] };
					if (action == "start_rand")		e.action		= Event::Action::random;
					if (action == "emit")			e.action		= Event::Action::emit;
					if (action == "stop")			e.action		= Event::Action::stop;
					if (action == "start")			e.action		= Event::Action::start;

					if (i["force_mixer"].type == json_boolean)	e.force_mixer	= i["force_mixer"];
					if (i["spatial"].type == json_boolean)		e.spatial		= i["spatial"];
					if (i["critical"].type == json_boolean)		e.critical		= i["critical"];
					if (i["loop"].type == json_boolean)			e.loop			= i["loop"];
					if (i["HRTF"].type == json_boolean)			e.HRTF			= i["hrtf"];
					if (i["attenuate"].type == json_boolean)	e.attenuate		= i["attenuate"];
					if (i["mixer"].type == json_integer)		e.mixer			= (json_int_t) i["mixer"];
					if (i["gain"].type == json_double)			e.gain			= (double) i["gain"];
					if (i["source"].type == json_integer)		e.source 		= (json_int_t) i["source"];

					if (i["on_start"].type == json_array) {
						auto onStart = i["on_start"].u.array;
						if (StaticConfig::MaxChainedEvents < onStart.length) {
							VAE_ERROR("Event %i:%i has too many chained on_start events.", id, bank.id)
							return Result::TooManyRecords;
						}
						for (size_t j = 0; j < onStart.length; j++) {
							e.on_start[j] = (json_int_t) (*onStart.values[j]);
						}
					}

					if (i["on_end"].type == json_array) {
						auto onEnd = i["on_end"].u.array;
						if (StaticConfig::MaxChainedEvents < onEnd.length) {
							VAE_ERROR("Event %i:%i has too many chained on_end events.", id, bank.id)
							return Result::TooManyRecords;
						}
						for (size_t j = 0; j < onEnd.length; j++) {
							e.on_end[j] = (json_int_t) (*onEnd.values[j]);
						}
					}
				}
			}

			json_value_free_ex(&settings, json);
			return Result::Success;
		}
	};
} } // namespace vae::vore

#endif // _VAE_BANK_LOADER
