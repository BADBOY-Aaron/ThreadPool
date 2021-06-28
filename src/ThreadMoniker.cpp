
#include <atomic>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <thread>
#include <unordered_map>

#include "ThreadMoniker.h"


namespace utils {
namespace logger {

/// Counter to generate (small) unique thread monikers.
static std::atomic<int> g_nextThreadMoniker(1);

ThreadMoniker::ThreadMoniker(const std::string& moniker) : m_moniker{moniker.empty() ? generateMoniker() : moniker} {
}

std::string ThreadMoniker::generateMoniker() {
    std::ostringstream stream;
    stream << std::setw(3) << std::hex << std::right << g_nextThreadMoniker++;
    return stream.str();
}

const ThreadMoniker& ThreadMoniker::getMonikerObjectFromMap(const std::string& moniker) {
    /// Map each thread to a moniker.
    static std::unordered_map<std::thread::id, ThreadMoniker> threadMonikers;
    /// Map each moniker to a thread.
    static std::unordered_map<std::string, std::thread::id> monikerThreads;
    /// Lock used to synchronize access to the local maps.
    static std::mutex monikerMutex;

    std::lock_guard<std::mutex> lock{monikerMutex};
    auto id = std::this_thread::get_id();
    auto entry = threadMonikers.find(id);
    if (entry == threadMonikers.end()) {
        auto oldEntry = monikerThreads.find(moniker);
        if (oldEntry != monikerThreads.end()) {
            threadMonikers.erase(oldEntry->second);
        }
        auto& object = threadMonikers.emplace(std::make_pair(id, ThreadMoniker(moniker))).first->second;
        monikerThreads[object.m_moniker] = id;
        return object;
    }
    return entry->second;
}

}  // namespace logger
}  // namespace utils
