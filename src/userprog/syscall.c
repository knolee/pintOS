#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "threads/init.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  // See which system call is needed.
  // System call causes interrupt and stores type of call in ESP reg
  void *esp = f->esp;

  check_pointer(esp);

  // Switch depending on system call
  switch (*(int *)esp) {
    case SYS_HALT:
      halt();
      break;
  }

  printf ("system call!\n");
  thread_exit ();
}

// Check if a user provided pointer is correct.
// Check if NULL or not below PHYS_BASE
// If so, free memory and exit process
int check_pointer(void *pointer) {
  if (pointer == NULL || !is_user_vaddr(pointer)) {
    process_exit();

    return 0;
  }

  return 1;
}

void halt(void) {
  shutdown_power_off();
}
