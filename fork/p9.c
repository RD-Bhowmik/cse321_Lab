#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

const int len = 2;

int main() {
	int id;
	int a[] = {5,8};
	int b = len-1;
	id = fork();
	if (id < 0){
		printf("fork failed\n");
	}
	else if(id > 0){
		wait(NULL);
		printf("parent process executing\n");
	}
	else{
		printf("child process executing\n");
		a[b-1]=a[b-1]+2;
		a[b] = a[b]-3;
	}
	for(int i=0;i<len;i++){
		printf("value of a[%d]: %d\n",i,a[i]);
	}
	return 0;
}
