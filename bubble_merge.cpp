#include <iostream>
#include <omp.h>
using namespace std;

/* ---------- Parallel Bubble Sort ---------- */
void bubbleSort(int a[], int n)
{
    for (int i = 0; i < n; i++)
    {
        int first = i % 2;

        #pragma omp parallel for
        for (int j = first; j < n - 1; j += 2)
        {
            if (a[j] > a[j + 1])
                swap(a[j], a[j + 1]);
        }
    }
}

/* ---------- Merge Function ---------- */
void merge(int a[], int l, int m, int r)
{
    int i = l, j = m + 1, k = 0;
    int temp[100];

    while (i <= m && j <= r)
        temp[k++] = (a[i] < a[j]) ? a[i++] : a[j++];

    while (i <= m)
        temp[k++] = a[i++];

    while (j <= r)
        temp[k++] = a[j++];

    for (i = l, k = 0; i <= r; i++, k++)
        a[i] = temp[k];
}

/* ---------- Parallel Merge Sort ---------- */
void mergeSort(int a[], int l, int r)
{
    if (l < r)
    {
        int m = (l + r) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            mergeSort(a, l, m);

            #pragma omp section
            mergeSort(a, m + 1, r);
        }

        merge(a, l, m, r);
    }
}

/* ---------- Print Array ---------- */
void printArray(int a[], int n)
{
    for (int i = 0; i < n; i++)
        cout << a[i] << " ";
    cout << endl;
}

/* ---------- Main ---------- */
int main()
{
    int a[] = {9, 5, 1, 8, 3, 7};
    int n = 6;

    int choice;

    cout << "1. Bubble Sort\n2. Merge Sort\n";
    cout << "Enter choice: ";
    cin >> choice;

    cout << "\nOriginal Array:\n";
    printArray(a, n);

    if (choice == 1)
    {
        bubbleSort(a, n);
        cout << "\nSorted by Bubble Sort:\n";
    }
    else
    {
        mergeSort(a, 0, n - 1);
        cout << "\nSorted by Merge Sort:\n";
    }

    printArray(a, n);

    return 0;
}