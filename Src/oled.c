/*
 * oled.c
 *
 *  Created on: Jun 2, 2026
 *      Author: kapi-
 */


#include "main.h"
#include "stdint.h"
#include "oled.h"
#include "i2c.h"
#include "ascii.h"

#define OLED_ADDR 0x78
#define OLED_DATA 0x40


void OLED_Command(uint8_t cmd)
{
	uint8_t buff[2];

	buff[0] = 0x00;
	buff[1] = cmd;

	I2C1_WriteBuffer(OLED_ADDR, buff, 2);
}


void OLED_Command2(uint8_t cmd, uint8_t value)
{
	uint8_t buff[3];

	buff[0] = 0x00;
	buff[1] = cmd;
	buff[2] = value;

	I2C1_WriteBuffer(OLED_ADDR, buff, 3);
}

void OLED_Init(void)
{
	OLED_Command(0xAE); // off
	OLED_Command2(0xD5, 0x80); // clock
	OLED_Command2(0xA8, 0x3F); // multiplex
	OLED_Command2(0xD3, 0x00); // offset
	OLED_Command(0x40); // start line
	OLED_Command(0xA1); // segment re-map
	OLED_Command(0xC8); // COM output scan direction
	OLED_Command2(0xDA, 0x12); // COM pins
	OLED_Command2(0x8D, 0x14); // charge pump on
	OLED_Command2(0x20, 0x02); // memory ADRESSING; page ADRESSING mode [reset]
	OLED_Command2(0x81, 0x7F); // contrast control
	OLED_Command(0xA6); // normal display
	OLED_Command(0xA4); // resume RAM content display
	OLED_Command2(0xD9, 0x22); // pre-charge period
	OLED_Command2(0xDB, 0x20); // VCOMH
	OLED_Command(0xAF); // on
}

void OLED_DataBuffer(uint8_t *data, uint8_t length)
{
	uint8_t buff[length + 1];
	uint8_t num = 0;

	buff[0] = OLED_DATA;


	while(num < length)
	{

		buff[num + 1] = data[num];
		num ++;
	}
	I2C1_WriteBuffer(OLED_ADDR, buff, length + 1);
}

void OLED_Clear(void)
{
	uint8_t page;


	uint8_t tab[128];
	uint8_t i;

	for(i = 0; i < 128; i++)
	{
		tab[i] = 0;
	}


	for(page = 0; page < 8; page++)
	{
		OLED_Command(0xB0 + page);
		OLED_DataBuffer(tab, 128);
	}


}

void OLED_Fill(void)
{
	uint8_t page;


	uint8_t tab[128];
	uint8_t i;

	for(i = 0; i < 128; i++)
	{
		tab[i] = 0xFF;
	}


	for(page = 0; page < 8; page++)
	{
		OLED_Command(0xB0 + page);
		OLED_DataBuffer(tab, 128);
	}

}


void OLED_SetCursor(uint8_t page, uint8_t column)
{
	uint8_t lower = column & 0x0F;
	uint8_t higher = column >> 4;

	OLED_Command(0xB0 + page);
	OLED_Command(0x00 + lower);
	OLED_Command(0x10 + higher);
}

void OLED_WriteChar(char c)
{
	uint8_t i = 0;
	uint8_t space = 0x00;

	while(OledChars[i] != '\0')
	{

		if(OledChars[i] == c)
		{
			OLED_DataBuffer((uint8_t*)Ascii[i], 5);
			OLED_DataBuffer(&space, 1);
			return;
		}
		i++;
	}

}

void OLED_WriteString(const char *z)
{

	while(*z != '\0')
	{
		OLED_WriteChar(*z);
		z++;
	}

}

void OLED_WriteInt(uint16_t num)
{
	char BuffNumber[5];

	uint16_t wynik1 = num % 10;
	uint16_t wynik2 = (num / 10) % 10;
	uint16_t wynik3 = (num / 100) % 10;
	uint16_t wynik4 = num / 1000;

	char text1 = wynik4 + '0';
	char text2 = wynik3 + '0';
	char text3 = wynik2 + '0';
	char text4 = wynik1 + '0';
	char text5 = '\0';

	BuffNumber[0] = text1;
	BuffNumber[1] = text2;
	BuffNumber[2] = text3;
	BuffNumber[3] = text4;
	BuffNumber[4] = text5;

	uint8_t start = 0;

	while((BuffNumber[start] == '0') && start < 3)
	{
		start++;
	}

	if(num > 9999)
		{
			OLED_WriteString("OVF");
		}
	else
		{
			OLED_WriteString(start + BuffNumber);
		}

}

void OLED_Write_Int(int32_t num)
{
    char BuffNumber[7];

    if(num < 0)
    {
        OLED_WriteChar('-');
        num = -num;
    }

    uint16_t wynik1 = num % 10;
    uint16_t wynik2 = (num / 10) % 10;
    uint16_t wynik3 = (num / 100) % 10;
    uint16_t wynik4 = (num / 1000) % 10;
    uint16_t wynik5 = (num / 10000) % 10;

    BuffNumber[0] = wynik5 + '0';
    BuffNumber[1] = wynik4 + '0';
    BuffNumber[2] = wynik3 + '0';
    BuffNumber[3] = wynik2 + '0';
    BuffNumber[4] = wynik1 + '0';
    BuffNumber[5] = '\0';

    uint8_t start = 0;

    while((BuffNumber[start] == '0') && (start < 4))
    {
        start++;
    }

    if(num > 99999)
    {
        OLED_WriteString("OVF");
    }
    else
    {
        OLED_WriteString(start + BuffNumber);
    }
}








