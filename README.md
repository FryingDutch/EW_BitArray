# EW_BitArray: A dynamic bit array.

**EW_BitArray** is a C++ datatype that allows the user to dynamically store an array of booleans inside the bytes itself.  
And therefore the user is no longer bound to use a single byte for each boolean.  
  
It has a build in counter that keeps track of the amount of bits used.  
It can use a container of 1, 2, 4, or 8 bytes. In all cases, the most signifcant bits are reserved for the counter.
  
## Bits available
1 Byte : 5  bits  
2 Bytes: 12 bits  
4 Bytes: 27 bits  
8 Bytes: 58 bits

## Usage
Much of the usage looks similar to [`std::vector`](https://en.cppreference.com/w/cpp/container/vector):
```cpp
// Create a new EW_BitArray of 1 byte
EW_BitArray<uint8_t> boolArray;

// Add a new boolean
boolArray.add(true);

// Remove first element
boolArray.pop_front();

// Remove last element:
boolArray.pop_back();

// Remove at index
boolArray.remove(i)

// Replace at index
boolArray.replace(i, false);

// To get the current amount of elements inside the array
boolArray.size()

// To get the maximum allowed elements inside the array
boolArray.MAX();

// Clear array
boolArray.clear();
```
The user can loop trough a **EW_BitArray** like any other array:
```cpp
for(size_t i = 0; i < boolArray.size(); i++)
{
   std::cout << boolArray[i] << "\n";
}
```

## Known issues and limitations
- No implementation for a correctness focused backup server yet
