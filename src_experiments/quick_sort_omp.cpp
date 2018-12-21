#include <algorithm>

int partition(int arr[], int low, int high);

void quick_sort_omp(int arr[], int low, int high)
{
    if (low < high) {
        int pi = partition(arr, low, high);

        #pragma omp parallel sections
        {
            #pragma omp section
            quick_sort_omp(arr, low, pi - 1);

            #pragma omp section
            quick_sort_omp(arr, pi + 1, high);
        }
    }
}
