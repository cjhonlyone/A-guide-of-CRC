#include "stdio.h"
#include "string.h"

#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int

#define s8 signed char
#define s16 signed short
#define s32 signed int

#define f32 float

void printfbinary(u32 s, u32 len, u32 indent)
{
	for (int k=0;k<indent;k++)
		printf(" ");
	for (int m = (len-1); m >= 0; m--)
		if ((((s) & (0x01 << m)) >> m) == 0x0001)
			printf("1");
		else
			printf("0");
}
void printf_(u32 len, u32 indent)
{
	for (int k=0;k<indent;k++)
		printf(" ");
	for (int m = (len-1); m >= 0; m--)
		printf("-");
}
u32 mod2mult(u32 a,u32 b)
{
	u32 result = 0;
	for(int i = 0;i < 32 ;i++)
	{
		if ((a & (0x00000001 << i)) == (0x00000001 << i))
		{
			result = result ^ (b << i);
		}
	}
	return result;
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
//crc16-ccitt x16+x12+x5+1
u16 crc16_ccitt_direct(u8 *data, u8 length, u32 init_value)
{
	u16 i, j;
	u32 crc_reg = init_value;
	u16 crc_gx = 0x1021;
	u16 crc_num = 16;
	u32 crc_bit16, crc_in;
	u8 crc_byte;

	u8 *ptr = data;
	printf("串行 数据长度:%d 初值:0x%04x\n",length,init_value);
	for (i=0;i<length;i++)
		printf("0x%02x ", *ptr++);
	printf("\n");

    // 处理所有字节
	for (j = 0; j<length; j++, data++)
	{
        crc_byte = *data;
        // 处理每个bit
		for (i = 0; i<8; i++)
		{
            // crc_reg 移出的最高位
			crc_bit16 = (crc_reg & (0x0001 << (crc_num -1 ))) >> (crc_num -1 );
			// 下一位移入的bit
            crc_in = ((crc_byte & (0x01 << (7-i))) >> (7-i));

			switch(crc_bit16 + crc_in)
			{
				case 0x00000000:
				case 0x00000002:
                    // 异或为0，只进行左移
					crc_reg = (crc_reg << 1) ;
					break;
				case 0x00000001:
                    // 异或为1，左移后再与g(x)异或
					crc_reg = (crc_reg << 1);
					crc_reg = crc_reg ^ crc_gx;
					break;
				default:
					break;
			}
		}
	}
	printf("CRC = 0x%04x\n\n",crc_reg & 0x0000ffff);
	return crc_reg & 0x0000ffff;
}
u16 crc16_ccitt_nondirect (u8 *data, u8 length, u32 init_value)
{
	
	u16 i, j;
	u32 crc_reg = init_value;
	// u32 crc_reg_r1;
	u16 crc_gx = 0x1021;
	u16 crc_num = 16;
	u8 crc_byte;
	u32 quotient = 0;

	u8 *ptr = data;
	printf("长除 数据长度:%d 初值:0x%04x\n信息码: ",length-2,init_value);
	for (i=0;i<(length-2);i++)
		printf("0x%02x ", *ptr++);
	printf("| 0x00 0x00\n");
	
	// printfbinary(init_value, 16, 0);
	// ptr = data;
	// for (i=0;i<(length);i++)
	// 	printfbinary(*ptr++, 8, 0);
	// printf("\n");

    // 处理所有字节
	for (j = 0; j<length; j++, data++)
	{

		crc_byte = *data;
		// 处理每个bit
		for (i = 0; i<8; i++)
		{
			quotient = quotient << 1;
			// crc_reg_r1 = crc_reg;
            // 左移一位，下一位bit移入crc_reg
            crc_reg = (crc_reg << 1) | ((crc_byte & (0x01 << (7-i))) >> (7-i));
			if ((crc_reg & (0x00000001 << crc_num)) == (0x00000001 << crc_num))
			{
				quotient = quotient | 0x00000001;
                // crc_reg 移出的是1
				// crc_reg 减去（异或）生成多项式gx
				// printfbinary(crc_reg_r1, 16, (j)*8+i+1-1);
				// printf(" 0x%04x\n",crc_reg_r1 & 0x0000ffff);
				// printfbinary(crc_gx, 16, (j)*8+i+1);
				// printf(" 0x%04x\n",crc_gx & 0x0000ffff);
                crc_reg = crc_reg ^ crc_gx;
                // printf_(16, (j)*8+i+1);printf("\n");
                // printfbinary(crc_reg, 16, (j)*8+i+1);
                // printf(" 0x%04x\n",crc_reg & 0x0000ffff);

			}
		}
	}
	printf("CRC = 0x%04x\n\n",crc_reg & 0x0000ffff);
	// printf("quotient = 0x%08x\n", quotient);

	// u32 result = mod2mult(quotient,0x00011021);
	// printf("result: 0x%08x\n",result);
	return crc_reg & 0x0000ffff; 
}

int main()
{
	u8 m[20];
	u8 *ptr = m;
	u8 m_k;

	// printf("result: 0x%08x\n",mod2mult(0x00008c1f,0x00011021));	

	// memset(ptr,0,sizeof(u8)*20);
	// strcpy((char *)ptr, "A");
	// m_k = 1;

	// printf("M(x): ");
	// for (int i = 0; i < m_k; i++)
	// {
	// 	printfbinary(m[i], 8, 0);
	// }
	// printf("\n\n");

	// crc16_ccitt_direct(ptr, m_k, 0x0000);

	// crc16_ccitt_nondirect(ptr, m_k+2, 0x0000);

	// crc16_ccitt_direct(ptr, m_k, 0xffff);

	// crc16_ccitt_nondirect(ptr, m_k+2, 0xffff);

	// memset(ptr,0,sizeof(u8)*20);
	// d[0]='A';//d[1]=0xcf;//d[2]=0xde;//d[3]=0x0f;
	// m_k = 2;

	// printf("M(x): ");
	// for (int i = 0; i < m_k; i++)
	// {
	// 	printfbinary(m[i], 8,0);
	// }
	// printf("\n\n");

	// crc16_ccitt_direct(ptr, m_k, 0x0000);

	// crc16_ccitt_nondirect(ptr, m_k+2, 0x0000);

	// crc16_ccitt_direct(ptr, m_k, 0xffff);

	// crc16_ccitt_nondirect(ptr, m_k+2, 0xffff);

	memset(ptr,0,sizeof(u8)*20);
	strcpy((char *)ptr, "123456789");
	m_k = 9;

	// printf("M(x): ");
	// for (int i = 0; i < m_k; i++)
	// {
	// 	printfbinary(m[i], 8, 0);
	// }
	// printf("\n\n");

	// m[0]=0x84;m[1]=0xCF;m_k=11;
	crc16_ccitt_nondirect(ptr, m_k+2, 0x0000);

	crc16_ccitt_nondirect(ptr, m_k+2, 0x84CF);
	
	crc16_ccitt_direct(ptr, m_k, 0x0000);

	// m[0]=0x84;m[1]=0xCF;m_k=2;


	crc16_ccitt_direct(ptr, m_k, 0xffff);

	crc16_ccitt_nondirect(ptr, m_k+2, 0xffff);

	crc16_ccitt_direct(ptr, m_k, 0xffff);	

	crc16_ccitt_nondirect(ptr, m_k+2, 0x84cf);


	// crc16_ccitt_direct(ptr, m_k, 0x1d0f);

	// crc16_ccitt_nondirect(ptr, m_k+2, 0xffff);

	// crc16_ccitt_direct(ptr, m_k, 0xffff);

	

	// crc16_ccitt_nondirect(ptr+2, m_k, 0x84cf);

	// crc16_ccitt_direct(ptr, m_k, 0xffff);



}
