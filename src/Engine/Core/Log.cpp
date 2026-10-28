#include "Core/Log.h"

#include <cstdarg>
#include <cstdio>

#include <SDL3/SDL_log.h>

namespace {

constexpr size_t kMaxEntries = 1000;
std::deque<Log::Entry> entries;

}

void Log::Write(Level level, const char* format, ...)
{
    char buffer[1024];

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    switch (level) {
    case Level::Info:    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "%s", buffer);  break;
    case Level::Warning: SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "%s", buffer);  break;
    case Level::Error:   SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", buffer); break;
    }

    entries.push_back({ level, buffer });
    if (entries.size() > kMaxEntries)
        entries.pop_front();
}

const std::deque<Log::Entry>& Log::Entries()
{
    return entries;
}

void Log::Clear()
{
    entries.clear();
}
