#pragma once

#include <format>
#include <iostream>

enum LogLevel {
    TRACE, DEBUG, INFO, WARN, ERROR, CRITICAL
};

constexpr const char *logLevelNames[6] = {
    "TRACE", "DEBUG", " INFO", " WARN", "ERROR", " CRIT"
};

constexpr const char *logLevelColors[6] = {
    "\e[0;34m", "\e[0;35m", "\e[0;37m", "\e[0;33m", "\e[0;91m", "\e[1;31m"
};

class Logger {
    LogLevel _level = TRACE;

    explicit Logger() = default;

    static std::string getCurrentDateTime() {
        const time_t now = time(nullptr);
        const tm time = *localtime(&now);
        char buffer[80];

        strftime(buffer, sizeof(buffer), "%Y-%m-%d.%X", &time);
        return buffer;
    }

    template<class... Args>
    void log(const LogLevel level, std::format_string<Args...> format, Args &&... args) const {
        std::ostream &os = level >= ERROR ? std::cerr : std::cout;

        if (level < _level) return;
        os << "[" << getCurrentDateTime() << "] ";
        os << logLevelColors[level] << logLevelNames[level] << "\e[0m" << " ";
        os << std::format(format, std::forward<Args>(args)...) << std::endl;
    }

public:
    static Logger &getInstance() {
        static Logger instance;

        return instance;
    }

    template<class... Args>
    void trace(std::format_string<Args...> format, Args &&... args) const {
        log(TRACE, format, std::forward<Args>(args)...);
    }

    template<class... Args>
    void debug(std::format_string<Args...> format, Args &&... args) const {
        log(DEBUG, format, std::forward<Args>(args)...);
    }

    template<class... Args>
    void info(std::format_string<Args...> format, Args &&... args) const {
        log(INFO, format, std::forward<Args>(args)...);
    }

    template<class... Args>
    void warn(std::format_string<Args...> format, Args &&... args) const {
        log(WARN, format, std::forward<Args>(args)...);
    }

    template<class... Args>
    void error(std::format_string<Args...> format, Args &&... args) const {
        log(ERROR, format, std::forward<Args>(args)...);
    }

    template<class... Args>
    void critical(std::format_string<Args...> format, Args &&... args) const {
        log(CRITICAL, format, std::forward<Args>(args)...);
    }
};
