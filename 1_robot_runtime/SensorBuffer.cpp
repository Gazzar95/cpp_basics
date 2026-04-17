/*
Requirements:
- Class `SensorBuffer` in `sensor_buffer.h` / `sensor_buffer.cpp`
- Constructor takes `capacity` as argument, allocates buffer on the heap with `new`
- Destructor frees with `delete[]`
- Methods: `push(double value)`, `stats()` (prints min, max, mean), `isFull()`, `isEmpty()`
- In `main.cpp`, instantiate with `SensorBuffer buffer{10};` (uniform init)
- Handle the edge cases: push to full buffer overwrites oldest (ring behavior)
*/
#include <iostream>
#include <string>

class SensorBuffer
{
public:
    SensorBuffer(int cap)
    {
        capacity = cap;
        data = new double[capacity];
    };

    void push(double value)
    {

        if (isFull())
        {
            data[k] = value;
            k = (k + 1) % capacity;
        }
        else
        {
            data[n] = value;
            n++;
        }
    };

    void stats()
    {
        if (isEmpty())
        {
            std::cout << "Error: no data available";
        }
        else
        {
            double min_d{};
            double max_d{};
            double avg_d{};
            min_d = min();
            max_d = max();
            avg_d = avg();
            std::cout << "COUNT=" << n << " MIN=" << min_d << " MAX=" << max_d << " AVG=" << avg_d << "\n";
        }
    }

    bool isFull() { return n == capacity; }

    bool isEmpty() { return n == 0; }

    ~SensorBuffer()
    {
        delete[] data;
    };

private:
    int n{};
    int k{};
    double *data;
    int capacity{};

    double min()
    {
        double min{data[0]};
        for (int i{}; i < n; i++)
        {
            if (data[i] < min)
            {
                min = data[i];
            }
        }
        return min;
    };

    double max()
    {
        double max{data[0]};
        for (int i{}; i < n; i++)
        {
            if (data[i] > max)
            {
                max = data[i];
            }
        }
        return max;
    }

    double avg()
    {
        double sum{0};
        double avg;
        for (int i{0}; i < n; i++)
        {
            sum = sum + data[i];
        }

        avg = (sum) / n;

        return avg;
    }
};