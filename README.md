# EW_BitArray: A dynamic bit array.

**EW_BitArray** is a C++ datatype that allows the user to _dynamically_ store an array of booleans inside the bytes itself.  
And therefore the user is no longer bound to use a single byte for each boolean.  
  
It has a build in counter that keeps track of the amount of bits used.  
It can use a container of **1, 2, 4, or 8 bytes**. In all cases, the _most signifcant bits_ are reserved for the counter.

There is also nu use of local variables, only the necessary function arguments. This is to minimalize memory usage in general.  
Also it doesnt use its own methods, even though it would make the code more readable, it takes away from its efficiency.
  
## Bits available
**1 Byte -** 5 bits  
**2 Bytes -** 12 bits  
**4 Bytes -** 27 bits  
**8 Bytes -** 58 bits

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
- Not possible to replace an element by using the array indexing:
```cpp
for(size_t i = 0; i < boolArray.size(); i++)
{
   boolArray[i] = 0;
}
```
