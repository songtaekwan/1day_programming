#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MASK_LENGTH 5

uint16_t raw_array[MASK_LENGTH] = {0,};
uint16_t raw_array_index = 0;

float movingAverageFilter()
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
	insertIntoRawArray(1);
	insertIntoRawArray(2);
	insertIntoRawArray(3);
	insertIntoRawArray(4);
	insertIntoRawArray(5);
	insertIntoRawArray(6);
	printf("%f\r\n", movingAverageFilter());

	insertIntoRawArray(7);
	printf("%f\r\n", movingAverageFilter());

	insertIntoRawArray(8);
	printf("%f\r\n", movingAverageFilter());

	insertIntoRawArray(9);
	printf("%f\r\n", movingAverageFilter());

	insertIntoRawArray(10);
	printf("%f\r\n", movingAverageFilter());
}
