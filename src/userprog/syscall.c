#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  uint32_t *esp;
  esp = f->esp;
  
  // check_pointer(esp);
  
  switch(*esp){

    case SYS_HALT:
      printf("syscall: halt\n");
      halt();

    case SYS_EXIT:
      printf("syscall: exit\n");

    case SYS_EXEC:
      printf("syscall: exec\n");

    case SYS_WAIT:
      printf("syscall: wait\n");

    case SYS_CREATE:
      printf("syscall: create\n");

    case SYS_REMOVE:
      printf("syscall: remove\n");

    case SYS_OPEN:
      printf("syscall: open\n");

    case SYS_FILESIZE:
      printf("syscall: filesize\n");

    case SYS_READ:
      printf("syscall: read\n");

    case SYS_WRITE:
      printf("syscall: write\n");

    case SYS_SEEK:
      printf("syscall: seek\n");

    case SYS_TELL:
      printf("syscall: tell\n");

    case SYS_CLOSE:
      printf("syscall: close\n");
  }
}

// Taken from spencers commit, not verified yet
// Check if a user provided pointer is correct.
// Check if NULL or not below PHYS_BASE
// If so, free memory and exit process
/*
int check_pointer (void *pointer) {
  if (pointer == NULL || !is_user_vaddr(pointer)) {
    process_exit();

    return 0;
  }

  return 1;
}
*/

void halt (void) {
  shutdown_power_off();
}
