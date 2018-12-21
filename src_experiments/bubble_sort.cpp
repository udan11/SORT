#include <algorithm>

void bubble_sort(int arr[], int n)
{
    int updated, it = 0;
    do {
        updated = 0;
        ++it;

        for (int j = 0; j < n - it; j++) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                updated = 1;
            }
        }
    } while (updated);
}
