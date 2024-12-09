/*
# Multithread Bubble Sort
Dr. Barnett,
    You're probably here because you noticed my Bubble Sort durations seemed 
really weird. They are; that's because my bubble sort algorithm is multi-
threaded. I spent a really long time hoping I would be able to make it faster
by throwing more threads at it. Unfortunately, it's even slower than the simple 
implementation. I still decided to use it because I put a lot of work into it,
and becuase I find it really funny. I learned a lot about multithreading in the 
prrocess so I'm still considering it a success!
Note: I also completed a normal bubble sort algorithm in bubbleSortSimple() 
      in case you want to see that. Also, the number of threads my code 
      uses is defined in const int N_THREADS.

## How does it work?
- The driver creates as many threads as specified in N_THREADS.
- Each has a pointer to the work queue, which it will pull from when there's
  work available.
- When a thread finds an out-of-order value, it knows the list might not be
  sorted, so it contributes a new assgnment to the queue.
- Assignments consist of an absolute stopping point (hardstop), past which the
  list is known to be sorted, and a soft stopping point (softstop), which
  informs whichever thread takes it on as to the current position of this
  thread. This allows each thread to ensure that it doesn't step on the previous
  one's toes.
 */

#include "bubbleSort.h"
#include <cstdio>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>


// Normalthreaded implementation
void bubbleSortSimple(long long* A, int n) {
    long long temp;
    bool sorted;
    for (int i = 0; i < n; ++i) {
        sorted = true;
        for (int j = n - 1; j > i; --j) {
            if (A[j - 1] > A[j]) {
                sorted = false;
                temp = A[j - 1];
                A[j - 1] = A[j];
                A[j] = temp;
            }
        }
        // Short-circuit: If this pass made no swaps whatsoever,
        //                the array must be sorted.
        if (sorted) return;
    }
}


// Multithreaded implementation
const int N_THREADS = 15;

std::mutex queueMutex;
std::mutex doneMutex;

struct queueItem {
    std::mutex* softlock;
    int* softstop;
    int hardstop;
};

void bubbleThreadWorker(int id, long long *A, int n, std::queue<queueItem>* queue, bool* done) {
    bool contributed;
    long long temp;

    while (true) {
        doneMutex.lock();
        if (*done) {
            doneMutex.unlock();
            return;
        }
        doneMutex.unlock();

        if (!queueMutex.try_lock()) continue;
        if (!queue->empty()) {
            // Claim the next assignment and release the mutex
            std::mutex* softlock = queue->front().softlock;
            int* softstop = queue->front().softstop;
            int hardstop = queue->front().hardstop;
            queue->pop();
            queueMutex.unlock();

            std::mutex* nextLock = new std::mutex;
            int* current = new int;
            contributed = false;

            for (int i = n - 1; i > hardstop; --i) {
                nextLock->lock();
                *current = i;
                nextLock->unlock();

                while (true) {
                    // Wait for previous thread to get out of the way
                    softlock->lock();
                    if (i > *softstop + 1) {
                        softlock->unlock();
                        break;
                    }
                    softlock->unlock();

                    // std::printf("t%i waiting: i=%i, ss=%i\n", id, i, *softstop);
                    // std::cout << std::flush;
                }

                if (A[i] < A[i - 1]) {
                    if (!contributed) {
                        queueMutex.lock();
                        queue->push({nextLock, current, hardstop + 1});
                        queueMutex.unlock();
                        contributed = true;
                    }

                    temp = A[i - 1];
                    A[i - 1] = A[i];
                    A[i] = temp;
                }
            }

            // If no swaps were made, we're done sorting
            if (!contributed) {
                doneMutex.lock();
                *done = true;
                doneMutex.unlock();
            } else {
                nextLock->lock();
                *current = hardstop - 1;
                nextLock->unlock();
            }

            delete softlock;
            delete softstop;

        } else {
            queueMutex.unlock();
        }
    }
}

void bubbleSortMulti(long long *A, int n) {
    std::mutex* nextLock = new std::mutex;
    int* current = new int(-2);

    std::queue<queueItem> queue;
    queue.push({nextLock, current, 0});

    bool done = false;

    std::vector<std::thread> threads;
    for (int i = 0; i < N_THREADS; ++i) {
        threads.emplace_back(std::thread(bubbleThreadWorker, i, A, n, &queue, &done));
    }

    for (auto &t : threads) {
        t.join();
    }
}

// The actual bubbleSort function: Calls one of the two impls.
void bubbleSort(long long *A, int n) {
    bubbleSortSimple(A, n);
}
