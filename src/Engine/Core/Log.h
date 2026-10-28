#pragma once

#include <deque>
#include <string>

// Engine-wide logging. Messages go to the debug output (SDL log) and are kept in a bounded buffer that the editor console reads.
namespace Log {

enum class Level { Info, Warning, Error };

struct Entry {
    Level level;
    std::string text;
};

void Write(Level level, const char* format, ...);

const std::deque<Entry>& Entries();
void Clear();

}

#define LOG(...)         Log::Write(Log::Level::Info,    __VA_ARGS__)
#define LOG_WARNING(...) Log::Write(Log::Level::Warning, __VA_ARGS__)
#define LOG_ERROR(...)   Log::Write(Log::Level::Error,   __VA_ARGS__)
