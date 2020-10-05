#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

void syscall_init (void);

int check_pointer(void *pointer);

// System Call Implementation (Y/N)
// System Call: halt N
void halt (void);
// System Call: exit N
void exit (int status);
// System Call: exec N
pid_t exec (const char *cmd_line);
// System Call: wait N
int wait (pid_t pid);
// System Call: create N
bool create (const char *file, unsigned initial_size);
// System Call: remove N
bool remove (const char *file);
// System Call: open N
int open (const char *file);
// System Call: filesize N
int filesize (int fd);
// System Call: read N
int read (int fd, void *buffer, unsigned size);
// System Call: write N
int write (int fd, const void *buffer, unsigned size);
// System Call: seek N
void seek (int fd, unsigned position);
// System Call: tell N
unsigned tell (int fd);
// System Call: close N
void close (int fd);

#endif /* userprog/syscall.h */
