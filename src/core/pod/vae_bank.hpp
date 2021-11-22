#ifndef _VAE_BANK
#define _VAE_BANK

#include <fstream>
#include <string>
#include <iostream>
#include <vector>

#include "../../../external/json.hpp"

#include "../vae_types.hpp"
#include "./vae_source.hpp"
#include "./vae_event.hpp"

namespace vae { namespace core {
	struct Bank {
		struct Loader;					//
		BankHandle id;					// Not used
		std::string name;				// Name of the bank
		std::string path;				// Path to the bank definition file
		std::vector<Source> sources;	// Audio sources defined
		std::vector<Event> events;		// Events defined
	};


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

	struct Bank::Loader {
		Result operator()(const char* path, Bank& bank) {
			std::string p = path;
			p += "/bank.json";
			std::ifstream file(p);
			if (!file.is_open()) { return Result::FileNotFound; }
			// std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
			auto data = nlohmann::json::parse(file);

			bank.name = data["name"];
			bank.id = data["id"];
			bank.path = path;

			auto sources = data["sources"];
			if (!sources.empty()) {
				bank.sources.resize(sources.size());
				for (auto& i : sources) {
					Source s;
					s.id		= i["id"];
					if (sources.size() <= s.id) {
						return Result::BankFormatError;
					}
					s.name		= i["name"];
					s.path		= i["path"];
					s.format	= i["format"];
					s.type		= i["type"];
					s.channels	= i["channels"];
					s.length	= i["length"];
					s.rate		= i["rate"];
					bank.sources[s.id] = s;
				}
			}
			auto events = data["events"];
			if (!events.empty()) {
				bank.events.resize(events.size());
				for (auto& i : events) {
					Event e;
					e.id		= i["id"];
					if (events.size() <= e.id) {
						return Result::BankFormatError;
					}
					e.name		= i["name"];
					e.type		= i["type"];

					auto trigSources = i["sources"];
					e.sources.reserve(trigSources.size());
					for (auto& s : trigSources) {
						e.sources.push_back(s);
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

					bank.events[e.id] = e;
				}
			}

			return Result::Success;
		}
	};
} } // namespace vae::vore

#endif // _VAE_BANK
