// Priority locks

#include "prioritylock.h"

void
init_prioritylock(struct prioritylock *lk, char *name)
{
  initlock(&lk->lk, "priority lock");
  lk->name = name;
  lk->locked = FALSE;
  lk->pid = ZERO;

  for (int i = ZERO; i < QUEUE_SIZE; ++i)
  {
    lk->queue[i] = ZERO;
  }
}

void
acquire_priority(struct prioritylock *lk)
{
  acquire(&lk->lk);
  boolean first = queue_is_empty(lk);

  if (!is_in_queue(lk, myproc()->pid))
    enqueue(lk, myproc()->pid);

  if (first && !lk->locked)
  {
    lk->locked = TRUE;
    lk->pid = myproc()->pid;
  }

  else 
  {
      while (lk->locked) 
      {
        sleep(lk, &lk->lk);
      }
      release(&lk->lk);
  }
}

void
release_priority(struct prioritylock *lk)
{
  acquire(&lk->lk);

  if (queue_is_empty(lk)) {
    lk->locked = FALSE;
    lk->pid = ZERO;
  }
  else {
    lk->locked = TRUE;
    lk->pid = dequeue(lk);
  }
  wakeup(lk);

  release(&lk->lk);
}

/*
int
holding_priority(struct sleeplock *lk)
{
  int r;
  
  acquire(&lk->lk);
  r = lk->locked && (lk->pid == myproc()->pid);
  release(&lk->lk);
  return r;
}
*/

int 
dequeue(struct prioritylock *lk)
{
  int target = lk->queue[ZERO];

  for (int i = ZERO; i < QUEUE_SIZE-ONE; ++i)
  {
    if (lk->queue[i] == ZERO)
      break;

    lk->queue[i] = lk->queue[i+ONE];
  }

  return target;
}

void 
enqueue(struct prioritylock *lk, int pid)
{
  int index = ZERO;
  for (int i = ZERO; i < QUEUE_SIZE; ++i)
  {
    if (lk->queue[i] > pid) {
      continue;
    }

    else {
      index = i;
      break;
    }
  }

  insert(lk, pid, index);
}

void 
insert(struct prioritylock *lk, int pid, int index)
{
  for (int i = index+ONE; i < QUEUE_SIZE; ++i)
  {
    lk->queue[i] = lk->queue[i-ONE];
  }

  lk->queue[index] = pid;
}

boolean 
queue_is_empty(struct prioritylock *lk)
{
  if (lk->queue[ZERO])
    return TRUE;
  return FALSE;
}

boolean 
is_in_queue(struct prioritylock *lk, int pid)
{
  for (int i = ZERO; i < QUEUE_SIZE; ++i)
  {
    if (pid > lk->queue[i])
      break;
    else if (pid == lk->queue[i])
      return TRUE;
  }

  return FALSE;
}