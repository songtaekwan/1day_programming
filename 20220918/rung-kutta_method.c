#include <stdio.h>

#define len 20  //시행횟수

float fa(float b, float k, float m, float v, float x)
{
	float a;
	
	a = -(b*v + k*x) / m;
	printf("a: %f\n",a);
	return a;
}
float frk(float b, float k, float m)
{
	float dt = 0.01;  
	float t[len] = {0.0,};      //초기 시간
	float v[len] = {0.0,};      //초기 속도
	float x[len] = {0.0,};      //초기 변위
	
	float v1, v2, v3, v4, x1, x2, x3, x4;
	x[0] = 0.1;
	
	for(int i=0; i<len; i++)
	{
		v1=fa(b,k,m,v[i],x[i])*dt;
		x1=v[i]*dt;
		
		v2=fa(b,k,m,v[i]+(v1/2),x[i]+(x1/2))*dt;
	    x2=(v[i]+(v1/2))*dt;
			  
		v3=fa(b,k,m,v[i]+(v2/2),x[i]+(x2/2))*dt;
	    x3=(v[i]+(v2/2))*dt;
			  
		v4=fa(b,k,m,v[i]+v3,x[i]+x3)*dt;
	    x4=(v[i]+v3)*dt;
			  
		t[i+1] = t[i] + dt;
		v[i+1] = v[i] + ((1.0/6.0)*(v1+ 2*v2 + 2*v3 + v4));
	    x[i+1] = x[i] + ((1.0/6.0)*(x1+ 2*x2 + 2*x3 + x4));
		
		printf("t%d: %f\n",i, t[i]);
		printf("v%d: %f\n",i, v[i]);
		printf("x%d: %f\n\n\n",i, x[i]);
	}
	/*
	printf("시간\n");
	for(int i=0; i<len; i++)
	{
		printf("%f\n", t[i]);
	}
	printf("속도\n");
	for(int i=0; i<len; i++)
	{
		printf("%f\n", v[i]);
	}
	printf("변위\n");
	for(int i=0; i<len; i++)
	{
		printf("%f\n", x[i]);
	}
	*/
} 

int main ()
{
	float b;
	float k;
	float m;
	
	printf("b입력: ");
	scanf("%f",&b);
	printf("k입력: ");
	scanf("%f",&k);
	printf("m입력: ");
	scanf("%f",&m);

	frk(b,k,m);	
}
