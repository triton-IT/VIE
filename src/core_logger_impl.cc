#include "core_logger_impl.h"

namespace live::tritone::vie::core::debug {
	live::tritone::vie::debug::Logger* logger = nullptr;

	ILogger* ILogger::getInstance() {
		if(logger == nullptr)
		{
			logger = new live::tritone::vie::debug::Logger();
			logger->open("vie.log", true);
		}
		return logger;
	}
}

namespace live::tritone::vie::debug {
	void Logger::open(const std::string& file_name, const bool auto_flush) {
		//TODO: Don't have right to write to VST folder. So choose the right log solution
		log_file_.open(std::string("C:\\tmp\\") + file_name, std::ios::out);
		auto_flush_ = auto_flush;
	}

	void Logger::close() {
		log_file_.close();
	}

	void Logger::trace(const std::string& log) {
		write(log);
	}

	void Logger::debug(const std::string& log) {
		write(log);
	}

	void Logger::warning(const std::string& log) {
		write(log);
	}

	void Logger::info(const std::string& log) {
		write(log);
	}

	void Logger::error(const std::string& log) {
		write(log);
	}

	void Logger::write(const std::string& log) {
		log_file_ << log << std::endl;
		if (auto_flush_) {
			log_file_.flush();
		}
	}
}
