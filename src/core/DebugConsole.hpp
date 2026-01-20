#ifndef DEBUG_CONSOLE_HPP
#define DEBUG_CONSOLE_HPP

#include <mutex>
#include <string>
#include <vector>

class DebugConsole {
  private:
    std::vector<std::string> Logs;
    mutable std::mutex mtx;

  public:
    DebugConsole() = default;
    void Log(const std::string &log);

    std::string OneString();
    std::string LastLog();
    void clear();
};

#endif
