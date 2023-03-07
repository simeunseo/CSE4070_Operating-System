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
  lock_init(&filesys_lock);
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

  for (int i=3; i<FD_LIMIT; i++){
    if(thread_current()->fdt[i]!=NULL){
      close(i);
    }
  }

/*
  struct thread *t = thread_current();
  struct thread *t_child;
	struct list_elem *begin = list_begin(&(t->child_thread));
  struct list_elem *end = list_end(&(t->child_thread));
	struct list_elem *t_ptr;
  for (t_ptr = begin; t_ptr!=end; t_ptr=list_next(t_ptr)){
    t_child = list_entry(t_ptr, struct thread, child_thread_elem);
    process_wait(t_child->tid);
  }
*/

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
  if (!is_user_vaddr(buffer)) exit(-1);
  struct thread *cur_thread = thread_current();
  struct file* fp = cur_thread->fdt[fd];

  int return_val = -1;

  lock_acquire(&filesys_lock);
  if (fd == 0){
    for (int i=0; i<size; i++){
      if(input_getc() == '\0'){
        return_val = i;
        break;
      }
    }
  } else if (fd>2) {  
    if (fp == NULL) {
      return_val = -1;
    }
    else {
      return_val = file_read(fp, buffer, size);
    }
  }
  else {
    return_val = -1;
  }
  lock_release(&filesys_lock);
  return return_val;
}

int
write (int fd, const void *buffer, unsigned size) { 
  if (!is_user_vaddr(buffer)) exit(-1);

  lock_acquire(&filesys_lock);
  int return_val = -1;
  if (fd == 1) {
    putbuf(buffer, size);
    return_val = size;
  }
  else if (fd > 2){
    if (thread_current()->fdt[fd] == NULL){
      return_val = -1;
    }
    else {
      struct thread *cur_thread = thread_current();
      struct file* fp = cur_thread->fdt[fd];

      if (fp->deny_write){
        return_val = 0;
      } else{
        return_val = file_write(fp, buffer, size);
      }
    }
  }
  else {
    return_val = -1;
  }
  lock_release(&filesys_lock);
  return return_val;
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

bool
create(const char *file, unsigned initial_size) {
  if (file == NULL) exit(-1);
  return filesys_create(file, initial_size);
}

bool
remove(const char *file) {
  if (file == NULL) exit(-1);
  return filesys_remove(file);
}

int
open(const char *file) {
  if (!is_user_vaddr(file)) exit(-1);
  if (file == NULL) return -1;
  lock_acquire(&filesys_lock);
  

  int return_val = -1;
   
  struct thread *cur_thread = thread_current();
  struct file* fp = filesys_open(file);
  if (fp == NULL) {
    lock_release(&filesys_lock);
    return -1;
  }

  if (strcmp(thread_name(), file)==0){
    file_deny_write(fp);
  }
  for(int fd = 3; fd<FD_LIMIT; fd ++){
    if(cur_thread->fdt[fd]==NULL){
      cur_thread->fdt[fd]=fp;
      return_val = fd;
      cur_thread->fd_index = fd+1;
      break;
    }
  }

  lock_release(&filesys_lock);
  return return_val;
}

int
filesize (int fd) {
  struct thread *cur_thread = thread_current();
  struct file* fp = cur_thread->fdt[fd];

  if (fp == NULL) return -1;
  return file_length(fp);
}

void
seek (int fd, unsigned position) {
  struct thread *cur_thread = thread_current();
  struct file* fp = cur_thread->fdt[fd];
  if (fp == NULL) exit(-1);
	file_seek(fp, position);
}

unsigned
tell (int fd) {
  struct thread *cur_thread = thread_current();
  struct file* fp = cur_thread->fdt[fd]; 
  if (fp == NULL) return -1;
  return file_tell(fd);
}

void
close (int fd) {
  struct thread *cur_thread = thread_current();
  struct file* fp = cur_thread->fdt[fd];
  if (fp == NULL){
    exit(-1);
  }
  fp = NULL;
  file_close(fp);
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
      if (!is_user_vaddr(f->esp+4)) exit(-1);
      if (!is_user_vaddr(f->esp+8)) exit(-1);
      if (!is_user_vaddr(f->esp+12)) exit(-1);
      f->eax = write(*(uint32_t *)(f->esp+4), (void *)*(uint32_t *)(f->esp + 8), (unsigned)*((uint32_t *)(f->esp + 12)));
      break;
    }
    case SYS_READ:
    {
      if (!is_user_vaddr(f->esp+4)) exit(-1);
      if (!is_user_vaddr(f->esp+8)) exit(-1);
      if (!is_user_vaddr(f->esp+12)) exit(-1);
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
      if (!is_user_vaddr(f->esp+8)) exit(-1);
      if (!is_user_vaddr(f->esp+12)) exit(-1);
      if (!is_user_vaddr(f->esp+16)) exit(-1);
      f->eax=max_of_four_int((int)*(uint32_t*)(f->esp+4),(int)*(uint32_t*)(f->esp+8),(int)*(uint32_t*)(f->esp+12),(int)*(uint32_t*)(f->esp+16));
      break;
    }
    case SYS_CREATE:
    {
      if (!is_user_vaddr(f->esp+4)) exit(-1);
      if (!is_user_vaddr(f->esp+8)) exit(-1);
      f->eax = create((const char *)*(uint32_t *)(f->esp + 4), (unsigned)*(uint32_t *)(f->esp + 8));
      break;
    }
    case SYS_REMOVE:
    {
      if (!is_user_vaddr(f->esp+4)) exit(-1);
      f->eax = remove((const char*)*(uint32_t *)(f->esp + 4));
      break;
    }
    case SYS_OPEN:
    {
      if (!is_user_vaddr(f->esp+4)) exit(-1);
      f->eax = open((const char*)*(uint32_t*)(f->esp+4));
      break;
    }
    case SYS_FILESIZE:
    {
      if (!is_user_vaddr(f->esp+4)) exit(-1);
      f->eax = filesize((int)*(uint32_t*)(f->esp+4));
      break;
    }
    case SYS_SEEK:
    {
      if (!is_user_vaddr(f->esp+4)) exit(-1);
      if (!is_user_vaddr(f->esp+8)) exit(-1);
      seek((int)*(uint32_t*)(f->esp+4), (unsigned)*(uint32_t *)(f->esp+8));
      break;
    }
    case SYS_TELL:
    {
      if (!is_user_vaddr(f->esp+4)) exit(-1);
      f->eax = tell((int)*(uint32_t *)(f->esp + 4));
      break;
    }
    case SYS_CLOSE:
    {
      if (!is_user_vaddr(f->esp+4)) exit(-1);
      close((int)*(uint32_t *)(f->esp + 4));
      break;
    }
  }
}
