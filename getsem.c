#include "types.h"
#include "stat.h"
#include "user.h"

int main(){

	printf(1,"sta//sh_var_int = %d\n", sh_var_read());
	int id = create_sem(1);
	int pid = fork();
	int i = 0;
	while(i<10000){
		sem_p(id);
		sh_var_write();
		sem_v(id);
		i++;
	}
	if(pid>0){
		wait();
		free_sem(id);
	}
	printf(1,"\nend//sh_var_int = %d \n", sh_var_read());
	exit();
}

