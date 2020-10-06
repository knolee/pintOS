#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

#include <stdbool.h>

struct fd {
  int fdesc;
  tid_t thread;
  struct file *f;
}

void syscall_init (void);
// System Call Implementation (Y/N)
// System Call: halt N
void halt(void);
// System Call: exit N
void exit(int status);
// System Call: exec N
// pid_t exec(const char *cmd_line);
bool create (const char *file, unsigned initial_size);
// System Call: create N
// System Call: remove N
// System Call: open N
// System Call: filesize N
// System Call: read N
// System Call: write N
// System Call: seek N
// System Call: tell N
// System Call: close N

#endif /* userprog/syscall.h */
