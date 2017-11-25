/**
 * Copyright 2017 @ mtdcy.chen
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, 
 *    this list of conditions and the following disclaimer in the documentation 
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 **/

/**
 * File: 
 * Desc:
 *
 * Changes:
 * 20171201     mtdcy   initial version
 *
 **/

#ifndef __mtp_threads_h__
#define __mtp_threads_h__

#include <mtp/types.h>
#include <pthread.h>

namespace mtp {

    class Condition;
    class Mutex {
        public:
            Mutex();
            virtual ~Mutex();

            void lock();
            void unlock();
            bool tryLock();

        public:
            // scoped lock class 
            class Autolock {
                public:
                    inline explicit Autolock(Mutex& lock) : mLock(lock) { mLock.lock(); }
                    inline explicit Autolock(Mutex* lock) : mLock(*lock) { mLock.lock(); }
                    inline ~Autolock() { mLock.unlock(); }

                private:
                    Mutex&  mLock;
                    DISALLOW_EVILS(Autolock);
            };

        private:
            friend class Condition;
            pthread_mutex_t     mMutex;
            DISALLOW_EVILS(Mutex);
    };

    class Condition {
        public:
            Condition();
            virtual ~Condition();

            void wait(Mutex&);
            void signal();
            void broadcast();

        private:
            pthread_cond_t      mCondition;
            DISALLOW_EVILS(Condition);
    };

    class Thread {
        public:
            Thread();
            virtual ~Thread();

        protected:
            virtual bool threadLoop() = 0;

        private:
            pthread_t           mThread;
            DISALLOW_EVILS(Thread);
    };


    ///////////////////////////////////////////////////////////////////////////
    inline Mutex::Mutex() { 
        pthread_mutex_init(&mMutex, NULL);
    }

    inline Mutex::~Mutex() {
        pthread_mutex_destroy(&mMutex);
    }

    inline void Mutex::lock() {
        pthread_mutex_lock(&mMutex);
    }

    inline void Mutex::unlock() {
        pthread_mutex_unlock(&mMutex);
    }

    inline bool Mutex::tryLock() {
        // TODO: add error handling
        return pthread_mutex_trylock(&mMutex) == 0;
    }

    inline Condition::Condition() {
        pthread_cond_init(&mCondition, NULL);
    }

    inline Condition::~Condition() {
        pthread_cond_destroy(&mCondition);
    }

    inline void Condition::wait(Mutex& mutex) {
        pthread_cond_wait(&mCondition, &mutex.mMutex);
    }

    inline void Condition::signal() {
        pthread_cond_signal(&mCondition);
    }

    inline void Condition::broadcast() {
        return pthread_cond_broadcast(&mCondition);
    }
};

#endif // __mtp_threads_h__
