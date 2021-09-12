// FiveBooleanByte is library independend
#include <iostream> // Needed for logging and debugging
#include <bitset> // Needed to show the byte in a binary representation

template<typename T>
struct EW_BitArray
{
	// Typedef the typename to a byte to make its goal more clear and the code still readable.
	typedef T bytes;

private:
	// Set a union to make it possible to call the bits on a different naming, without taking up more memory.
	union
	{
		bytes bits = 0x00;
		bytes counter;
		bytes booleans;
		bytes values;
	};

public:
	// Overload operator to make it possible to compare FiveBooleanBytes with one another without taking the counterbits into factor.
	bool operator==(const EW_BitArray& rhs)
	{
		switch (sizeof(T))
		{
		case 1:
			return (this->values & 31) == (rhs.values & 31);

		case 2:
			return (this->values & 4095) == (rhs.values & 4095);

		case 4:
			return (this->values & 67108863) == (rhs.values & 67108863);

		case 8:
			return (this->values & 0x01FFFFFFFFFFFFFF) == (rhs.values & 0x01FFFFFFFFFFFFFF);

		default:
			return false;
		}
	}

	// Overload operator to make it possible to compare FiveBooleanBytes with one another without taking the counterbits into factor.
	bool operator!=(const EW_BitArray& rhs)
	{
		switch (sizeof(T))
		{
		case 1:
			return (this->values & 31) != (rhs.values & 31);

		case 2:
			return (this->values & 4095) != (rhs.values & 4095);

		case 4:
			return (this->values & 67108863) != (rhs.values & 67108863);

		case 8:
			return (this->values & 0x01FFFFFFFFFFFFFF) != (rhs.values & 0x01FFFFFFFFFFFFFF);

		default:
			return false;
		}
	}

	int operator[](std::size_t _index)
	{
		switch (sizeof(T))
		{
		case 1:
			if (_index < (this->counter >> 5) && _index < 5 && _index >= 0) 
				return this->bits >> (((this->counter >> 5) - 1) - _index) & 1;
			else return -1;

		case 2:
			if (_index < (this->counter >> 12) && _index < 12 && _index >= 0) 
				return this->bits >> (((this->counter >> 12) - 1) - _index) & 1;
			else return -1;

		case 4:
			if (_index < (this->counter >> 27) && _index < 27 && _index >= 0) 
				return this->bits >> (((this->counter >> 27) - 1) - _index) & 1;
			else return -1;

		case 8:
			if (_index < (this->counter >> 58) && _index < 58 && _index >= 0) 
				return this->bits >> (((this->counter >> 58) - 1) - _index) & 1;
			else return -1;

		default:
			return -1;
		}
	}

	// Overload operator to make it possible to copy FiveBooleanBytes into another.
	void operator=(const EW_BitArray& rhs)
	{
		this->values = rhs.values;
	}

public:
	// Start with an empty byte or an already filled in one. Keep in mind that you'll also have to manually input the counter correctly in that case.
	EW_BitArray(bytes _bits = 0x00, T _size = 0)
	{
		if (_size == 0) _bits = 0x00;

		switch (sizeof(T))
		{
		case 1:
			this->bits = (_bits & 31) | (_size << 5);
			break;

		case 2:
			this->bits = (_bits & 4095) | (_size << 12);
			break;

		case 4:
			this->bits = (_bits & 67108863) | (_size << 27);
			break;

		case 8:
			this->bits = (_bits & 0x01FFFFFFFFFFFFFF) | ((unsigned long long)_size << 58);
			break;

		default:
			break;
		}
	}

	// Add a bool at the least significant bit
	void push_back(const bool _boolean)
	{
		switch (sizeof(T))
		{
		case 1:
			if (this->counter >> 5 < 5) 
				this->bits = (((this->booleans << 1) + _boolean) & 31) | (((this->counter >> 5) + 1) << 5);
			break;

		case 2:
			if (this->counter >> 12 < 12) 
				this->bits = (((this->booleans << 1) + _boolean) & 4095) | (((this->counter >> 12) + 1) << 12);
			break;

		case 4:
			if (this->counter >> 27 < 27) 
				this->bits = (((this->booleans << 1) + _boolean) & 67108863) | (((this->counter >> 27) + 1) << 27);
			break;

		case 8:
			if (this->counter >> 58 < 58) 
				this->bits = (((T)((T)this->booleans << 1) + (T)_boolean) & (T)0x01FFFFFFFFFFFFFF) | (((unsigned long long)(this->counter >> 58) + 1) << 58);
			break;

		default:
			break;
		}

		std::bitset<sizeof(T) * 8> a(values);
		std::cout << "After adding the value: " << a << "\n";
		std::cout << "Numerical value of counter: " << this->size() << "\n\n";
	}

	// Remove last element
	void pop_back()
	{
		switch (sizeof(T))
		{
		case 1:
			if (this->counter >> 5 > 0) 
				this->bits = ((this->booleans & 31) >> 1) | (((this->counter >> 5) - 1) << 5);
			break;

		case 2:
			if (this->counter >> 12 > 0) 
				this->bits = ((this->booleans & 4095) >> 1) | (((this->counter >> 12) - 1) << 12);
			break;

		case 4:
			if (this->counter >> 27 > 0) 
				this->bits = ((this->booleans & 67108863) >> 1) | (((this->counter >> 27) - 1) << 27);
			break;

		case 8:
			if (this->counter >> 58 > 0) 
				this->bits = ((this->booleans & (T)0x01FFFFFFFFFFFFFF) >> 1) | (((unsigned long long)(this->counter >> 58) - 1) << 58);
			break;

		default:
			break;
		}

		std::bitset<sizeof(T) * 8> a(values);
		std::cout << "After removing the least significant bit: " << a << "\n";
		std::cout << "Numerical value of counter: " << this->size() << "\n\n";
	}

	// Remove first element
	void pop_front()
	{
		//first counter is to find the position of the to be removed bit, second is to decrement the counter.
		switch (sizeof(T))
		{
		case 1:
			this->bits = (this->booleans & (T)((T)pow(2, (this->counter >> 5) - 1) - 1)) | (((this->counter >> 5) - 1) << 5);
			break;

		case 2:
			this->bits = (this->booleans & (T)((T)pow(2, (this->counter >> 12) - 1) - 1)) | (((this->counter >> 12) - 1) << 12);
			break;

		case 4:
			this->bits = (this->booleans & (T)((T)pow(2, (this->counter >> 27) - 1) - 1)) | (((this->counter >> 27) - 1) << 27);
			break;

		case 8:
			this->bits = (this->booleans & (T)((T)pow(2, (this->counter >> 58) - 1) - 1)) | (((unsigned long long)(this->counter >> 58) - 1) << 58);
			break;

		default:
			break;
		}

		std::bitset<sizeof(T) * 8> a(values);
		std::cout << "After removing the most significant bit: " << a << "\n";
		std::cout << "Numerical value of counter: " << this->size() << "\n\n";
	}

	// Remove element at position
	void erase(const unsigned char _position)
	{
		switch (sizeof(T))
		{
		case 1:
			if (this->counter >> 5 >= _position) 
				this->bits = ((((this->booleans & (((T)31 >> ((this->counter >> 5) - _position)) << ((this->counter >> 5) - _position)))) >> 1) | (this->booleans & (char)(pow(2, (this->counter >> 5) - (_position + 1)) - 1))) | (((this->counter >> 5) - 1) << 5);
			break;

		case 2:
			if (this->counter >> 12 >= _position)  
				this->bits = ((((this->booleans & (((T)4095 >> ((this->counter >> 12) - _position)) << ((this->counter >> 12) - _position)))) >> 1) | (this->booleans & (char)(pow(2, (this->counter >> 12) - (_position + 1)) - 1))) | (((this->counter >> 12) - 1) << 12);
			break;

		case 4:
			if (this->counter >> 27 >= _position)  
				this->bits = ((((this->booleans & (((T)67108863 >> ((this->counter >> 27) - _position)) << ((this->counter >> 27) - _position)))) >> 1) | (this->booleans & (char)(pow(2, (this->counter >> 27) - (_position + 1)) - 1))) | (((this->counter >> 27) - 1) << 27);
			break;

		case 8:
			if (this->counter >> 58 >= _position) 
				this->bits = ((((this->booleans & (((T)0x01FFFFFFFFFFFFFF >> ((this->counter >> 58) - _position)) << ((this->counter >> 58) - _position)))) >> 1) | (this->booleans & (char)(pow(2, (this->counter >> 58) - (_position + 1)) - 1))) | (((this->counter >> 58) - 1) << 58);
			break;

		default:
			break;
		}

		std::bitset<sizeof(T) * 8> a(values);
		std::cout << "After removing bit at position \"" << (int)_position << "\": " << a << "\n";
		std::cout << "Numerical value of counter: " << this->size() << "\n\n";
	}

	// Replace element at position
	void replace(const unsigned char _position, bool _newValue)
	{
		switch (sizeof(T))
		{
		case 1:
			if (this->counter >> 5 >= _position) 
				this->bits = (this->bits & (((T)0xFF << ((this->counter >> 5) - _position)) + ((T)pow(2, (this->counter >> 5) - (_position + 1)) - 1))) | ((T)(_newValue & 1) << ((((this->counter >> 5) - 1) - _position)));
			break;

		case 2:
			if (this->counter >> 12 >= _position) 
				this->bits = (this->bits & (((T)0xFFFF << ((this->counter >> 12) - _position)) + ((T)pow(2, (this->counter >> 12) - (_position + 1)) - 1))) | ((T)(_newValue & 1) << ((((this->counter >> 12) - 1) - _position)));
			break;

		case 4:
			if (this->counter >> 27 >= _position) 
				this->bits = (this->bits & (((T)0xFFFFFFFF << ((this->counter >> 27) - _position)) + ((T)pow(2, (this->counter >> 27) - (_position + 1)) - 1))) | ((T)(_newValue & 1) << ((((this->counter >> 27) - 1) - _position)));
			break;

		case 8:
			if (this->counter >> 58 >= _position) 
				this->bits = (this->bits & (((T)0xFFFFFFFFFFFFFFFF << ((this->counter >> 58) - _position)) + ((T)pow(2, (this->counter >> 58) - (_position + 1)) - 1))) | ((T)(_newValue & 1) << ((((this->counter >> 58) - 1) - _position)));
			break;

		default:
			break;
		}

		std::bitset<sizeof(T) * 8> a(values);
		std::cout << "After replacing the bit at position \"" << (int)_position << "\" with the value \"" << (int)_newValue << "\": " << a << "\n";
		std::cout << "Numerical value of counter: " << this->size() << "\n\n";
	}

	// Clear all data
	void clear()
	{
		switch (sizeof(T))
		{
		case 1:
			this->bits >>= 8;
			break;

		case 2:
			this->bits >>= 16;
			break;

		case 4:
			this->bits >>= 32;
			break;

		case 8:
			this->bits >>= 64;
			break;

		default:
			break;
		}

		std::bitset<sizeof(T) * 8> a(values);
		std::cout << "After clearing the byte: " << a << "\n";
		std::cout << "Numerical value of counter: " << this->size() << "\n\n";
	}

	// To get the amount of elements
	unsigned int size()
	{
		switch (sizeof(T))
		{
		case 1:
			return (unsigned int)(this->counter >> 5);

		case 2:
			return (unsigned int)(this->counter >> 12);

		case 4:
			return (unsigned int)(this->counter >> 27);

		case 8:
			return (unsigned int)(this->counter >> 58);

		default:
			return 0;
		}
	}

	// To get the maximum allowed elements
	const unsigned int MAX()
	{
		switch (sizeof(T))
		{
		case 1:
			return 5;

		case 2:
			return 12;

		case 4:
			return 27;

		case 8:
			return 58;

		default:
			return 0;
		}
	}
};
