#include <algorithm>

void merge(int arr[], int l, int m, int r);

void merge_sort_omp(int arr[], int l, int r)
{
    if (l < r) {
        int m = l + (r - l) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            merge_sort_omp(arr, l, m);

            #pragma omp section
            merge_sort_omp(arr, m + 1, r);
        }

        merge(arr, l, m, r);
    }
}
