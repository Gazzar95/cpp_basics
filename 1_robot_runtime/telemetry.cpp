#include <iostream>

void sliding_window(int n, int value, int data[])
{
    for (int i = 0; i < n - 1; i++)
    {
        data[i] = data[i + 1];
    }
    data[n - 1] = value;
}

int min(int arr[], int size)
{

    int min{arr[0]};
    for (int i = 0; i < size; i++)
    {
        if (arr[i] < min)
        {
            min = arr[i];
        }
    }
    return min;
}

int max(int arr[], int size)
{
    int max{arr[0]};
    for (int i{0}; i < size; i++)
    {
        if (arr[i] > max)
        {
            max = arr[i];
        }
    }
    return max;
}

float avg(int arr[], int size)
{
    int sum{0};
    float avg;
    for (int i{0}; i < size; i++)
    {
        sum = sum + arr[i];
    }

    avg = static_cast<float>(sum) / size;

    return avg;
}

void dump(int arr[], int size)
{
    std::cout << "SAMPLES: ";

    for (int i{0}; i < size; i++)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";
}