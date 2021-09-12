#include "EW_BitArray.h"
#include <vector>

int main(void)
{
    EW_BitArray<unsigned long long> boolArray;

    boolArray.push_back(false);
    boolArray.push_back(true);
    boolArray.push_back(false);
    boolArray.push_back(true);
    boolArray.push_back(false);

    for (int i = 0; i < boolArray.size(); i++) std::cout << boolArray[i];

    EW_BitArray<unsigned char> boolArray2{ 0b11111, 5 };
    if (!boolArray[0]) std::cout << sizeof(boolArray);

    boolArray.pop_back();
    //===============================================================

    std::vector<bool> boolVector;

    boolVector.push_back(false);
    boolVector.push_back(true);
    boolVector.push_back(false);
    boolVector.push_back(true);
    boolVector.push_back(false);

    for (int i = 0; i < boolVector.size(); i++) std::cout << boolVector[i];

    std::vector<bool> boolVector2{ 1, 1, 1, 1, 1 };
    if (!boolVector[0]) std::cout << sizeof(boolVector);

    boolVector.pop_back();
    //===============================================================

    std::vector<EW_BitArray<unsigned long long>> combinedArray;

    for (int i = 0; i < 100; i++)
    {
        combinedArray.push_back(EW_BitArray<unsigned long long>{});

        for (int j = 0; j < combinedArray[i].MAX(); j++)
        {
            combinedArray[i].push_back(true);
        }
    }
} 