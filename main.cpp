#include <iostream>
#include <bitset>

struct FiveBooleanByte
{
	typedef unsigned char byte;
	byte booleans;

public:
	//provide an empty byte
	FiveBooleanByte() :
		booleans(0b0'0'0'0'0'0'0'0){
	}

	void add(bool _boolean)
	{
		byte counter = booleans >> 5;

		if (counter < 5)
		{
			byte result;
			if (_boolean) result = 0b1;
			else result = 0b0;

			this->booleans <<= 1;
			this->booleans += result;
			this->booleans &= 0b00'01'11'11;
			counter++;
			counter <<= 5;

			this->booleans |= counter;
		}
	}

	void remove(byte index)
	{
		byte counter = booleans >> 5;
		byte mask;

		if (counter > 0)
		{
			switch (index)
			{
			case 1:
				booleans &= 0b11'10'11'11;
				break;

			case 2:
				if (counter > index)
				{
					booleans &= 0b00'01'01'11;
					mask = (0b00'01'00'00 & booleans) >> 1;
					booleans &= 0b00'00'01'11;
					booleans |= mask;					
				}

				else booleans &= 0b00'00'01'11;
				break;

			case 3:
				if (counter > index)
				{
					booleans &= 0b00'01'10'11;
					mask = (0b00'01'10'00 & booleans) >> 1;
					booleans &= 0b00'00'00'11;
					booleans |= mask;
				}

				else booleans &= 0b00'00'00'11;

			case 4:
				if (counter > index)
				{
					booleans &= 0b00'01'11'01;
					mask = (0b00'01'11'00 & booleans) >> 1;
					booleans &= 0b00'00'00'01;
					booleans |= mask;
				}

				else booleans &= 0b00'00'00'01;

			case 5:
				if (counter > index)
				{
					booleans &= 0b00'01'11'10;
					mask = (0b00'01'11'10 & booleans) >> 1;
					booleans &= 0b00'00'00'00;
					booleans |= mask;
				}

				else booleans &= 0b00'00'00'00;

			default:
				break;
			}

			counter--;
			counter <<= 5;
			booleans |= counter;
		}
	}
};

FiveBooleanByte bools;

int main(void)
{
	bools.add(true);
	bools.add(true);
	bools.add(true);
	bools.add(true);
	bools.add(true);

	std::bitset<8> r(bools.booleans);
	std::cout << r << "\n";

	bools.remove(4);
	
	std::bitset<8> z(bools.booleans);
	std::cout << z;
}