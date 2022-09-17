#include <stdio.h>
#include <stdlib.h>

float cnt = 0;
int sum = 0;
float pre_avg = 0;
float avg = 0;
float avg_cal;
float alpha;

float MovAvgFilter(int x, float alpha)
{
    sum = sum + x;
	cnt++;
    printf("sum: %d, cnt: %f\n",sum, cnt);

	avg_cal = (float)sum / cnt;
    printf("avg_cal: %f\n",avg_cal);

	avg = alpha * pre_avg + (1 - alpha) * x;
	pre_avg = avg;
	
    return avg;
}

int main()
{
	int x;
	printf("alpha(0< <1): ");
	scanf("%f",&alpha);
	while(1)
	{
	   printf("데이터 입력: ");
	   scanf("%d",&x);
	   printf("avg : %f\r\n", MovAvgFilter(x, alpha));
	}
}
