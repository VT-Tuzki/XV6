#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"
#define SEM_MAX_NUM 128

int sem_in_use = 0;	

int sh_var_int = 1;


struct sem{
	struct spinlock lock;
	int resource_count;
	int proc_queue_len;
	int allocated;
	int procs[];
};
struct sem sems[SEM_MAX_NUM];
struct spinlock sems_lock;


int sys_create_sem(){
	initlock(&sems_lock,"sems");
	
	int n_sem,i;
	if(argint(0,&n_sem)<0)
		return -1;
	for(i=0;i<SEM_MAX_NUM;i++){
		if(sems[i].allocated == 0){
			sems[i].allocated =1;
			sems[i].resource_count = n_sem;
			cprintf("create %d sem \n",i);
			return i;
		}
	}
	return 0;
}

int 
sys_free_sem(){
	int id;
	if(argint(0,&id)<0)
		return -1;
	acquire(&sems[id].lock);
	if(sems[id].allocated == 1&&sems[id].resource_count >0){
		sems[id].allocated = 0;
		cprintf("free %d sem\n",id);
	}
	release(&sems[id].lock);
	return 0;
}

int
sys_sem_p(){
	int id;
	if(argint(0,&id)<0)
		return - 1 ;
	acquire (&sems[id].lock) ;
	sems[id].resource_count -- ;
	if(sems[id].resource_count<0){
		sleep(&sems[id],&sems[id].lock); 
	}
	release(&sems[id].lock); 
return 0;
}
 
int
sys_sem_v(){
	int id;
	if (argint(0, &id)<0)
		return -1;
	acquire(&sems[id].lock) ;
	sems[id].resource_count += 1;
	if(sems[id].resource_count <= 0) 
		wakeup(&sems[id]);
	release(&sems [id].lock); 
	return 0;
}



int
sys_sh_var_read(){
	return sh_var_int;
}

int 
sys_sh_var_write(){
	sh_var_int+=1;
	return sh_var_int;
}

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

