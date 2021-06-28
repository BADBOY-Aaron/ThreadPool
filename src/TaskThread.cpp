/*******************************************************************
 *	Copyright(c) 2021-2050  
 *  All rights reserved.
 *	
 *	FileName:
 *	Description:
 *	
 *	Author:
 *	Create time:
 ******************************************************************/

#include <chrono>
#include <iostream>
#include "ThreadMoniker.h"
#include "TaskThread.h"

/// String to identify log entries originating from this file.
static const std::string TAG("TaskThread");

namespace utils {
namespace threading {
using namespace std::chrono;

TaskThread::TaskThread() :
        m_shuttingDown{false},
        m_stop{false},
        m_alreadyStarting{false},
        m_threadPool{ThreadPool::getDefaultThreadPool()} {
}

TaskThread::~TaskThread() {
    for (;;) {
        {
            std::lock_guard<std::mutex> guard(m_mutex);
            if (!m_alreadyStarting || m_workerThread == nullptr) {
                m_shuttingDown = true;
                return;
            }
            // if We get here, then we obtained the mutex between TaskThread::start and TaskThread::run methods.
        }
        // Wait until the thread has begun running so we can stop safely.
        while (m_alreadyStarting) {
            std::this_thread::yield();
        }
        m_stop = true;
    }
}

bool TaskThread::start(std::function<bool()> jobRunner) {
    if (!jobRunner) {
        std::cout << "startFailed reason invalidFunction" << std::endl;
        return false;
    }

    bool notRunning = false;
    if (!m_alreadyStarting.compare_exchange_strong(notRunning, true)) {
        return false;
    }

    m_startTime = steady_clock::now();

    m_stop = true;
    std::lock_guard<std::mutex> guard(m_mutex);
    if (m_shuttingDown) {
        std::cout << "startFailed reason shuttingDown" << std::endl;
        return false;
    }
    m_workerThread = m_threadPool->obtainWorker(m_moniker);

    m_moniker = m_workerThread->getMoniker();
    m_workerThread->run([this, jobRunner] {
        TaskThread::run(jobRunner);
        return false;
    });
    return true;
}

void TaskThread::run(std::function<bool()> jobRunner) {
    std::lock_guard<std::mutex> guard(m_mutex);
    std::cout << "startThread" << "moniker: "<< m_moniker << 
        "duration : " << duration_cast<microseconds>(steady_clock::now() - m_startTime).count() << std::endl;
    // Reset stop flag and already starting flag.
    m_stop = false;
    m_alreadyStarting = false;

    while (!m_stop && jobRunner())
        ;
    m_threadPool->releaseWorker(std::move(m_workerThread));
}

}  // namespace threading
}  // namespace utils
