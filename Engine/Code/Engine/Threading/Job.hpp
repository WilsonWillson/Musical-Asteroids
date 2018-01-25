#pragma once
#if !defined( __JOB__ )
#define __JOB__
#include "Engine/Threading/Signal.hpp"
#include "Engine/Core/Thread.hpp"
#include <vector>
#include <atomic>
class Job;

typedef void(*job_work_cb)(void*);
typedef ThreadSafeQueue<Job*> JobQueue;

enum eJobType
{
	JOB_GENERIC = 0,
	JOB_MAIN,
	JOB_IO,
	JOB_RENDER,
	JOB_LOGGER,

	JOB_TYPE_COUNT,
};

enum jobState
{
	JOB_CREATED = 0,
	JOB_DISPACTED,
	JOB_STUCK_ON_DEPENDENT,
	JOB_ENQUEUED,
	JOB_RUNNING,
	JOB_FINISHED,

	JOB_STATE_COUNT
};

//--------------------------------------------------------------------
//--------------------------------------------------------------------
class Job
{
public:
	eJobType type;
	jobState state;
	job_work_cb work_cb;

	void *user_data;

	std::vector<Job*> dependents;
	unsigned int num_dependencies;
	unsigned int reference_count = 1;

public:
	void on_finish();
	void on_dependancy_finished();

	void dependent_on(Job *parent);


	// data and other methods are up to you
	// though hint - most optionals, especially to do with
	// optimization, will happen here.


};

//--------------------------------------------------------------------
//--------------------------------------------------------------------
class JobConsumer
{
public:
	JobConsumer();

	void add_category(eJobType type);
	
	// consumes a single job.  Returns true if a job
	// was consume, false if not job was ready.
	bool consume_job();

	// consumes as many jobs are as available
	// and returns the number consumed.
	unsigned int consume_all();
	void consume_for_ms(unsigned int msLimit);

public:
	std::vector<JobQueue*> categories;
};

//---------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
class JobSystem
{
public:
	JobQueue* queues;
	Signal **signals;
	JobConsumer *generic_consumer;
	unsigned int queue_count;

	bool is_running;
};

//------------------------------------------------------------------------
// BASIC SYSTEM FUNCTIONALITY
//------------------------------------------------------------------------

// Initializes the system.  Setups up all queues, and spins
// up the generic consumer threads. 
//
// If generic_thread_count is positive, spin up that many threads
// If it is negative, spin up the number of logical cores on the machine added to the supplied count
// You should always spin up at least 1.
void JobSystemStartup(unsigned int job_category_count, int generic_thread_count = -1);

// Shuts down the system, allowing all generic jobs to finish
// and asserting that all other groups have no enqueued jobs before returning
void JobSystemShutdown();

// called from the main thread.  Consuems main thread jobs
// and should be called from App::Update
// Also useful if you want to do a main thread callback system.
void JobSystemMainStep();

// If you want to associate a signal with a category
// that will be fired when a job is added to that queue.
void JobSystemSetCategorySignal(unsigned int cat_id, Signal *signal);

// Job creations and dispatching.
Job* JobCreate(eJobType type, job_work_cb work_cb, void *user_data);
void JobDispatchAndRelease(Job *job);

// Creates and immediately dispatches and relases.
void JobRun(eJobType type, job_work_cb cb, void *user_data);

//------------------------------------------------------------------------
// EXTRAS : REF COUNTING
//------------------------------------------------------------------------

// dispatches without releasing the job.  JobDispatchAndRelease
// could be switched to call this.
void JobDispatch(Job *job);

// Releases a reference to the job. 
void JobRelease(Job *job);

//------------------------------------------------------------------------
// EXTRA: UTILITY
//------------------------------------------------------------------------

// Waits on a job to enter the finished state before
// continuing on.
void JobWait(Job *job);

// Same as above, but will release my hold on this job when done
// Functionally similar to ThreadJoin, but for jobs. 
void JobWaitAndRelease(Job *job);

//------------------------------------------------------------------------
// EXTRA: DEBUG
//------------------------------------------------------------------------

// Returns the current number of "live" jobs - all jobs
// that have at least one reference to them.
unsigned int GetLiveJobCount();

// Returns the number of jobs that are alive, but have not yet
// finished their work.
unsigned int GetActiveJobCount();

#endif 
