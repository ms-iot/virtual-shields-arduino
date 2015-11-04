/*
    Copyright(c) Microsoft Open Technologies, Inc. All rights reserved.

    The MIT License(MIT)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files(the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions :

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#ifndef SensorModels_h
#define SensorModels_h

#include <stdint.h>

enum SensorAction
{
	Stop = 0,
	Once = 1,
	Start = 2,
	OnceOnChange = 3
};

enum EPtrType
{
	None = 0,
	ProgPtr = 1,
	MemPtr = 2,
	Int = 3,
	Uint = 4,
	Double = 5,
	Long = 6,
	Bool = 7,
	Char = 8,
	ArrayStart = 9,
	ArrayEnd = 10,
	ValueOnly = 11,
	Format = 12,
	Parse = 13
};

union ARGB
{
	uint32_t color;
	struct
	{
        uint8_t blue, green, red, alpha;
	};

	ARGB(uint8_t alpha, uint8_t red, uint8_t green, uint8_t blue) :
        blue(blue), green(green), red(red), alpha(alpha)
	{
	}

	ARGB(uint8_t red, uint8_t green, uint8_t blue) :
        blue(blue), green(green), red(red), alpha(0)
	{
	}

	ARGB() : color(static_cast<uint32_t>(0)) {}

	ARGB(uint32_t color) : color(color) {}

	ARGB(const char * hex) : ARGB((unsigned long)strtol(&hex[hex[0] == '#'], NULL, 16))
	{
	}

	void hex(uint8_t* hexSource)
	{
        uint8_t hex[9] =
        { static_cast<uint8_t>(alpha >> 4), static_cast<uint8_t>(alpha & 0x0F),
            static_cast<uint8_t>(red >> 4), static_cast<uint8_t>(red & 0x0F),
            static_cast<uint8_t>(green >> 4), static_cast<uint8_t>(green & 0x0F),
            static_cast<uint8_t>(blue >> 4), static_cast<uint8_t>(blue & 0x0F) };

		for (int i = 0; i < 8; i++)
		{
			hexSource[i] = hex[i] + (hex[i] > 0x09 ? 0x37 : 0x30);
		}

		hexSource[8] = '\0';

		return;
	}
};

const bool AsText = true;

struct EPtr
{
	EPtrType ptrType;
	const char* key = NULL;
	union
	{
		const char* value = NULL;
		double doubleValue;
		uint32_t uintValue;
		int intValue;
		long longValue;
		bool boolValue;
		char charValue;
	};

	int length;
	bool keyIsMem = false;
	bool asText = false;
	bool encoded = false;
	EPtr* eptrs = NULL;

	/// <summary>
	/// Initializes a new instance of the <see cref="EPtr"/> struct.
	/// </summary>
	EPtr() {}

	/// <summary>
	/// Initializes a new instance of the <see cref="EPtr"/> struct.
	/// </summary>
	/// <param name="ptrType">Type of the EPtr.</param>
	EPtr(EPtrType ptrType) : ptrType(ptrType) {}

	/// <summary>
	/// Initializes a new instance of the <see cref="EPtr"/> struct.
	/// </summary>
	/// <param name="ptrType">Type of the EPtr.</param>
    EPtr(EPtrType ptrType, const char* key, EPtr* eptrs, int len) : ptrType(ptrType), key(key), intValue(len), asText(true), eptrs(eptrs) {}

	/// <summary>
	/// Initializes a new instance of the <see cref="EPtr"/> struct.
	/// </summary>
	/// <param name="ptrType">Type of the EPtr.</param>
	/// <param name="key">The key.</param>
	EPtr(EPtrType ptrType, const char* key) : ptrType(ptrType), key(key) {}
	
	/// <summary>
	/// Initializes a new instance of the <see cref="EPtr"/> struct.
	/// </summary>
	/// <param name="ptrType">Type of the PTR.</param>
	/// <param name="key">The key.</param>
	/// <param name="value">The value.</param>
    EPtr(EPtrType ptrType, const char* key, const char* value) : ptrType(ptrType), key(key), value(value), length(-1), asText(true) {}

	/// <summary>
	/// Initializes a new instance of the <see cref="EPtr"/> struct.
	/// </summary>
	/// <param name="key">The key.</param>
	/// <param name="value">The value.</param>
    EPtr(const char* key, const char* value) : ptrType(ProgPtr), key(key), value(value), asText(true) {}

	/// <summary>
	/// Initializes a new instance of the <see cref="EPtr"/> struct.
	/// </summary>
	/// <param name="key">The key.</param>
	/// <param name="value">The value.</param>
    EPtr(const char* key, const char value) : ptrType(value ? Char : None), key(key), charValue(value), asText(true) {}

	/// <summary>
	/// Initializes a new instance of the <see cref="EPtr"/> struct.
	/// </summary>
	/// <param name="key">The key.</param>
	/// <param name="value">The value.</param>
	/// <param name="ptrType">Type of the EPtr.</param>
    EPtr(const char* key, int value, EPtrType ptrType = Int) : ptrType(ptrType), key(key), intValue(value) {}

	/// <summary>
	/// Initializes a new instance of the <see cref="EPtr"/> struct.
	/// </summary>
	/// <param name="key">The key.</param>
	/// <param name="value">The value.</param>
	/// <param name="ptrType">Type of the EPtr.</param>
    EPtr(const char* key, uint32_t value, EPtrType ptrType = Uint) : ptrType(ptrType), key(key), uintValue(value) {}

	/// <summary>
	/// Initializes a new instance of the <see cref="EPtr"/> struct.
	/// </summary>
	/// <param name="key">The key.</param>
	/// <param name="value">The value.</param>
	/// <param name="ptrType">Type of the EPtr.</param>
    EPtr(const char* key, long value, EPtrType ptrType = Long) : ptrType(ptrType), key(key), longValue(value) {}

	/// <summary>
	/// Initializes a new instance of the <see cref="EPtr"/> struct.
	/// </summary>
	/// <param name="key">The key.</param>
	/// <param name="value">The value.</param>
	/// <param name="asText">As text.</param>
    EPtr(const char* key, double value, bool asText = false) : ptrType(Double), key(key), doubleValue(value), asText(asText) {}

	/// <summary>
	/// Initializes a new instance of the <see cref="EPtr"/> struct.
	/// </summary>
	/// <param name="key">The key.</param>
	/// <param name="value">The value.</param>
    EPtr(const char* key, bool value) : ptrType(Bool), key(key), boolValue(value) {}

    EPtr(const char* key, const char* value, int length) : ptrType(MemPtr), key(key), value(value), length(length) {}

	static int parse(const char* text, EPtr* eptrs, int length, const char separator = '|', int eptrStartIndex = 0)
	{
		int index = 0;
		int start = 0;
		int count = 0;
		while (text[index] || index>start)
		{
			if (!text[index] || text[index] == separator)
			{
				eptrs[eptrStartIndex++] = EPtr(NULL, text + start, index - start);
				start = index + 1;

				if (++count == length || !text[index])
				{
					break;
				}
			}

			index = index + 1;
		}

		return count;
	}
};

#endif