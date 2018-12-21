#include <algorithm>

const int RUN = 128;

void insertion_sort(int arr[], int n);
void merge(int arr[], int l, int m, int r);

void tim_sort(int arr[], int n)
{
    for (int i = 0; i < n; i += RUN) {
        insertion_sort(arr + i, std::min(RUN, n - i));
    }

    for (int size = RUN; size < n; size = 2 * size) {
        for (int left = 0; left < n; left += 2 * size) {
            int mid = left + size - 1;
            if (mid <= n) {
                int right = std::min(left + 2 * size - 1, n - 1);
                merge(arr, left, mid, right);
            }
        }
    }
}
