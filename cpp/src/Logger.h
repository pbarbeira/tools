//
// Created by pbarbeira on 06-04-2025.
//

#ifndef LOGGER_H
#define LOGGER_H

#include <memory>
#include <vector>
#include <chrono>

enum LogLevel{
    ERROR, WARNING, INFO, DEBUG
};

struct Log{
    LogLevel level{};
    time_t timestamp{};
    std::string message;
};

class Logger{
    std::ostream& _out;
    LogLevel _level;
    std::vector<std::unique_ptr<Log>> _logs;

    static std::string _buildLogMessage(const Log* log) {
        auto logTime = std::put_time(std::localtime(&log->timestamp), "%Y-%m-%d %H:%M:%S");
        std::string logLevel;
        switch (log->level) {
            case ERROR: logLevel = "ERROR"; break;
            case WARNING: logLevel = "WARNING"; break;
            case INFO: logLevel = "INFO"; break;
            case DEBUG: logLevel = "DEBUG"; break;
            default: throw std::runtime_error("Unknown log level");
        }
        std::stringstream ss;
        ss << "[" << logTime << "] - [" << logLevel << "]::" << log->message << "\n";
        return ss.str();
    }

    public:
        Logger() = delete;

        explicit Logger(std::ostream& out):
            _out(out), _level(LogLevel::DEBUG){}

        explicit Logger(std::ostream& out, const LogLevel& level):
            _out(out), _level(level) {}

        void log(const LogLevel level, const std::string& message){
            auto log = std::make_unique<Log>();
            log->level = level;
            log->message = message;
            log->timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            if (level <= _level) {
                _out << _buildLogMessage(log.get());
            }
            _logs.push_back(std::move(log));

        }

        std::vector<std::unique_ptr<Log>> getLogs(bool flush = false){
            auto logs = std::move(_logs);
            if (flush) {
                _logs.clear();
            }
            return std::move(logs);
        }

        void dump(bool flush = false) {
            for (const auto& log : _logs) {
                _out << _buildLogMessage(log.get());
            }
            if (flush){
                _logs.clear();
            }
        }
};

class ConsoleLogger :   public Logger {
    public:
        ConsoleLogger():
            Logger(std::cout, LogLevel::DEBUG) {}

        explicit ConsoleLogger(const LogLevel& level):
            Logger(std::cout, level) {}
};

class StringLogger : public Logger {
    std::stringstream _ss;
    public:
        StringLogger():
            Logger(_ss){}

        explicit StringLogger(const LogLevel& level):
            Logger(_ss, level){}

        explicit StringLogger(std::stringstream& ss):
            Logger(ss){}

        StringLogger(std::stringstream& ss, const LogLevel& level):
            Logger(ss, level){}

        std::string dump(const bool flush = false) {
            _ss.str("");
            Logger::dump(flush);
            std::string out = _ss.str();
            _ss.str("");
            return out;
        }
};

#endif //LOGGER_H
