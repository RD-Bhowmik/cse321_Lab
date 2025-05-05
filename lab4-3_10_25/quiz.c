#include <stdio.h>

int main(){
	int num1, num2, num3;
	int max, min;
	float avg;
	printf("enter the 3 numebrs: ");
	scanf("%d %d %d", &num1 , &num2, &num3); 
	
	max = num1; 
	if (num2 > max) {
		max = num2;
	} 
	if (num3 > max) {
		max = num3;
	}
	
	min = num1;
	if (num2<min){
		min = num2; 
	}
	if (num3< min) {
		min = num3; 
	}
		
		
	avg = (num1+num2+num3)/3.0;
	
	printf("max: %d\n", max);
	printf("min: %d\n", min);
	printf("avg: %.2f\n", avg);
	
	
	
}