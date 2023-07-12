#pragma once

#include <spdlog/spdlog.h>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

constexpr const char* log_path = "log/kyse-debug.log";

class Log {
public:
	inline static std::shared_ptr<spdlog::logger> logger;

	static void initialize() {
		std::vector<spdlog::sink_ptr> logSinks;
		auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		sink->set_color(spdlog::level::trace, 3); // blue
		sink->set_color(spdlog::level::info, 7); // white
		logSinks.emplace_back(sink);
		
		logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_path, true));

		logSinks[0]->set_pattern("%^[%T] %n: %v%$");
		logSinks[1]->set_pattern("[%T] [%l] %n: %v");

		logger = std::make_shared<spdlog::logger>("KYSE", begin(logSinks), end(logSinks));
		spdlog::register_logger(logger);
		logger->set_level(spdlog::level::trace);
		logger->flush_on(spdlog::level::trace);
	}

	// restatement of spdlog for API syntax purposes

	template<class... Args>
	static void trace(const spdlog::format_string_t<Args...>& fmt, Args&&... args) {
		logger->trace( fmt, std::forward<Args>(args)... );
	}
	template<class... Args>
	static void info(const spdlog::format_string_t<Args...>& fmt, Args&&... args) {
		logger->info( fmt, std::forward<Args>(args)... );
	}
	template<class... Args>
	static void warn(const spdlog::format_string_t<Args...>& fmt, Args&&... args) {
		logger->warn( fmt, std::forward<Args>(args)... );
	}
	template<class... Args>
	static void error(const spdlog::format_string_t<Args...>& fmt, Args&&... args) {
		logger->error( fmt, std::forward<Args>(args)... );
	}
	template<class... Args>
	static void critical(const spdlog::format_string_t<Args...>& fmt, Args&&... args) {
		logger->critical( fmt, std::forward<Args>(args)... );
	}

    template<typename T>
	static void trace(const T& msg) { logger->trace( msg ); }
    template<typename T>
	static void info(const T& msg) { logger->info( msg ); }
    template<typename T>
	static void warn(const T& msg) { logger->warn( msg ); }
    template<typename T>
	static void error(const T& msg) { logger->error( msg ); }
    template<typename T>
	static void critical(const T& msg) { logger->critical( msg ); }
};