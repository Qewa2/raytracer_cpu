#include "DebugConsole.hpp"

#include <mutex>
#include <string>
#include <vector>

void DebugConsole::Log(const std::string &log) {
    std::lock_guard<std::mutex> lock(mtx);
    Logs.push_back(log);
}

std::string DebugConsole::OneString() {
    std::lock_guard<std::mutex> lock(mtx);
    if (!Logs.empty()) {
        std::string LogsString;
        for (auto &log : Logs) {
            LogsString += log;
            LogsString += '\n';
        }
        return LogsString;
    } else {
        return "";
    }
}

std::string DebugConsole::LastLog() {
    std::lock_guard<std::mutex> lock(mtx);
    if (!Logs.empty()) {
        return Logs.back();
    } else {
        return "";
    }
}

void DebugConsole::clear() { Logs.clear(); }
