#ifndef TKLBZ_LOG
#define TKLBZ_LOG

#include <fstream>

// TODO use wrapped mutex
#include <mutex>
#include <iostream>

// TODO throw out strings
#include <string>

#include "./TPrint.h"


namespace tklb {
	class Logger {
	public:
		enum Level {
			DEBUG = 0,
			INFO,
			WARN,
			ERROR,
			CRITICAL
		};

	private:
		std::ofstream mLogFile;
		std::mutex mWriteLock;
		Level mLevel = Level::DEBUG;

		static Logger& instance() {
			static Logger _instance;
			return _instance;
		}

		static void print(const char* msg, const Level level = Level::INFO) {
			auto& inst = instance();
			if (level < inst.mLevel) { return; }

			const char* prefix = nullptr;
			if (level == Level::DEBUG) { prefix = "DEBUG:"; }
			if (level == Level::INFO) { prefix = "INFO:"; }
			if (level == Level::WARN) { prefix = "WARN:"; }
			if (level == Level::ERROR) { prefix = "ERROR:"; }
			if (level == Level::CRITICAL) { prefix = "CRITICAL:"; }

			if (level > Level::WARN) {
				std::cerr << prefix << "\t" << msg << std::endl;
			} else {
				std::cout << prefix << "\t" << msg << std::endl;
			}

			if (!inst.mLogFile.is_open()) { return; }
			inst.mWriteLock.lock();
			inst.mLogFile << prefix << "\t" << msg << std::endl;
			inst.mLogFile.flush();
			inst.mWriteLock.unlock();
		}

		Logger() {
			mLogFile.open("log.txt", std::ofstream::trunc);
			if (!mLogFile.is_open()) {
				std::cerr << "ERROR:\tCould not open log file!" << std::endl;
			}
		}

		~Logger() {
			if (mLogFile.is_open()) {
				print("Shutting down...");
				mLogFile.close();
			}
		}

		Logger(const Logger&) = delete;
		Logger& operator = (const Logger&) = delete;

	public:

		static constexpr const char* devider = "-------------------------------------------------------";

		static void setLevel(Level loglevel) {
			instance().mLevel = loglevel;
		}


		static void debug(const std::string& msg) {
	#ifndef NDEBUG
			print(msg.c_str(), Level::DEBUG);
	#endif
		};

		static void info(const std::string& msg) {
			print(msg.c_str(), Level::INFO);
		};

		static void warn(const std::string& msg) {
			print(msg.c_str(), Level::WARN);
		};

		static void error(const std::string& msg) {
			print(msg.c_str(), Level::ERROR);
		};

		static void critical(const std::string& msg) {
			print(msg.c_str(), Level::CRITICAL);
		};
	};
}


#endif
