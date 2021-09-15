#include "EW_BitArray.h"
#include <iostream>

int main(void)
{
    EW_BitArray<> boolArray;

    boolArray.add(false);
    boolArray.add(true);
    boolArray.add(false);
    boolArray.add(true);
    boolArray.add(false);

    std::cout << boolArray.MAX();

    EW_BitArray<uint8_t> boolArray2{ 5, 0b11111 };
} 