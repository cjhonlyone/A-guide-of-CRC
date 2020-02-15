


// ff00 0100 b2e3 211c 4866 5501 0002 0003
// 0004 0005 0006 0007 0008 0009 000a 000b
// 000c 000d 000e 000f 0010 0000 0011 0000
// 0012 0000 0013 0014 0015 0016 0017 0018
// 0019 001a 001b 001c 001d 0000 001e 0000
// 001f 0000 0020 0000 0021 0022 0023 0024
// 0025 0026 0000 0027 0000 0028 0000 0029
// 0000 002a 0000 002b 0000 002c 002d 002e
// 002f 0030 0000 0031 0000 0032 3300 0000
// 3400 3500 3600 3700 3800 3900 3a00 3b3c
// 003d 003e 003f 0040 0041 0042 4300 4400
// 4500 4600 4700 4800 4900 4a00 4b00 4c00
// 0000 4d00 4e00 4f00 5000 5100 5200 5300
// 5400 5500 5600 5700 5800 5900 5a00 5b00
// 5c00 5d00 5e00 5f60 6100 0000 0000 0000
// 0000 0000 0000 0000 0000 0000 0000 0000

#include "stdio.h"

#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int

#define s8 signed char
#define s16 signed short
#define s32 signed int

#define f32 float

void printfbinary(u32 s, u32 len)
{
	for (int m = (len-1); m >= 0; m--)
	{
		if ((((s) & (0x01 << m)) >> m) == 0x0001)
			printf("1");
		else
			printf("0");

		// if (((m & 0x03) == 0x00) & (m != 0))
		// 	printf("_");
	}

	// printf("%04x", s & 0x0000ffff);
}
u32 reverseBits_32(u32 n)
{
	n = (n >> 16) | (n << 16);
	n = ((n & 0xff00ff00) >> 8) | ((n & 0x00ff00ff) << 8);
	n = ((n & 0xf0f0f0f0) >> 4) | ((n & 0x0f0f0f0f) << 4);
	n = ((n & 0xcccccccc) >> 2) | ((n & 0x33333333) << 2);
	n = ((n & 0xaaaaaaaa) >> 1) | ((n & 0x55555555) << 1);
	return n;
}
u8 reverseBits_8(u8 n)
{
	n = (n >> 4) | (n << 4);//printfbinary(n, 8);printf("\n");
	n = ((n & 0xcc) >> 2) | ((n & 0x33) << 2);//printfbinary(n, 8);printf("\n");
	n = ((n & 0xaa) >> 1) | ((n & 0x55) << 1);//printfbinary(n, 8);printf("\n");
	return n;
}
//crc-16/modbus x16+x15+x2+1 //5590
u16 crc16_ccitt_serial(u8 *data,u16 length, u8 bit_reverse)
{
	u16 i, j;
	u32 crc_reg = 0xffff;
	u16 crc_x = 0x1021;
	u16 crc_num = 16;
	u32 crc_bit16, crc_in;
	u8 crc_byte;
	for (j = 0; j<length; ++j, ++data)
	{
		if (bit_reverse)
			crc_byte = reverseBits_8(*data);
		else
			crc_byte = *data;

		for (i = 0; i<8; ++i)
		{
			// printfbinary(crc_reg, crc_num);
			// if (((crc_byte & (0x01 << i)) >> i) == 0x01)
			// 	printf(" <- 1");
			// else
			// 	printf(" <- 0");

			// printf("\n");

			crc_bit16 = (crc_reg & (0x0001 << (crc_num -1 ))) >> (crc_num -1 );
			crc_in = ((crc_byte & (0x01 << i)) >> i);

			switch(crc_bit16 + crc_in)
			{
				case 0x00000000:
				case 0x00000002:
					crc_reg = (crc_reg << 1) ;
					
					break;
				case 0x00000001:
					crc_reg = (crc_reg << 1);
					crc_reg = crc_reg ^ crc_x;
					break;
				default:
					break;
			}
		}
		// printf("\n");
	}
	return crc_reg & 0x0000ffff;
}
// *data ：待校验数据的起始地址
// len	 ：待校验数据的长度
u16 crc16_ccitt_changchu (u8 *data, u8 length, u8 bit_reverse)
{
	u16 i, j;
	u32 crc_reg = 0xffff;
	u16 crc_x = 0x1021;
	u16 crc_num = 16;
	u8 crc_byte;
	for (j = 0; j<length; ++j, ++data)
	{
		if (bit_reverse)
			crc_byte = reverseBits_8(*data);
		else
			crc_byte = *data;

		for (i = 0; i<8; ++i)
		{
			//printf("data%d: %02x\n", j, *data);
			//printf("crc_reg: %04x\n", crc_reg);

			// for (int m = 0; m < (8*j+i); ++m)
			// {
			// 	printf(" ");
			// }
			// printfbinary(crc_reg, crc_num);
			// if ((((*data) & (0x01 << i)) >> i) == 0x01)
			// 	printf("1");
			// else
			// 	printf("0");
			// printf("\n");
			// for (int m = 0; m < ((8*j+i) + 1); ++m)
			// {
			// 	printf(" ");
			// }
			// printfbinary(crc_x, crc_num);

			// printf("\n");
			// for (int m = 0; m < ((8*j+i) + 1); ++m)
			// {
			// 	printf(" ");
			// }
			// for (int m = 0; m < 16; ++m)
			// {
			// 	printf("-");
			// }

			// printf("\n");

			if ((crc_reg & (0x0001 << (crc_num -1 ))) == (0x0001 << (crc_num -1 )))
			{
				crc_reg = (crc_reg << 1) | ((crc_byte & (0x01 << i)) >> i);
				crc_reg = crc_reg ^ crc_x;
			}
			else
			{
				crc_reg = (crc_reg << 1) | ((crc_byte & (0x01 << i)) >> i);
			}
		}
	}
	// for (int m = 0; m < ((8*length)); ++m)
	// {
	// 	printf(" ");
	// }
	// printfbinary(crc_reg, crc_num);
	// printf(" = 0x%04x = CRC", crc_reg & 0x0000ffff);
	// printf("\n");

	return crc_reg & 0x0000ffff;                  		//CRC终值
}

void main()
{
	u8 d[10];
	u8 *ptr = d;
	u8 d_len;


	d_len = 1;
	d[0] = 'A';d[1] = 0x00;d[2] = 0x00;d[3] = 0x00; //"A"

	
	for (int i = 0; i < d_len; ++i)
	{
		printfbinary(d[i], 8);
	}
	printf("\n");
	printf("*高位先发，没有补0，串行结构 CRC = 0x%04x\n", crc16_ccitt_serial(ptr, d_len, 1));
	printf("*低位先发，没有补0，串行结构 CRC = 0x%04x\n", crc16_ccitt_serial(ptr, d_len, 0));
	printf("\n");

	printf(" 高位先发，补0，串行结构 CRC = 0x%04x\n", crc16_ccitt_serial(ptr, d_len+2, 1));
	printf(" 低位先发，补0，串行结构 CRC = 0x%04x\n", crc16_ccitt_serial(ptr, d_len+2, 0));
	printf("\n");

	printf(" 高位先发，没有补0，长除 CRC = 0x%04x\n", crc16_ccitt_changchu(ptr, d_len, 1));
	printf(" 低位先发，没有补0，长除 CRC = 0x%04x\n", crc16_ccitt_changchu(ptr, d_len, 0));
	printf("\n");

	printf("*高位先发，补0，长除 CRC = 0x%04x\n", crc16_ccitt_changchu(ptr, d_len+2, 1));
	printf("*低位先发，补0，长除 CRC = 0x%04x\n", crc16_ccitt_changchu(ptr, d_len+2, 0));
	printf("\n");

	d_len = 2;
	d[0] = 0x52;d[1] = 0x66;d[2] = 0x00;d[3] = 0x00; //"A"
	for (int i = 0; i < d_len; ++i)
	{
		printfbinary(d[i], 8);
	}
	printf("\n");
	printf("*高位先发，没有补0，串行结构 CRC = 0x%04x\n", crc16_ccitt_serial(ptr, d_len, 1));
	printf("*低位先发，没有补0，串行结构 CRC = 0x%04x\n", crc16_ccitt_serial(ptr, d_len, 0));
	printf("\n");

	printf(" 高位先发，补0，串行结构 CRC = 0x%04x\n", crc16_ccitt_serial(ptr, d_len+2, 1));
	printf(" 低位先发，补0，串行结构 CRC = 0x%04x\n", crc16_ccitt_serial(ptr, d_len+2, 0));
	printf("\n");

	printf(" 高位先发，没有补0，长除 CRC = 0x%04x\n", crc16_ccitt_changchu(ptr, d_len, 1));
	printf(" 低位先发，没有补0，长除 CRC = 0x%04x\n", crc16_ccitt_changchu(ptr, d_len, 0));
	printf("\n");

	printf("*高位先发，补0，长除 CRC = 0x%04x\n", crc16_ccitt_changchu(ptr, d_len+2, 1));
	printf("*低位先发，补0，长除 CRC = 0x%04x\n", crc16_ccitt_changchu(ptr, d_len+2, 0));
	printf("\n");
	printf("test git");
}