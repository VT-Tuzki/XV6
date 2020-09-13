#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]){
	printf(1,"sta//sh_var_for_sem_demo = %d\n",sh_var_read());
	
	int pid = fork();
	int i = 0;
	while(i<10000){
		sh_var_write();
		i++;
	}
	if(pid>0){
	wait();
	}
	printf(1,"\nend//sh_var_for_sem_demo = %d\n", sh_var_read());
	exit();
}
