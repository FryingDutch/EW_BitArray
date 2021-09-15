#include <stdexcept>

template<typename T = uint64_t>
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
	// Overload operator to make it possible to compare EW_BitArrays with one another without taking the counterbits into factor.
	bool operator==(const EW_BitArray& rhs)
	{
		switch (sizeof(T))
		{
		case 1:
			return (values & 31) == (rhs.values & 31);

		case 2:
			return (values & 4095) == (rhs.values & 4095);

		case 4:
			return (values & 67108863) == (rhs.values & 67108863);

		case 8:
			return (values & 0x01FFFFFFFFFFFFFF) == (rhs.values & 0x01FFFFFFFFFFFFFF);
		}
	}

	// Overload operator to make it possible to compare EW_BitArrays with one another without taking the counterbits into factor.
	bool operator!=(const EW_BitArray& rhs)
	{
		switch (sizeof(T))
		{
		case 1:
			return (values & 31) != (rhs.values & 31);

		case 2:
			return (values & 4095) != (rhs.values & 4095);

		case 4:
			return (values & 67108863) != (rhs.values & 67108863);

		case 8:
			return (values & 0x01FFFFFFFFFFFFFF) != (rhs.values & 0x01FFFFFFFFFFFFFF);
		}
	}

	int16_t operator[](std::size_t _index)
	{
		switch (sizeof(T))
		{
		case 1:
			if (_index < (counter >> 5) && _index < 5 && _index >= 0) 
				return bits >> (((counter >> 5) - 1) - _index) & 1;
			else throw std::out_of_range("Index out of bounds");
			return -1;

		case 2:
			if (_index < (counter >> 12) && _index < 12 && _index >= 0) 
				return bits >> (((counter >> 12) - 1) - _index) & 1;
			else throw std::out_of_range("Index out of bounds");
			return -1;

		case 4:
			if (_index < (counter >> 27) && _index < 27 && _index >= 0) 
				return bits >> (((counter >> 27) - 1) - _index) & 1;
			else throw std::out_of_range("Index out of bounds");
			return -1;

		case 8:
			if (_index < (counter >> 58) && _index < 58 && _index >= 0) 
				return bits >> (((counter >> 58) - 1) - _index) & 1;
			else throw std::out_of_range("Index out of bounds");
			return -1;

		default:
			return -1;
		}
	}

	// Overload operator to make it possible to copy EW_BitArrays into one another.
	void operator=(const EW_BitArray& rhs)
	{
		values = rhs.values;
	}

public:
	// Start with an empty byte or an already filled in one. Keep in mind that you'll also have to manually input the counter correctly in that case.
	EW_BitArray(T _size = 0, bytes _bits = 0x00)
	{
		if (_size == 0) _bits = 0x00;

		switch (sizeof(T))
		{
		case 1:
			bits = (_bits & 31) | (_size << 5);
			break;

		case 2:
			bits = (_bits & 4095) | (_size << 12);
			break;

		case 4:
			bits = (_bits & 67108863) | (_size << 27);
			break;

		case 8:
			bits = (_bits & 0x01FFFFFFFFFFFFFF) | ((uint64_t)_size << 58);
			break;

		default:
			throw std::logic_error("No bits allocated");
			break;
		}
	}

	// Add a bool at the least significant bit
	void add(const bool _boolean) noexcept
	{
		switch (sizeof(T))
		{
		case 1:
			if (counter >> 5 < 5) 
				bits = (((booleans << 1) + _boolean) & 31) | (((counter >> 5) + 1) << 5);
			break;

		case 2:
			if (counter >> 12 < 12) 
				bits = (((booleans << 1) + _boolean) & 4095) | (((counter >> 12) + 1) << 12);
			break;

		case 4:
			if (counter >> 27 < 27) 
				bits = (((booleans << 1) + _boolean) & 67108863) | (((counter >> 27) + 1) << 27);
			break;

		case 8:
			if (counter >> 58 < 58) 
				bits = (((T)((T)booleans << 1) + (T)_boolean) & (T)0x01FFFFFFFFFFFFFF) | (((uint64_t)(counter >> 58) + 1) << 58);
			break;
		}
	}

	// Remove last element
	void pop_back() noexcept
	{
		switch (sizeof(T))
		{
		case 1:
			if (counter >> 5 > 0) 
				bits = ((booleans & 31) >> 1) | (((counter >> 5) - 1) << 5);
			break;

		case 2:
			if (counter >> 12 > 0) 
				bits = ((booleans & 4095) >> 1) | (((counter >> 12) - 1) << 12);
			break;

		case 4:
			if (counter >> 27 > 0) 
				bits = ((booleans & 67108863) >> 1) | (((counter >> 27) - 1) << 27);
			break;

		case 8:
			if (counter >> 58 > 0) 
				bits = ((booleans & (T)0x01FFFFFFFFFFFFFF) >> 1) | (((uint64_t)(counter >> 58) - 1) << 58);
			break;
		}
	}

	// Remove first element
	void pop_front() noexcept
	{
		//first counter is to find the position of the to be removed bit, second is to decrement the counter.
		switch (sizeof(T))
		{
		case 1:
			bits = (booleans & (T)((T)pow(2, (counter >> 5) - 1) - 1)) | (((counter >> 5) - 1) << 5);
			break;

		case 2:
			bits = (booleans & (T)((T)pow(2, (counter >> 12) - 1) - 1)) | (((counter >> 12) - 1) << 12);
			break;

		case 4:
			bits = (booleans & (T)((T)pow(2, (counter >> 27) - 1) - 1)) | (((counter >> 27) - 1) << 27);
			break;

		case 8:
			bits = (booleans & (T)((T)pow(2, (counter >> 58) - 1) - 1)) | (((uint64_t)(counter >> 58) - 1) << 58);
			break;
		}
	}

	// Remove element at position
	void remove(const uint8_t _position) noexcept
	{
		switch (sizeof(T))
		{
		case 1:
			if (counter >> 5 >= _position) 
				bits = ((((booleans & (((T)31 >> ((counter >> 5) - _position)) << ((counter >> 5) - _position)))) >> 1) | (booleans & (int8_t)(pow(2, (counter >> 5) - (_position + 1)) - 1))) | (((counter >> 5) - 1) << 5);
			break;

		case 2:
			if (counter >> 12 >= _position)  
				bits = ((((booleans & (((T)4095 >> ((counter >> 12) - _position)) << ((counter >> 12) - _position)))) >> 1) | (booleans & (int8_t)(pow(2, (counter >> 12) - (_position + 1)) - 1))) | (((counter >> 12) - 1) << 12);
			break;

		case 4:
			if (counter >> 27 >= _position)  
				bits = ((((booleans & (((T)67108863 >> ((counter >> 27) - _position)) << ((counter >> 27) - _position)))) >> 1) | (booleans & (int8_t)(pow(2, (counter >> 27) - (_position + 1)) - 1))) | (((counter >> 27) - 1) << 27);
			break;

		case 8:
			if (counter >> 58 >= _position) 
				bits = ((((booleans & (((T)0x01FFFFFFFFFFFFFF >> ((counter >> 58) - _position)) << ((counter >> 58) - _position)))) >> 1) | (booleans & (int8_t)(pow(2, (counter >> 58) - (_position + 1)) - 1))) | (((counter >> 58) - 1) << 58);
			break;
		}
	}

	// Replace element at position
	void replace(const uint8_t _position, bool _newValue) noexcept
	{
		switch (sizeof(T))
		{
		case 1:
			if (counter >> 5 >= _position) 
				bits = (bits & (((T)0xFF << ((counter >> 5) - _position)) + ((T)pow(2, (counter >> 5) - (_position + 1)) - 1))) | ((T)(_newValue & 1) << ((((counter >> 5) - 1) - _position)));
			break;

		case 2:
			if (counter >> 12 >= _position) 
				bits = (bits & (((T)0xFFFF << ((counter >> 12) - _position)) + ((T)pow(2, (counter >> 12) - (_position + 1)) - 1))) | ((T)(_newValue & 1) << ((((counter >> 12) - 1) - _position)));
			break;

		case 4:
			if (counter >> 27 >= _position) 
				bits = (bits & (((T)0xFFFFFFFF << ((counter >> 27) - _position)) + ((T)pow(2, (counter >> 27) - (_position + 1)) - 1))) | ((T)(_newValue & 1) << ((((counter >> 27) - 1) - _position)));
			break;

		case 8:
			if (counter >> 58 >= _position) 
				bits = (bits & (((T)0xFFFFFFFFFFFFFFFF << ((counter >> 58) - _position)) + ((T)pow(2, (counter >> 58) - (_position + 1)) - 1))) | ((T)(_newValue & 1) << ((((counter >> 58) - 1) - _position)));
			break;
		}
	}

	// Clear all data
	void clear() noexcept
	{
		switch (sizeof(T))
		{
		case 1:
			bits >>= 8;
			break;

		case 2:
			bits >>= 16;
			break;

		case 4:
			bits >>= 32;
			break;

		case 8:
			bits >>= 64;
			break;

		default:
			break;
		}
	}

	// To get the amount of elements
	const uint16_t size() noexcept
	{
		switch (sizeof(T))
		{
		case 1:
			return (uint32_t)(counter >> 5);

		case 2:
			return (uint32_t)(counter >> 12);

		case 4:
			return (uint32_t)(counter >> 27);

		case 8:
			return (uint32_t)(counter >> 58);
		}
	}

	// To get the maximum allowed elements
	const uint16_t MAX() noexcept
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
		}
	}
};
