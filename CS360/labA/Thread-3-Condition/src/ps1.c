#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "printqsim.h"

/* This "solution" basically just allows us to compile the lab.
   It won't solve the problem, but it will compile and create
   an executable.  The users will all be told that their print
   jobs have been submitted, but they haven't.  The printers simply
   exit. */

void initialize_v(Spq *s)
{
  s->v = NULL;
}

void submit_job(Spq *s, Job *j)
{
  return;
}

Job *get_print_job(Spq *s)
{
 
  return NULL;
}
