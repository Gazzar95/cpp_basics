#include <iostream>
#include "telemetry.h"
#include "cmdstack.h"
#include "SensorBuffer.h"

/*
assignment 3:
- Switch from sliding window to SensorBuffer
*/

int main()
{
    // While loop bool
    bool run = true;

    // Tel params
    double value;

    // cmd stack params
    std::string cmd;
    std::string mov;
    std::string data_mov[30];
    int k{0};

    SensorBuffer buffer{10};
    while (run)

    {

        std::cin >> cmd;

        if (cmd == "sample")
        {
            std::cin >> value;
            if (std::cin.fail())
            {
                std::cout << "ERROR: value must be a number\n";
                std::cin.clear();            // clear the error flag
                std::cin.ignore(1000, '\n'); // discard the bad input
            }
            else
            {
                buffer.push(value);
            }
        }
        else if (cmd == "stats")
        {
            buffer.stats();
        }
        else if (cmd == "dump")
        {
            buffer.dump();
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