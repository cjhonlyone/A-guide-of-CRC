/*
demonstrates how the incorrect check value of 0x29B1 may be reported
for the test string “123456789” when it should be 0xE5CC.
*/
#include <stdio.h>
#include <string.h>

#define           poly     0x1021          /* crc-ccitt mask */

/* global variables */
char text[1000];
unsigned short good_crc;
unsigned short bad_crc;
unsigned short text_length;

int main(void)
{
    void go();
    void repeat_character(unsigned char, unsigned short);

    // sprintf(text, "%s", "");
    // go();

    sprintf(text, "%s", "A");
    go();

    // sprintf(text, "%s", "123456789");
    // go();

    // repeat_character(65, 1);
    // go();

    return 0;
}

void go(void)
{
    void update_good_crc(unsigned short);
    void augment_message_for_good_crc();
    void update_bad_crc(unsigned short);

    unsigned short ch, i;

    good_crc = 0xffff;
    bad_crc = 0xffff;
    i = 0;
    text_length= 0;
    while((ch=text[i])!=0)
    {
        update_good_crc(ch);
        update_bad_crc(ch);
        // printf("n\n");
        i++;
        text_length++;
    }
    augment_message_for_good_crc();
    printf(
        "Good_CRC = %04X,  Bad_CRC = %04X,  Length = %u,  Text =%s\n",
       good_crc,         bad_crc,         text_length,  text
    );
}

void repeat_character(unsigned char ch, unsigned short n)
{
    unsigned short i;
    for (i=0; i<n; i++)
    {
        text[i] = ch;
    }
    text[n] = 0;
}

void update_good_crc(unsigned short ch)
{
    unsigned short i, v, xor_flag;

    /*
    Align test bit with leftmost bit of the message byte.
    */
    v = 0x80;

    for (i=0; i<8; i++)
    {
        if (good_crc & 0x8000)
        {
            xor_flag= 1;
        }
        else
        {
            xor_flag= 0;
        }
        good_crc = good_crc << 1;

        if (ch & v)
        {
            /*
            Append next bit of message to end of CRC if it is not zero.
            The zero bit placed there by the shift above need not be
            changed if the next bit of the message is zero.
            */
            good_crc= good_crc + 1;
        }

        if (xor_flag)
        {
            good_crc = good_crc ^ poly;
        }

        /*
        Align test bit with next bit of the message byte.
        */
        v = v >> 1;
    }
}

void augment_message_for_good_crc()
{
    unsigned short i, xor_flag;

    for (i=0; i<16; i++)
    {
        if (good_crc & 0x8000)
        {
            xor_flag= 1;
        }
        else
        {
            xor_flag= 0;
        }
        good_crc = good_crc << 1;

        if (xor_flag)
        {
            good_crc = good_crc ^ poly;
        }
    }
}

void update_bad_crc(unsigned short ch)
{
    /* based on code found at
    http://www.programmingparadise.com/utility/crc.html
    */

    unsigned short i, xor_flag;

    /*
    Why are they shifting this byte left by 8 bits??
    How do the low bits of the poly ever see it?
    */
    ch<<=8;

    for(i=0; i<8; i++)
    {
        if ((bad_crc ^ ch) & 0x8000)
        {
            xor_flag = 1;
        }
        else
        {
            xor_flag = 0;
        }
        bad_crc = bad_crc << 1;
        if (xor_flag)
        {
            bad_crc = bad_crc ^ poly;
        }
        ch = ch << 1;
    }
}