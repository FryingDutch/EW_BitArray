#include <iostream>
#include <bitset>

struct FiveBooleanByte
{
	typedef unsigned char byte;

	union
	{
		byte booleans;
		byte values;
		byte ports;
	};


public:
	//start with an empty byte
	FiveBooleanByte() :
		booleans(0b0'0'0'0'0'0'0'0) {
	}

	void add(bool _boolean)
	{
		//bitshift the 3 most significant bits completly to the right and check if their numerical value is lower then 5 (bitcounter)
		if (booleans >> 5 < 5)
		{
			// Bitshift one to left 
			// Add 0b0(false) or 0b1(true)
			// Mask away the counter (3most significant bits) of this current result which is no langer valid due to the bitshift to the left. 
			// Then mask the values of this result with the actual state of the booleans counter (the 3 most significant bits) to add them together.
			if (_boolean) this->booleans = (((this->booleans << 1) + 0b1) & 0b00'01'11'11) | (this->booleans & 0b11'10'00'00);
			else this->booleans = (((this->booleans << 1) + 0b0) & 0b00'01'11'11) | (this->booleans & 0b11'10'00'00);

			// Mask away the values to get the state of the counter (3 most signifcant bits), bitshift 5 to get its numerical value, add one (to counter) and bitshift them back
			// to their original place. Then, use the current state of the booleans to mask away and add the results back together.
			this->booleans = (((this->booleans >> 5) + 1) << 5) | (this->booleans & 0b00'01'11'11);
		}
	}

	void empty()
	{
		this->booleans = 0b00'00'00'00;
	}
};

FiveBooleanByte bools;

int main(void)
{
	bools.add(true);
	bools.add(true);
	bools.add(false);
	bools.add(true);
	bools.add(true);

	std::bitset<8> z(bools.values);
	std::cout << "End: " << z << "\n";
}
