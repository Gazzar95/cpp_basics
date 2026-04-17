#include <iostream>
#include "telemetry.h"
#include "cmdstack.h"

/*
assignment 3:
- Switch from sliding window to SensorBuffer
    -SensorBuffer:
*/

bool run = true;

std::string cmd;

// Tel params
int data[10];
int value;

int n{0};
int min_d;
int max_d;
float avg_d;

// cmd stack params
std::string mov;
std::string data_mov[30];
int k{0};

int main()
{
    while (run)
    {

        std::cin >> cmd;

        if (cmd == "sample")
        {
            std::cin >> value;

            if (n < 10)
            {
                data[n] = value;
                n++;
            }
            else if (std::cin.fail())
            {
                std::cout << "ERROR: value must be an integer\n";
                std::cin.clear();            // clear the error flag
                std::cin.ignore(1000, '\n'); // discard the bad input
            }
            else
            {
                sliding_window(n, value, data);
            }
        }
        else if (cmd == "stats")
        {
            if (n == 0)
            {
                std::cout << "EMPTY" << "\n";
                ;
            }
            else
            {
                min_d = min(data, n);
                max_d = max(data, n);
                avg_d = avg(data, n);
                std::cout << "COUNT=" << n << " MIN=" << min_d << " MAX=" << max_d << " AVG=" << avg_d << "\n";
            }
        }
        else if (cmd == "dump")
        {
            dump(data, n);
        }

        else if (cmd == "push")
        {
            std::cin >> mov;
            push(data_mov, mov, k);
        }
        else if (cmd == "pop")
        {
            pop(data_mov, k);
        }
        else if (cmd == "peek")
        {
            peek(data_mov, k);
        }
        else if (cmd == "quit")
        {
            run = false;
        }
        else
        {
            std::cout << "ERROR: Incorrect Input\n";
        }
    }
    return 0;
}