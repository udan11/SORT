#ifndef __PPSORT_H__
#define __PPSORT_H__

#include <boost/asio.hpp>

inline int partition(int* arr, int low, int high)
{
    int pivot = arr[low];
    int i = low - 1, j = high + 1;

    while (1) {
        do { ++i; } while (arr[i] < pivot);
        do { --j; } while (arr[j] > pivot);
        if (i >= j) return j;
        std::swap(arr[i], arr[j]);
    }
}

inline void quick_sort(boost::asio::thread_pool& pool, int arr[], int low, int high)
{
    if (low >= high) {
        return;
    }

    if (high - low < 16384) {
        std::sort(arr + low, arr + high + 1);
    }
    else {
        int pi = partition(arr, low, high);
        boost::asio::post(pool, [&pool, arr, low, pi]() { quick_sort(pool, arr, low, pi); });
        boost::asio::post(pool, [&pool, arr, high, pi]() { quick_sort(pool, arr, pi + 1, high); });
    }
}

#endif /* __PPSORT_H__ */
