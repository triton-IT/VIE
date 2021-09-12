#pragma once

#include <iostream>
#include <fstream>
#include <string>

namespace live::tritone::vie::utils {
	class Logger {
	public:		
		void open(std::string fileName, bool autoFlush);

		void close();

		void write(std::string log);

	private:
		std::fstream logFile_;
		bool autoFlush_;
	};
}

