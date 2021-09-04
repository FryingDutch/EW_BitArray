#include <iostream>
#include <bitset>

struct FiveBooleanByte
{
	typedef unsigned char byte;

public:
	// Set a union to make it possible to call the bits on a different naming, without taking up more memory.
	union
	{
		byte bits;
		byte counter;
		byte booleans;
		byte values;
		byte ports;
	};

public:
	// Overload operators to make it possible to compare FiveBooleanBytes with one another without taking the counter (3 most signifcant bits) into factor.
	bool operator==(const FiveBooleanByte& rhs)
	{
		if (this->values & 0b00'01'11'11 == rhs.values & 0b00'01'11'11) return true;
		return false;
	}

	bool operator!=(const FiveBooleanByte& rhs)
	{
		if (this->values & 0b00'01'11'11 != rhs.values & 0b00'01'11'11) return true;
		return false;
	}


public:
	//start with an empty byte
	FiveBooleanByte() :
		booleans(0b0'0'0'0'0'0'0'0) {
	}

	void add(bool _boolean)
	{
		//bitshift the 3 most significant bits completely to the right and check if their numerical value is lower then 5 (bitcounter)
		if (this->counter >> 5 < 5)
		{
			// Bitshift one to left to make room at the least significant bit for the new value.
			// Add 0b0(false) or 0b1(true) at the least significant bit.
			// Mask away the counter (3 most significant bits) of this current result which is no langer valid due to the bitshift to the left. 
			// Then mask the values of this result with the actual state of the booleans counter (the 3 most significant bits) to add them together.
			if (_boolean) this->bits = (((this->booleans << 1) + 0b1) & 0b00'01'11'11) | (this->counter & 0b11'10'00'00);
			else this->bits = (((this->booleans << 1) + 0b0) & 0b00'01'11'11) | (this->counter & 0b11'10'00'00);

			std::bitset<8> a(values);
			std::cout << "After adding the value: " << a << "\n";

			// Bitshift 5 to get the counters (3most significant bits) numerical value, add one (to counter) and bitshift them back to their original place.
			// Then, use the current state of the bits to mask away the old counter (3 most significant bits of the original value) and add the results back together.
			this->bits = (((this->counter >> 5) + 1) << 5) | (this->booleans & 0b00'01'11'11);

			std::bitset<8> b(values);
			std::cout << "After incrementing the counter: " << b << "\n\n";
		}
	}

	void clear()
	{
		this->booleans = 0b00'00'00'00;
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
	std::cout << "The size of FIveBooleanByte (without counting its functions): " << sizeof(FiveBooleanByte) << " Byte(s).\n";

	// Uncomment for the second part of the showcase.
	/*
	FiveBooleanByte bools2;

	bools2.add(true);  // 1
	bools2.add(true);  // 2
	bools2.add(false); // 3
	bools2.add(true);  // 4
	bools2.add(true);  // 5

	//Only for showcasing purposes, this is to clear the screen of all previous prints.
	system("CLS");

	//compare 2nd FiveBooleanByte with the first.
	if(bools2 == bools) std::cout << "First and second FiveBooleanByte are equal.\n";

	//clear bools and compare again.
	bools.clear();
	if(bools != bools2) std::cout << "First and second FiveBooleanByte are not equal.\n";
	*/

	// Uncomment for the third part of the showcase.
	/*
	system("CLS");

	// A way for the user to extract single bits to check their value.
	if ((bools2.booleans & 0b00'00'01'00) >> 2 == false) std::cout << "This one bit is false\n";
	if ((bools2.booleans & 0b00'00'00'01) == true) std::cout << "This bit is true\n";
	*/
	
}
