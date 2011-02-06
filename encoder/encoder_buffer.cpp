/***************************************************************************
 *   Copyright (C) 2007 by Diolan                                          *
 *   www.diolan.com                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "osdep/osdep.h"

#include "encoder.h"
#include "encoder_buffer.h"

#pragma warning(disable:4307)

const XTEA_KEY_T XTEA_ITER = 64;
const XTEA_KEY_T DELTA = 0x9E3779B9;

void EncoderBuffer::encode(XTEA_KEY_T key[4])
{
	XTEA_KEY_T sum;
	XTEA_KEY_T data[2];

	size_t address;

	FragBuffer::iterator it = begin();
	while (it != end())
	{
		address = this->address(it);
		address &= 0xFFFFFFF8U;

		if (read((unsigned char *)&data[0], 8, address, true) == 0)
			continue;
		sum = 0;
		for(XTEA_KEY_T i = 0; i < XTEA_ITER; i++ )
		{

			data[0] += (((data[1]<<4) ^ (data[1]>>5)) + data[1]) ^ (sum + key[sum&0x03]);
			sum+= DELTA;
			data[1] += (((data[0]<<4) ^ (data[0]>>5)) + data[0]) ^ (sum + key[(sum>>11)&0x03]);
		}
		write((unsigned char *)&data[0], 8, address);
		it = lower_bound(address + 8);
	}
}

void EncoderBuffer::decode(XTEA_KEY_T key[4])
{
	XTEA_KEY_T sum;
	XTEA_KEY_T data[2];

	size_t address;

	FragBuffer::iterator it = begin();
	while (it != end())
	{
		address = this->address(it);
		address &= 0xFFFFFFF8U;

		if (read((unsigned char *)&data[0], 8, address, true) == 0)
			continue;

		sum = DELTA * XTEA_ITER;
		for(XTEA_KEY_T i = 0; i < XTEA_ITER; i++ )
		{
			data[1] -= (((data[0]<<4) ^ (data[0]>>5)) + data[0]) ^ (sum + key[(sum>>11)&0x03]);
			sum-= DELTA;
			data[0] -= (((data[1]<<4) ^ (data[1]>>5)) + data[1]) ^ (sum + key[sum&0x03]);
		}
		write((unsigned char *)&data[0], 8, address);
		it = lower_bound(address + 8);
	}
}

/* End of file */
