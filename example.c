/*
	example project using Variable size byte data library

	This project is licensed under zlib license:

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
*/
#include "include/variable_size_byte.h"
#include <stdio.h>
#include <string.h>

int main()
{
	unsigned long long unsigned_ex = 262144;
	signed long long signed_ex = -262144;

	unsigned long long unsigned_res = 0;
	signed long long signed_res = 0;

	unsigned char tempbuffer[16] = {0};

	// display unsigned data
	printf_s("Pre-encoded unsigned value: %d\n", unsigned_ex);

	// encode
	encode_variable_size_byte_unsigned(&tempbuffer[0], unsigned_ex);

	// display encoded data
	for (int i = 0; i < 16; i++)
	{
		printf_s("%02x ", tempbuffer[i]);
	}
	printf_s("\n");

	// decode and display result
	decode_variable_size_byte_unsigned(&tempbuffer[0], &unsigned_res);

	printf_s("Post-encoded unsigned value: %d\n", unsigned_res);

	// clear buffer
	memset(tempbuffer, 0, 16 * sizeof(unsigned char));

	// signed
	printf_s("Pre-encoded signed value: %d\n", signed_ex);

	// encode
	encode_variable_size_byte_signed(&tempbuffer[0], signed_ex);

	// display encoded data
	for (int i = 0; i < 16; i++)
	{
		printf_s("%02x ", tempbuffer[i]);
	}
	printf_s("\n");

	// decode and display result
	decode_variable_size_byte_signed(&tempbuffer[0], &signed_res);

	printf_s("Post-encoded signed value: %d\n", signed_res);

	return 0;
}
