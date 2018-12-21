#include <algorithm>

void bubble_sort_omp(int arr[], int n)
{
    int updated, it = 0;
    do {
        updated = 0;
        ++it;

        #pragma omp parallel for
        for (int j = 0; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                updated = 1;
            }
        }

        #pragma omp parallel for
        for (int j = 1; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                updated = 1;
            }
        }
    } while (updated);
}
