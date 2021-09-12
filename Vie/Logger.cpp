#pragma once

#include "Application.h"

#include <iostream>
#include <fstream>
#include <string>

#include "Application.h"

namespace live::tritone::vie::utils {
	void Logger::open(std::string fileName, bool autoFlush) {
		logFile_.open(contentPath + fileName, std::ios::app);
		autoFlush_ = autoFlush;
	}

	void Logger::close() {
		logFile_.close();
	}

	void Logger::write(std::string log) {
		logFile_ << log << std::endl;
		if (autoFlush_) {
			logFile_.flush();
		}
	}
}