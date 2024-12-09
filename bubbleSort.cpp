#include "bubbleSort.h"
#include <cstdio>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <iostream>

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

const int N_THREADS = 10;

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
        queueMutex.lock();

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

void bubbleSort(long long int A[], int n) {
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
