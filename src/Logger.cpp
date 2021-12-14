#include "Application.h"

#include <iostream>
#include <fstream>
#include <string>

#include "Application.h"

namespace live::tritone::vie::utils {
	void Logger::open(std::string fileName, bool autoFlush) {
		//TODO: Don't have right to write to VST folder. So choose the right log solution
		logFile_.open(std::string("D:\\") + fileName, std::ios::out);
		autoFlush_ = autoFlush;
		write("File opened");
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