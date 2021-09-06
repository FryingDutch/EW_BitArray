// FiveBooleanByte is library independend
#include <iostream> // Needed for logging and debugging
#include <bitset> // Needed to show the byte in a binary representation

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
		return (this->values & 0b00'01'11'11) == (rhs.values & 0b00'01'11'11);		
	}

	bool operator!=(const FiveBooleanByte& rhs)
	{
		return (this->values & 0b00'01'11'11) != (rhs.values & 0b00'01'11'11);
	}

	// Overload operator to make it possible to copy FiveBooleanBytes into another.
	void operator=(const FiveBooleanByte& rhs)
	{
		this->values = rhs.values;
	}

public:
	// Start with an empty byte or an already filled in one. Keep in mind that you'll also have to manually input the counter correctly in that case.
	FiveBooleanByte(byte _bits = 0b0'0'0'0'0'0'0'0) :
		bits(_bits) {
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
		// If the counter (3most most signifcant bits) shifted 5 times to the right, have a decimal value higher then 0, to not mess up the state of the counter.
		// Then mask away the cpunter bits (3 most significant) to make sure they dont take the position of a bit ment for a boolean (5 least significant bits).
		// Bitshift one to the right to remove the least significant bit.
		// Then take the counter bits (3 most significant)  of the original value and bitshift them 5 to the right to clear the booleans (5 least significant bits) and get their decimal value.
		// Then mask the result with each other to add them back together.
		if(this->counter >> 5 > 0) this->bits = ((this->booleans & 0b00'01'11'11) >> 1) | (((this->counter >> 5) - 1) << 5);

		std::bitset<8> a(values);
		std::cout << "After removing the least significant bit: " << a << "\n";
		std::cout << "Numerical value of counter: " << (int)(this->counter >> 5) << "\n\n";
	}

	// Again very generic in every position:
	// By first looking at what the counters reads, by bitshifting them 5 to the right, we know its decimal value.
	// If we know what the counters reads, we know where the most significant bit is (and by most significant, we mean the most significant boolean).
	// Then we take everything from the right of the most significant bit and mask away the rest of the byte.
	// Then, if we know what the current counter reads, we also know what its new value should be after decrementing by one.
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
			// Showing that a decimal 7 is the same as binary 00'00'01'11, and therefore, only masks the 3 least signifcant bits.
			// Showing that a hexidecimal 60 is the same as binary 01'10'00'00, therefore, if shifted (>>) 5 this constant will still read as 3.
			this->bits = (this->booleans & 7) | 0x60;          // If shifted right (>>) 5 this constant will read as 3.
			break;

		case 5:
			// Showing that a decimal 128 is the same as binary 10'00'00'00, therefore, if shifted (>>) 5 this constant will still read as 4.
			this->bits = (this->booleans & 0b00'00'11'11) | 128;           // If shifted right (>>) 5 this constant will read as 4.
			break;

		default:
			break;
		}

		std::bitset<8> a(values);
		std::cout << "After removing the most significant bit: " << a << "\n";
		std::cout << "Numerical value of counter: " << (int)(this->counter >> 5) << "\n\n";
	}
	
	// It first checks if the decimal value of the counter (3 most signifcant bits) is higher or equal to the position of the removed bit, to not mess up the state of the counter.
	// Further on the method itself is very generic with every position:
	// It first masks the left bits of the bit asked to be removed
	// Then it bitshifts these one to the right to close the gap of the to be removed bit.
	// Then it masks the right bits of the to be removed bit.
	// Then add these results back together.
	// Then decrement the counter by bitshifting 5 to the right from the origin value, to get its decimal value and decrement by one. 
	// Then shift them back to their original position.
	// Then add the new booleans and the correct counter back together again.
	// For case 1 we can use the same code as for "removeLeastSignificantBit", as the first bit will also always be the least significant.
	// However we do not link to the method directly because calling the method for something thats only 1 line of code would be a waste of resources.
	void removeBit(const byte _position)
	{
		if (this->counter >> 5 >= _position)
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

			default:
				break;
			}
		}

		std::bitset<8> a(values);
		std::cout << "After removing bit at position \"" << (int)_position << "\": " << a << "\n";
		std::cout << "Numerical value of counter: " << (int)(this->counter >> 5) << "\n\n";
	}

	// We first check if the position is currently filled by looking at the counter.
	// Then we mask away the to be replaced bit. 
	// Then we mask the new boolean to see if its a 0 or 1.
	// Then we shift it in position of the to be replaced bit, and mask the results back together again.
	// There is no need to do something with the counter bits (3 most significant),
	// as we dont shift anything around inside the byte and there is no change in amount of bits used.
	void replaceBit(const byte _position, bool _newValue)
	{
		if (this->counter >> 5 >= _position)
		{
			switch (_position)
			{
			case 1:
				this->bits = (this->bits & 0b11'11'11'10) + _newValue;
				break;

			case 2:
				this->bits = (this->bits & 0b11'11'11'01) | ((_newValue & 0b00'00'00'01) << 1);
				break;

			case 3:
				this->bits = (this->bits & 0b11'11'10'11) | ((_newValue & 0b00'00'00'01) << 2);
				break;

			case 4:
				this->bits = (this->bits & 0b11'11'01'11) | ((_newValue & 0b00'00'00'01) << 3);
				break;

			case 5:
				this->bits = (this->bits & 0b11'10'11'11) | ((_newValue & 0b00'00'00'01) << 4);
				break;

			default:
				break;
			}

			std::bitset<8> a(values);
			std::cout << "After replacing the bit at position \"" << (int)_position << "\" with the value \"" << (int)_newValue << "\": " << a << "\n";
			std::cout << "Numerical value of counter: " << (int)(this->counter >> 5) << "\n\n";
		}
	}

	void clear()
	{
		// Bitshift 8 to the right to clear the byte.
		this->bits >>= 8;

		std::bitset<8> a(values);
		std::cout << "After clearing the byte: " << a << "\n";
		std::cout << "Numerical value of counter: " << (int)(this->counter >> 5) << "\n\n";
	}

	// To get the counters decimal value.
	int getCounter()
	{
		return this->counter >> 5;
	}

	// If we know the position to get, we can just bitshift its own position minus 1 to get it at the least significant bit.
	// Then we can simply mask it with a 1 / true / 0b00'00'00'01.
	// Then return the result.
	// To not confuse the user, we allow them to also get the counter bits.
	// So to not get a false negative if they want to check.
	bool getBit(const byte _position)
	{
		return (this->bits >> (_position - 1)) & 1;
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

	myByte.removeBit(2);                // 4
	myByte.removeMostSignificantBit();  // 3
	myByte.removeLeastSignificantBit(); // 2

	// Also possible to use a copy of a boolean instance as an argument.
	bool foo{ true };
	myByte.add(foo); // 3

	myByte.replaceBit(3, false);

	std::bitset<8> binaryRepresentation{ myByte.values }, binaryRepresentation2{};
	std::cout << "End: " << binaryRepresentation << "\n";
	std::cout << "Numerical value of counter: " << (int)(myByte.getCounter()) << "\n\n";
	std::cout << "The size of FiveBooleanByte (without counting its functions): " << sizeof(myByte) << " Byte(s).\n\n";

	system("pause"); // Only for showcasing purposes, this is to pause the system untill there is an input.
	system("CLS"); // Only for showcasing purposes, this is to clear the screen of all previous prints.

	// Create in instance where the counter is set at one and the least significant bit that is dedicated to it, is false.
	FiveBooleanByte myByte2{ 0b00'10'00'00 }; // 1	
	
	// Showing that either NULL, 0 or 1, is also the same as false or true.
	myByte2.add(1);    // 2
	myByte2.add(0);    // 3
	myByte2.add(1);    // 4
	myByte2.add(NULL); // 5

	// Equalize a FiveBooleanByte with another.
	myByte2 = myByte;

	system("pause");
	system("CLS");

	// Compare 2nd FiveBooleanByte with the first.
	binaryRepresentation = myByte.values;
	binaryRepresentation2 = myByte2.values;
	if (myByte2 == myByte) std::cout << "First [\"" << binaryRepresentation << "\"] FiveBooleanByte is equal to second [\"" << binaryRepresentation2 << "\"] FiveBooleanByte.\n\n";

	// Clear bools and compare again.
	myByte.clear();
	binaryRepresentation = myByte.values;
	if(myByte != myByte2) std::cout << "First [\"" << binaryRepresentation << "\"] FiveBooleanByte and second [\"" << binaryRepresentation2 << "\"] FiveBooleanByte are not equal.\n\n";

	system("pause");
	system("CLS");

	// A way for the user to extract single bits to check their value.
	if (!myByte2.getBit(2)) std::cout << binaryRepresentation2 << " - [\"0 0 0 0 0 0 X 0\"] This bit is false.\n";
	if (myByte2.getBit(1) == true) std::cout << binaryRepresentation2 << " - [\"0 0 0 0 0 0 0 X\"] This bit is true.\n";

	for (char i = 0; i < myByte2.getCounter(); i++)
	{
		if (myByte2.getBit(i)) std::cout << "This bit is true\n";
	}
}
