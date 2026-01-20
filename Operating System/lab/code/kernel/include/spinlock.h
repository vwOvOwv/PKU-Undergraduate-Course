#ifndef __SPINLOCK_H
#define __SPINLOCK_H

struct cpu;

// Mutual exclusion lock.
struct spinlock {
  uint locked;       // Is the lock held?

  // For debugging:
  char *name;        // Name of lock.
  struct cpu *cpu;   // The cpu holding the lock.
};

// Initialize a spinlock 
void initlock(struct spinlock*, char*);

// Acquire the spinlock
// Must be used with release()
void acquire(struct spinlock*);

// Release the spinlock 
// Must be used with acquire()
void release(struct spinlock*);

// Check whether this cpu is holding the lock 
// Interrupts must be off 
int holding(struct spinlock*);

#endif
