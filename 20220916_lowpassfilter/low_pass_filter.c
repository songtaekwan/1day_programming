#include <stdio.h>
#include <stdlib.h>

float cnt = 0;
int sum = 0;
float pre_avg = 0;
float avg = 0;
float avg_cal;
float alpha;

float MovAvgFilter(int x)
{
    sum = sum + x;
	cnt++;
    printf("sum: %d, cnt: %f\n",sum, cnt);

	avg_cal = (float)sum / cnt;
    printf("avg_cal: %f\n",avg_cal);

	alpha = (cnt - 1) / cnt;
	
	printf("alpha: %f\n",alpha);
	
	avg = alpha * pre_avg + (1 - alpha) * x;
	pre_avg = avg;
	
    return avg;
}

int main()
{
	int x;
	while(1)
	{
	   printf("데이터 입력: ");
	   scanf("%d",&x);
	   printf("avg : %f\r\n", MovAvgFilter(x));
	}
}
