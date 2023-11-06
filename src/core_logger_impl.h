#ifndef VIE_CORE_LOGGER_IMPL_H_
#define VIE_CORE_LOGGER_IMPL_H_

#include <fstream>
#include <string>

#include "logger.h"

namespace live::tritone::vie::debug
{
  class Logger : public live::tritone::vie::core::debug::ILogger
  {
  public:
    void open(const std::string& file_name, const bool auto_flush);
    void close();

    void trace(const std::string &log);
    void debug(const std::string &log);
    void warning(const std::string &log);
    void info(const std::string &log);
    void error(const std::string &log);

  private:
		std::fstream log_file_;
    bool auto_flush_;

    void write(const std::string &log);
  };
} // namespace

#endif // VIE_CORE_LOGGER_IMPL_H_
