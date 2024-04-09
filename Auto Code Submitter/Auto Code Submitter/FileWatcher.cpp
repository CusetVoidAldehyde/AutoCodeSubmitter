#include "FileWatcher.h"
#include "tchar.h"
#include "iostream"
#include <sys/stat.h>

FileWatcher::FileWatcher(const std::string& file, unsigned int millis)
    : m_file(file)
    , m_waitMillis(millis)
    , m_stopped(false)
    , m_thread(&FileWatcher::run, this)
{
    m_lastFileInfo.mtime = time_cast(Clock::now());
    m_lastFileInfo.size = 0;

    updateLastModInfo();
}

FileWatcher::~FileWatcher()
{
    m_stopped = true;
    if (m_thread.joinable())
    {
        m_thread.join();
    }
}

void FileWatcher::run()
{
    while (!m_stopped)
    {
        bool modified = checkForFileModification();
        if (modified)
        {
            updateLastModInfo();
            std::cout << "File changed" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(m_waitMillis));
    }
}

bool FileWatcher::checkForFileModification()
{
    FileInfo fi;

    if (!getFileInfo(&fi, m_file))
    {
        return false;
    }

    bool modified = fi.mtime > m_lastFileInfo.mtime
        || fi.size != m_lastFileInfo.size;


    return modified;
}

void FileWatcher::updateLastModInfo()
{
    FileInfo fi;

    if (getFileInfo(&fi, m_file))
    {
        m_lastFileInfo = fi;
    }
}

bool FileWatcher::getFileInfo(FileInfo* fi, const std::string& name)
{
    struct _stat fileStatus;
    if (_stat(name.c_str(), &fileStatus) == -1)
    {
        return false;
    }

    fi->mtime = from_time_t(fileStatus.st_mtime);
    fi->size = fileStatus.st_size;

    return true;
}