#include <iostream>

void push(std::string data_mov[], const std::string &mov, int &k)
{
    if (mov == "F" || mov == "B" || mov == "L" || mov == "R" || mov == "S")
    {
        k++;
        data_mov[k] = mov;
    }
}

void pop(std::string data_mov[], int &k)
{
    if (k > 0)
    {
        std::cout << "POPPED=" << data_mov[k] << "\n";
        k--;
    }
    else
    {
        std::cout << "EMPTY" << "\n";
    }
}

void peek(std::string data_mov[], int &k)
{
    std::cout << "TOP=" << data_mov[k] << "\n";
}