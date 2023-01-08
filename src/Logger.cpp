#include "application.hpp"

#include <iostream>
#include <fstream>
#include <string>

namespace live::tritone::vie::debug {
	void logger::open(const std::string& file_name, const bool auto_flush) {
		//TODO: Don't have right to write to VST folder. So choose the right log solution
		log_file_.open(std::string("C:\\tmp\\") + file_name, std::ios::out);
		auto_flush_ = auto_flush;
	}

	void logger::close() {
		log_file_.close();
	}

	void logger::write(const std::string& log) {
		log_file_ << log << std::endl;
		if (auto_flush_) {
			log_file_.flush();
		}
	}
}