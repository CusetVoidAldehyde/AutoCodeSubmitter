#pragma once
#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include <ctime>
#include <chrono>
#include <string>
#include <thread>
#include <atomic>

typedef std::chrono::system_clock Clock;
typedef std::chrono::duration<long long, std::micro> Duration;
typedef std::chrono::time_point<Clock, Duration> Time;

template <typename FromDuration>
inline Time time_cast(std::chrono::time_point<Clock, FromDuration> const& tp)
{
    return std::chrono::time_point_cast<Duration, Clock> (tp);
}

inline Time now()
{
    return time_cast(Clock::now());
}

inline Time from_time_t(time_t t_time)
{
    return time_cast(Clock::from_time_t(t_time));
}

struct FileInfo
{
    Time mtime;
    off_t size;
};

class FileWatcher
{
public:
    FileWatcher(const std::string& file, unsigned int millis);
    ~FileWatcher();

protected:
    void run();

private:
    bool checkForFileModification();
    void updateLastModInfo();
    bool getFileInfo(FileInfo* fi, const std::string& name);

private:
    FileInfo m_lastFileInfo;
    std::string m_file;
    unsigned int const m_waitMillis;
    std::atomic_bool m_stopped;
    std::thread m_thread;
};

#endif // FILEWATCHER_H