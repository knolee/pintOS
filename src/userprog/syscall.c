#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "threads/init.h"
#include "filesys/file.h"
#include "filesys/filesys.h"
#include "threads/synch.h"

static void syscall_handler (struct intr_frame *);
static struct lock file_lock;

void halt(void) {
  shutdown_power_off();
}

// char* 1 byte, unsigned
bool create (const char *file, unsigned initial_size) {
  return filesys_create(file, initial_size);
}

bool remove (const char *file) {
  return filesys_remove(file);
}

int open (const char *file) {
  lock_acquire(&file_lock);
  struct file *f = filesys_open(file);
  lock_release(&file_lock);
  return (int) f;
}

int filesize (int fd) {
  lock_acquire(&file_lock);
  off_t fsize = file_length(fd);
  lock_release(&file_lock);
  return fsize;
}

int read (int fd, void *buffer, unsigned size) {
  lock_acquire(&file_lock);
  off_t bread = file_read(fd, buffer, size);
  lock_release(&file_lock);
  return bread;
}

int write (int fd, const void *buffer, unsigned size) {
  lock_acquire(&file_lock);
  off_t bwrote = file_write(fd, buffer, size);
  lock_release(&file_lock);
  return bwrote;
}

void seek (int fd, unsigned position) {
  lock_acquire(&file_lock);
  file_seek(fd, position);
  lock_release(&file_lock);
}

unsigned tell(int fd) {
  lock_acquire(&file_lock);
  off_t told = file_tell(fd);
  lock_release(&file_lock);
  return told;
}

void close (int fd) {
  lock_acquire(&file_lock);
  file_close(fd);
  lock_release(&file_lock);
}

void
syscall_init (void) 
{
  lock_init(&file_lock);
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
  // Return register is f->eax
  // enum is 4 bytes
  // first parameter is at esp + 4
  switch (*(int *)esp) {
    case SYS_HALT:
      halt();
      break;
    case SYS_CREATE:
      check_pointer(esp + 4);
      check_pointer(esp + 8);
      f->eax = create(esp + 4, esp + 8);
      break;
    case SYS_REMOVE:
      check_pointer(esp + 4);
      f->eax = remove(esp + 4);
      break;
    case SYS_OPEN:
      check_pointer(esp + 4);
      f->eax = open(esp + 4);
      break;
    case SYS_FILESIZE:
      check_pointer(esp + 4);
      f->eax = filesize(esp + 4);
      break;
    case SYS_READ:
      check_pointer(esp + 4);
      check_pointer(esp + 8);
      check_pointer(esp + 12);
      f->eax = read(esp + 4, esp + 8, esp + 12);
      break;
    case SYS_WRITE:
      check_pointer(esp + 4);
      check_pointer(esp + 8);
      check_pointer(esp + 12);
      f->eax = write(esp + 4, esp + 8, esp + 12);
      break;
    case SYS_SEEK:
      check_pointer(esp + 4);
      check_pointer(esp + 8);
      seek(esp + 4, esp + 8);
      break;
    case SYS_TELL:
      check_pointer(esp + 4);
      f->eax = tell(esp + 4);
      break;
    case SYS_CLOSE:
      check_pointer(esp + 4);
      close(esp + 4);
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
