#include <stdio.h>
#include <stdlib.h>

#define MASK_LENGTH 10

int raw_array[MASK_LENGTH] = {0,};
int raw_array_index = 0;

float MovAvgFilter()
{
    int i = 0;
    int sum = 0;
    float pre_avg = 0;
	float avg = 0;
	
    for (i = 0; i < raw_array_index ; i++) 
	{
    	sum += raw_array[i];
		printf("sum: %d\n",sum);
    }
	
	pre_avg = (float)sum / (raw_array_index+1);
	avg = pre_avg + (raw_array[raw_array_index]/MASK_LENGTH) - (raw_array[raw_array_index - MASK_LENGTH]/MASK_LENGTH);	
	
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
	int x;
	while(1)
	{
	   printf("데이터 입력: ",x);
	   scanf("%d",&x);
	   insertIntoRawArray(x);
	   printf("avg : %f\r\n", MovAvgFilter());
	}
}
