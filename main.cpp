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
		//bitshift the 3 most significant bits completely to the right and check if their decimal value is lower then 5 (bitcounter)
		if (this->counter >> 5 < 5)
		{
			// Bitshift one to left to make room at the least significant bit for the new value.
			// Add 0(false) or 1(true), by looking at the boolean provided by the user, at the least significant bit.
			// Mask away the counter (3 most significant bits) of this current result which is no langer valid due to the bitshift to the left. 
			// Bitshift 5 to the right from the original state to get the old counters (3most significant bits) decimal value. 
			// Add one (to counter) and bitshift them back to their original place (5 Bitshifts to the left).
			// Then, use the result of the boolean addition and counter incrementing and add the results back together.
			this->bits = (((this->booleans << 1) + _boolean) & 0b00'01'11'11) | (((this->counter >> 5) + 1) << 5);

			std::bitset<8> a(values);
			std::cout << "After adding the value: " << a << "\n";
			std::cout << "Numerical value of counter: " << (int)(this->counter >> 5) << "\n\n";
		}
	}

	void removeLeastSignificantBit()
	{
		// Mask away the cpunter bits (3 most significant) to make sure they dont take the position of a bit ment for a boolean (5 least significant bits).
		// Bitshift one to the right to remove the least significant bit.
		// Then take the counter bits (3 most significant)  of the original value and bitshift them 5 to the right to clear the booleans (5 least significant bits) and get their decimal value.
		// Then mask the result with each other to add them back together.
		this->bits = ((this->booleans & 0b00'01'11'11) >> 1) | (((this->counter >> 5) - 1) << 5);

		std::bitset<8> a(values);
		std::cout << "After removing the bit: " << a << "\n";
		std::cout << "Numerical value of counter: " << (int)(this->counter >> 5) << "\n\n";
	}

	// This function is very generic with every position:
	// It first masks the left bits of the bit asked to be removed
	// Then it bitshifts these one to the right to close the gap of the to be removed bit.
	// Then it masks the right bits of the to be removed bit.
	// Then add these results back together.
	// Then decrement the counter by bitshifting 5 to the right from the origin value, to get its decimal value and decrement by one. 
	// Then shift them back to their original position.
	// Then add the new booleans and the correct counter back together again.
	// For case 1 we can use the same code as for "removeLeastSignificantBit", as the first bit will also always be the least significant.
	// However we do not link to the method directly because calling the method for something thats only 1 line of code would be a waste of resources.
	void removeThisBit(byte _position)
	{
		switch (_position)
		{
		case 1:
			this->bits = ((this->booleans & 0b00'01'11'11) >> 1) | (((this->counter >> 5) - 1) << 5);
			break;

		case 2:
			this->bits = (((this->booleans & 0b00'01'11'00) >> 1) | (this->booleans & 0b00'00'00'01)) | (((this->counter >> 5) - 1) << 5);
			break;

		case 3:
			this->bits = (((this->booleans & 0b00'01'10'00) >> 1) | (this->booleans & 0b00'00'00'11)) | (((this->counter >> 5) - 1) << 5);
			break;

		case 4:
			this->bits = (((this->booleans & 0b00'01'00'00) >> 1) | (this->booleans & 0b00'00'01'11)) | (((this->counter >> 5) - 1) << 5);
			break;

		case 5:
			this->bits = this->booleans & 0b00'00'11'11 | (((this->counter >> 5) - 1) << 5);
			break;
		}
	}

	// Again very generic in every position:
	// By first looking at what the counters reads, by bitshifting them 5 to the right, we know its decimal value.
	// If we know what the counters reads, we know where the most significant bit is (and by most significant, we mean the most significant boolean).
	// Then we take everything from the right of the most significant bit and mask away the rest of the byte.
	// Then, if we know what the current counters reads, we also no what its new value should be after decrementing by one.
	// Therefore, we just mask the known constant of the decremented value with the result of the new booleans to add them back together again.
	void removeMostSignificantBit()
	{
		switch (this->counter >> 5)
		{
		case 1:
			this->bits >>= 8; // Clear the full byte since minus one will result in NULL.
			break;

		case 2:
			this->bits = (this->booleans & 0b00'00'00'01) | 0b00'10'00'00; // If shifted right (>>) 5 this constant will read as 1.
			break;

		case 3:
			this->bits = (this->booleans & 0b00'00'00'11) | 0b01'00'00'00; // If shifted right (>>) 5 this constant will read as 2.
			break;

		case 4:
			this->bits = (this->booleans & 0b00'00'01'11) | 0b01'10'00'00; // If shifted right (>>) 5 this constant will read as 3.
			break;

		case 5:
			this->bits = (this->booleans & 0b00'00'11'11) | 0b10'00'00'00; // If shifted right (>>) 5 this constant will read as 4.
			break;

		default:
			break;
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
	FiveBooleanByte myByte;

	myByte.add(true);  // 1
	myByte.add(true);  // 2
	myByte.add(false); // 3
	myByte.add(true);  // 4
	myByte.add(true);  // 5

	// Will not do anything because the counter is at 5
	myByte.add(false); // 6

	myByte.removeThisBit(5);

	std::bitset<8> z(myByte.values);
	std::cout << "End: " << z << "\n";
	std::cout << "Numerical value of counter: " << (int)(myByte.counter >> 5) << "\n\n";
	std::cout << "The size of FIveBooleanByte (without counting its functions): " << sizeof(myByte) << " Byte(s).\n";

	//-------------------------------------------------------------------------------------------------------------------
	// Uncomment for the second part of the showcase.
	/*
	FiveBooleanByte myByte2;
	
	// Showing that either 1 ,NULL or 0, is also the same as true or false.
	myByte2.add(0);     // 1
	myByte2.add(1);     // 2
	myByte2.add(1);     // 3
	myByte2.add(NULL);  // 4
	myByte2.add(1);     // 5

	// Only for showcasing purposes, this is to clear the screen of all previous prints.
	system("CLS");

	// Compare 2nd FiveBooleanByte with the first.
	if(myByte2 == myByte) std::cout << "First and second FiveBooleanByte are equal.\n";

	// Clear bools and compare again.
	myByte.clear();
	if(myByte != myByte2) std::cout << "First and second FiveBooleanByte are not equal.\n";
	*/

	//-----------------------------------------------------------------------------------------------------------------
	// Uncomment for the third part of the showcase.
	/*
	system("CLS");

	// A way for the user to extract single bits to check their value.
	if ((myByte2.booleans & 0b00'00'00'10) >> 1 == false) std::cout << "This one bit is false\n";
	if ((myByte2.booleans & 0b00'00'00'01) == true) std::cout << "This bit is true\n";
	*/
}
