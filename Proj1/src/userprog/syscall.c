#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include <string.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

void
halt (void)
{
  shutdown_power_off();
}

void
exit (int status)
{
  char* process_name;
  char* ptr;
  process_name = strtok_r(thread_name()," ",&ptr);

  printf("%s: exit(%d)\n",process_name,status);
  thread_current()->exit_status = status;
  thread_exit();
}

pid_t
exec (const char *cmd_line)
{
  char* token;
  char* ptr;
  char *fn_copy;

  fn_copy = palloc_get_page (0);
  if (fn_copy == NULL)
    return TID_ERROR;

  strlcpy(fn_copy, cmd_line, strlen(cmd_line)+1);
  token = strtok_r(fn_copy, " ", &ptr);
  struct file *file = NULL;
  file = filesys_open(token);
  if(file==NULL){
    return -1;
  }
  return ((pid_t)process_execute(cmd_line));
}

int
wait (pid_t pid)
{
  return process_wait((tid_t)pid);
}

int
read (int fd, void *buffer, unsigned size)
{
  int i=0;
  if (fd == 0){
    for (i=0; i<size; i++){
      if(input_getc() == '\0') break;
    }
    return i;
  }
  return -1;

}

int
write (int fd, const void *buffer, unsigned size) {  
  if (fd == 1) {
    putbuf(buffer, size);
    return size;
  }
  return -1; 
}

int
fibonacci(int n){
  int fib1 = 0;
  int fib2 = 1;

  for (int i=0; i<n; i++){
    fib1 += fib2;
    fib2 = fib1-fib2;
  }
  
  return fib1;
}

int
max_of_four_int(int a, int b, int c, int d){
  int array[4] = {a,b,c,d};
  int max = a;
  for (int i=1; i<4; i++){
    if (array[i]>max) max = array[i];
  }
  return max;
}

static void
syscall_handler (struct intr_frame *f) 
{
  int syscall = (*(uint32_t *)f->esp);
  switch(syscall){
    case SYS_HALT:
    {
      halt();
      break;
    }
    case SYS_EXIT:
    {
      if (!is_user_vaddr(f->esp+4)) exit(-1);
      exit(*(uint32_t *)(f->esp+4));
      break;
    }
    case SYS_EXEC:
    {
      if (!is_user_vaddr(f->esp+4)) exit(-1);
      f->eax = exec((const char*)*(uint32_t*)(f->esp+4));
      break;
    }
    case SYS_WAIT:
    {
      if (!is_user_vaddr(f->esp+4)) exit(-1);
      f->eax = wait(*(pid_t *)(f->esp+4));
      break;
    }
    case SYS_WRITE:
    {
      f->eax = write(*(uint32_t *)(f->esp+4), (void *)*(uint32_t *)(f->esp + 8), (unsigned)*((uint32_t *)(f->esp + 12)));
      break;
    }
    case SYS_READ:
    {
      if (!is_user_vaddr(f->esp+4)) exit(-1);
      f->eax = read(*(uint32_t *)(f->esp+4), (void *)*(uint32_t *)(f->esp + 8), (unsigned)*((uint32_t *)(f->esp + 12)));
      break;
    }
    case SYS_FIBONACCI:
    {
      if (!is_user_vaddr(f->esp+4)) exit(-1);
      f->eax=fibonacci((int)*(uint32_t*)(f->esp+4));
      break;
    }
    case SYS_MAX_OF_FOUR_INT:
    {
      if (!is_user_vaddr(f->esp+4)) exit(-1);
      f->eax=max_of_four_int((int)*(uint32_t*)(f->esp+4),(int)*(uint32_t*)(f->esp+8),(int)*(uint32_t*)(f->esp+12),(int)*(uint32_t*)(f->esp+16));
      break;
    }
  }
}
