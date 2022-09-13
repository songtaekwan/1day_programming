#include <stdio.h>
#include <stdint.h>

#define MASK_LENGTH 5

uint16_t raw_array[MASK_LENGTH] = {0,};
uint16_t raw_array_index = 0;

float MovAvgFilter()
{
    int i = 0;
    uint16_t sum = 0;

    for (i = 0; i < MASK_LENGTH; i++) {
    	sum += raw_array[i];
    }
    return ((float)sum / MASK_LENGTH);
}

void insertIntoRawArray(uint16_t value)
{
    raw_array[raw_array_index] = value;

    raw_array_index++;

    if (raw_array_index >= MASK_LENGTH) {
    	raw_array_index = 0;
    }
}

int main()
{
	for(int i =0; i<10; i++)
	{
	   insertIntoRawArray(rand()%9);
	   printf("%f\r\n", MovAvgFilter());
	}
}
