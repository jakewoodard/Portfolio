#include <pthread.h>

/* This is the main struct for the simulation.  Each thread gets its own 
   copy of this struct, and every copy is identical with the exception of the id.
   You'll note that there's a (void *) here.  That's what you get to define and
   use in your procedures. */

typedef struct {
  int nusers;          // Number of users in the simulation
  int nprinters;       // Number of users in the simulation
  int arrtime;         // Average interarrival time of printer jobs from a user.
  int maxpages;        // Maximum size of a printer job.
  int bufsize;         // Size of the printer queue.
  int nevents;         // Number of print jobs that each user will generate in the simulation.
  int id;              // Id of the user/printer. 
  int starttime;       // Starting time of the simulation (you can ignore this) 
  pthread_mutex_t *stdiolock;   // A mutex to protect stdio (you can ignore this)
  void *v;             // You get to define this and use it.
} Spq;

/* This struct defines a print job.  When a user submits a job, it submits one
   of these.  When a printer "gets" a job, it gets one of these. */

typedef struct {
  int jobsize;         // Number of pages to print.
  int userid;          // Id of the user (duh)
  int jobid;           // Print job number.
} Job;

/* This is called before the before the threads are created.  The 
   argument is the initial Spq struct, which is copied to all of
   the Sqp structs that go to the users/printers. */

extern void initialize_v(Spq *s);

/* This is what a user calls to submit a job.  It should only return when
   the job is on the printer queue. */

extern void submit_job(Spq *s, Job *j); 

/* This is what a printer calls to get jobs to print.  It returns when a job
   to print (otherwise, it blocks until it has a job to print. When the simulation
   is over, it will return NULL. */

extern Job *get_print_job(Spq *);
