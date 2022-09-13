#include <stdio.h>
#include <stdlib.h>

#define MASK_LENGTH 10

int raw_array[MASK_LENGTH] = {0,};
int raw_array_index = 0;
int cnt = 1;

float MovAvgFilter()
{
    int i = 0;
    int sum = 0;
    float pre_avg = 0;
	float avg = 0;
	
    for (i = 0; i < cnt ; i++) 
	{
    	sum += raw_array[i];
    }
	
	pre_avg = (float)sum / cnt;
	avg = pre_avg + (raw_array[cnt]/cnt) - (raw_array[0]/cnt);	
	
	pre_avg = avg;
	
    return avg;
}

void insertIntoRawArray(int value)
{
    raw_array[raw_array_index] = value;

    raw_array_index++;

    if (raw_array_index >= MASK_LENGTH) 
	{
    	raw_array_index = 0;
    }
}

int main()
{
	for(int i =0; i<10; i++)
	{
	   insertIntoRawArray(rand()%9);
	   printf("%f\r\n", MovAvgFilter());
	   cnt++;
	}
}
