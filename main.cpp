#include <iostream>
#include <bitset>

struct FiveBooleanByte
{
	// Typedef unsigned char to a byte to make its goal more clear.
	typedef unsigned char byte;

public:
	// Set a union to make it possible to call the bits on a different naming, without taking up more memory.
	union
	{
		byte bits;
		byte counter;
		byte booleans;
		byte values;
	};

public:
	// Overload operators to make it possible to compare FiveBooleanBytes with one another without taking the counter (3 most signifcant bits) into factor.
	bool operator==(const FiveBooleanByte& rhs)
	{
		if ((this->values & 0b00'01'11'11) == (rhs.values & 0b00'01'11'11)) return true;
		return false;
	}

	bool operator!=(const FiveBooleanByte& rhs)
	{
		if ((this->values & 0b00'01'11'11) != (rhs.values & 0b00'01'11'11)) return true;
		return false;
	}

public:
	//start with an empty byte
	FiveBooleanByte() :
		bits(0b0'0'0'0'0'0'0'0) {
	}

	// Add a bool at the least significant bit
	void add(const bool _boolean)
	{
		//bitshift the 3 most significant bits completely to the right and check if their numerical value is lower then 5 (bitcounter)
		if (this->counter >> 5 < 5)
		{
			// Bitshift one to left to make room at the least significant bit for the new value.
			// Add 0(false) or 1(true) at the least significant bit.
			// Mask away the counter (3 most significant bits) of this current result which is no langer valid due to the bitshift to the left. 
			// Then mask the values of this result with the actual state of the counter bits (the 3 most significant bits) to add them together.
			this->bits = (((this->booleans << 1) + _boolean) & 0b00'01'11'11) | (this->counter & 0b11'10'00'00);

			std::bitset<8> a(values);
			std::cout << "After adding the value: " << a << "\n";

			// Bitshift 5 to the right get the counters (3most significant bits) numerical value, add one (to counter) and bitshift them back to their original place.
			// Then, use the current state of the bits to mask away the old counter (3 most significant bits of the original value) and add the results back together.
			this->bits = (((this->counter >> 5) + 1) << 5) | (this->booleans & 0b00'01'11'11);

			std::bitset<8> b(values);
			std::cout << "After incrementing the counter: " << b << "\n";
			std::cout << "Numerical value of counter: " << (int)(counter >> 5) << "\n\n";
		}
	}

	void clear()
	{
		// Bitshift 8 to the right to clear the byte.
		this->bits >>= 8;
	}
};

int main(void)
{
	FiveBooleanByte bools;

	bools.add(true);  // 1
	bools.add(true);  // 2
	bools.add(false); // 3
	bools.add(true);  // 4
	bools.add(true);  // 5

	// Will not do anything because the counter is at 5
	bools.add(false); // 6

	std::bitset<8> z(bools.values);
	std::cout << "End: " << z << "\n";
	std::cout << "Numerical value of counter: " << (int)(bools.counter >> 5) << "\n\n";
	std::cout << "The size of FIveBooleanByte (without counting its functions): " << sizeof(bools) << " Byte(s).\n";

	//-------------------------------------------------------------------------------------------------------------------
	// Uncomment for the second part of the showcase.
	/*
	FiveBooleanByte bools2;

	// Showing that either 1 or NULL, is also the same as true or false.
	bools2.add(1);     // 1
	bools2.add(1);     // 2
	bools2.add(NULL);  // 3
	bools2.add(1);     // 4
	bools2.add(1);     // 5

	// Only for showcasing purposes, this is to clear the screen of all previous prints.
	system("CLS");

	// Compare 2nd FiveBooleanByte with the first.
	if(bools2 == bools) std::cout << "First and second FiveBooleanByte are equal.\n";

	// Clear bools and compare again.
	bools.clear();
	if(bools != bools2) std::cout << "First and second FiveBooleanByte are not equal.\n";
	*/

	//-----------------------------------------------------------------------------------------------------------------
	// Uncomment for the third part of the showcase.
	/*
	system("CLS");

	// A way for the user to extract single bits to check their value.
	if ((bools2.booleans & 0b00'00'01'00) >> 2 == false) std::cout << "This one bit is false\n";
	if ((bools2.booleans & 0b00'00'00'01) == true) std::cout << "This bit is true\n";
	*/
}
