#include "EW_BitArray.h"
#include <vector>

int main(void)
{
    EW_BitArray<uint16_t> boolArray;

    boolArray.add(false);
    boolArray.add(true);
    boolArray.add(false);
    boolArray.add(true);
    boolArray.add(false);

    std::cout << boolArray.size();

    EW_BitArray<uint8_t> boolArray2{ 5, 0b11111 };
} 