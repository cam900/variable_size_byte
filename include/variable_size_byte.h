/*
	Variable size byte data library

	This library is licensed under zlib license:

	============================================================================

	zlib License

	(C) 2022-present cam900 and contributors

	This software is provided 'as-is', without any express or implied
	warranty.  In no event will the authors be held liable for any damages
	arising from the use of this software.

	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must not
	claim that you wrote the original software. If you use this software
	in a product, an acknowledgment in the product documentation would be
	appreciated but is not required.
	2. Altered source versions must be plainly marked as such, and must not be
	misrepresented as being the original software.
	3. This notice may not be removed or altered from any source distribution.

	============================================================================

	Usage:
	- include this file at C/C++ source
	- [de/en]code_variable_size_byte_[signed/unsigned] to de/encode data

	Format:

	- Unsigned:
	  - 1 byte: 0aaa aaaa
	    - result: a
	  - 2 byte: 1aaa aaaa 0bbb bbbb
	    - result: 128 + a + (b * 128)
	  - 3 byte: 1aaa aaaa 1bbb bbbb 0ccc cccc
	    - result: 16512 + a + (b * 128) + (c * 16384)
	    ...
	  - N byte: 1aaa aaaa 1bbb bbbb 1ccc cccc ... 0NNN NNNN

	- Signed:
	  - 1 byte: 0saa aaaa
	    - result: a
	  - 2 byte: 1saa aaaa 0bbb bbbb
	    - result: 64 + a + (b * 64)
	  - 3 byte: 1saa aaaa 1bbb bbbb 0ccc cccc
	    - result: 8256 + a + (b * 64) + (c * 8192)
	    ...
	  - N byte: 1saa aaaa 1bbb bbbb 1ccc cccc ... 0NNN NNNN
	  - s: Sign bit
	    - result = -(result + 1) if set

*/
#ifndef VARIABLE_SIZE_BYTE_H
#define VARIABLE_SIZE_BYTE_H

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

/*
	decode_variable_size_byte_[signed/unsigned]
	- decode variable size byte data from buffer
	- src (Input): buffer that stored variable size byte data
	- output (Output): where decoded data stores into
	- returns the size
*/
unsigned long long decode_variable_size_byte_unsigned(const unsigned char *src, unsigned long long *output)
{
	unsigned long long in = *src;
	if (in & 0x80) // multi-byte flag?
	{
		size_t len = 1;
		unsigned long long res = in & 0x7f;
		int adj = 7;
		while (in & 0x80) // continuous data?
		{
			len++;
			in = *(++src); // fetch next byte
			res += ((in & 0x7f) << adj) + (1 << adj); // add to result
			adj += 7;
		}
		*output = res;
		return len;
	}

	*output = in;
	return 1;
}

unsigned long long decode_variable_size_byte_signed(const unsigned char *src, signed long long *output)
{
	unsigned long long in = 0; // data from file
	size_t size = decode_variable_size_byte_unsigned(src, &in);
	unsigned char flag = in & 0x40;                         // sign bit
	// convert to signed
	in = (in & 0x3f) | ((in & ~0x7f) >> 1);
	if (flag)
	{
		*output = (-in) - 1;
	}
	else
	{
		*output = in;
	}
	return size;
}

/*
	encode_variable_size_byte_[signed/unsigned]
	- encode variable size byte data and store into buffer
	- dst (Output): buffer that where encoded data stores into
	- input (Input): data to encode
	- returns the size
*/
unsigned long long encode_variable_size_byte_unsigned(unsigned char *dst, unsigned long long input)
{
	if (input < 0x80) // 0-7f : Single byte
	{
		*dst = input;
		return 1;
	}
	else // 80 or larger: Multi byte
	{
		unsigned long long prv = 0x80;
		unsigned long long tmp = 0x80;
		int adj = 7;
		int siz = 0;
		unsigned long long ret = 0;
		while (input >= tmp)
		{
			prv = tmp;
			siz++;
			adj += 7;
			tmp += 1 << adj;
		}
		input -= prv;
		while (siz)
		{
			*(dst++) = 0x80 | (input & 0x7f);
			siz--;
			input >>= 7;
			ret++;
		}
		*dst = (input & 0x7f);
		ret++;
		return ret;
	}
}

unsigned long long encode_variable_size_byte_signed(unsigned char *dst, signed long long input)
{
	unsigned char sign = 0;       // sign bit
	if (input < 0) // input is negative?
	{
		input = -(input + 1);
		sign = 0x40;
	}
	// convert to unsigned
	unsigned long long in = (input & 0x3f) | ((input & ~0x3f) << 1) | sign;
	return encode_variable_size_byte_unsigned(dst, in);
}

#ifdef __cplusplus
}
#endif

#endif