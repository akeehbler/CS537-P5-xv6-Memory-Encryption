#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "ptentry.h"

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

/******************************************************
*                Added Sys Calls                      *
******************************************************/
//TODO: All of These
int sys_mencrypt(void){
  //TODO: Might have to do some other stuff in here
  // Might have to align virtual addr here, not sure
  char *virtual_addr;
  int len;
  if(argptr(0, (void*)&virtual_addr, sizeof(*virtual_addr)) < 0){
    return -1;
  }
  if(argint(1, &len) < 0){
    return -1;
  }
  return mencrypt(virtual_addr, len);
}

int sys_getpgtable(void){
  struct pt_entry *entries;
  int num;
  if(argptr(0, (void*)&entries, sizeof(*entries)) < 0){
    return -1;
  }
  if(argint(1, &num) < 0){
    return -1;
  }
  return getpgtable(entries, num);
}

int sys_dump_rawphymem(void){
  //TODO: Make sure that the physical addr is aligned here maybe
  //Use argint here probably
  int physical_addr;
  char *buffer;
  if(argint(0, &physical_addr) < 0){
    return -1;
  }
  if(argptr(1, (void*)&buffer, sizeof(*buffer)) < 0){
    return -1;
  }
  return 1;
}
