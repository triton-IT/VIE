#pragma once
 
#include <fstream>
#include <string>

namespace live::tritone::vie::debug {
	class logger {
	public:
		void open(const std::string& file_name, bool auto_flush);

		void close();

		void write(const std::string& log);

	private:
		std::fstream log_file_;
		bool auto_flush_ = true;
	};
}
