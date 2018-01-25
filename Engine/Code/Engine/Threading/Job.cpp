#include "Engine/Threading/Job.hpp"
#include "Engine/Core/Thread.hpp"
#include "Engine/Core/Atomic.hpp"
#include "Engine/Core/Time.hpp"

static JobSystem *gJobSystem = nullptr;


JobConsumer::JobConsumer()
{

}

void JobConsumer::add_category(eJobType type)
{
	categories.push_back(&gJobSystem->queues[type]);
}

bool JobConsumer::consume_job()
{
	Job *job;
	for (unsigned int i = 0; i < categories.size(); i++)
	{
		JobQueue* queue = categories[i];
		if(queue->pop(&job) == false)
			continue;
		AtomicIncrement(&job->reference_count);
		job->state = JOB_RUNNING;
		job->work_cb(job->user_data);

		job->on_finish();
		return true;
	}

	return false;
}


unsigned int JobConsumer::consume_all()
{
	Job *job;
	unsigned int processed_jobs = 0;

	for (unsigned int i = 0; i < categories.size(); i++)
	{
		JobQueue* queue = categories[i];
		while (queue->pop(&job)) 
		{
			AtomicIncrement(&job->reference_count);
			job->state = JOB_RUNNING;
			job->work_cb(job->user_data);
			++processed_jobs;

			job->on_finish();
			delete job;
		}
	}
	return processed_jobs;
}



void JobConsumer::consume_for_ms(unsigned int msLimit)
{
	Job *job;
	unsigned long long startTime = GetCurrentPerformanceCounter();
	unsigned long long currentTime = startTime;

	unsigned long long timeDifference = currentTime - startTime;
	while (msLimit > timeDifference)
	{
		for (unsigned int i = 0; i < categories.size(); i++)
		{
			JobQueue* queue = categories[i];
			if (queue->pop(&job) == false)
				continue;

			AtomicIncrement(&job->reference_count);
			job->state = JOB_RUNNING;
			job->work_cb(job->user_data);

			job->on_finish();

			currentTime = GetCurrentPerformanceCounter();
			timeDifference = currentTime - startTime;
			if (msLimit > timeDifference)
				break;
		}
	}
}

//------------------------------------------------------------------------
static void GenericJobThread(void *signal)
{
	JobConsumer *generic_consumer = gJobSystem->generic_consumer;
	Signal* jobSignal = (Signal*)signal;

	while (gJobSystem->is_running)
	{
		generic_consumer->consume_all();
		jobSignal->wait();
	}

	generic_consumer->consume_all();

}

//------------------------------------------------------------------------
void Job::on_finish()
{
	state = JOB_FINISHED;
	AtomicDecrement(&reference_count);
	for (unsigned int i = 0; i < dependents.size(); ++i) {
		dependents[i]->on_dependancy_finished();
	}
}

//------------------------------------------------------------------------
void Job::on_dependancy_finished()
{
	JobDispatchAndRelease(this);
}

//------------------------------------------------------------------------
void Job::dependent_on(Job *parent)
{
	AtomicIncrement(&num_dependencies);
	parent->dependents.push_back(this);
}

//------------------------------------------------------------------------
void JobSystemStartup(unsigned int job_category_count, int generic_thread_count /*= -1*/)
{
	int core_count = (int)std::thread::hardware_concurrency();
	if (generic_thread_count <= 0) {
		core_count += generic_thread_count;
	}
	core_count--; // one is always being created - so subtract from total wanted;

	 // We need queues! 
	gJobSystem = new JobSystem();
	gJobSystem->queues = new ThreadSafeQueue<Job*>[job_category_count];
	gJobSystem->signals = new Signal*[job_category_count];
	gJobSystem->queue_count = job_category_count;
	gJobSystem->is_running = true;

	for (unsigned int i = 0; i < job_category_count; ++i) {
		gJobSystem->signals[i] = nullptr;
	}

	// create the signal
	gJobSystem->signals[JOB_GENERIC] = new Signal();
	gJobSystem->signals[JOB_LOGGER] = new Signal();
	gJobSystem->signals[JOB_MAIN] = new Signal();
	gJobSystem->signals[JOB_RENDER] = new Signal();

	JobConsumer *generic_consumer = new JobConsumer();
	generic_consumer->add_category(JOB_GENERIC);
	generic_consumer->add_category(JOB_LOGGER); 
	generic_consumer->add_category(JOB_MAIN);
	generic_consumer->add_category(JOB_RENDER);
	gJobSystem->generic_consumer = generic_consumer;


	ThreadCreate(GenericJobThread, gJobSystem->signals[JOB_GENERIC]);
	ThreadCreate(GenericJobThread, gJobSystem->signals[JOB_LOGGER]);
	ThreadCreate(GenericJobThread, gJobSystem->signals[JOB_MAIN]);
	ThreadCreate(GenericJobThread, gJobSystem->signals[JOB_RENDER]);
/*

	for (int i = 0; i < core_count; ++i) {
		ThreadCreate(GenericJobThread, gJobSystem->signals[JOB_GENERIC]);
	}*/
}

//------------------------------------------------------------------------
void JobSystemShutdown()
{
	gJobSystem->generic_consumer->consume_all();

	delete gJobSystem->generic_consumer;
	delete[] gJobSystem->signals;


	//delete gJobSystem->queues;
}


void JobSystemMainStep()
{
	gJobSystem->generic_consumer->consume_job();
}

void JobSystemSetCategorySignal(unsigned int cat_id, Signal *signal)
{
	gJobSystem->signals[cat_id] = signal;
}

//------------------------------------------------------------------------
Job* JobCreate(eJobType type, job_work_cb work_cb, void *user_data)
{
	Job *job = new Job();
	job->type = type;
	job->work_cb = work_cb;
	job->user_data = user_data;
	job->num_dependencies = 1;
	job->reference_count = 0;
	job->state = JOB_CREATED;
	return job;
}

//------------------------------------------------------------------------
void JobDispatchAndRelease(Job *job)
{
	// if I'm not ready to run, don't. 
 	unsigned int dcount = 0;
	dcount = AtomicDecrement(&job->num_dependencies);
	if (dcount != 0) {
		job->state = JOB_STUCK_ON_DEPENDENT;
		return;
	}

	dcount = AtomicIncrement(&job->num_dependencies);
	gJobSystem->queues[job->type].push(job);
	job->state = JOB_ENQUEUED;
	Signal *signal = gJobSystem->signals[job->type];
	if (nullptr != signal) {
		signal->signal_all();
	}
}


void JobDispatch(Job *job)
{
	unsigned int dcount = AtomicIncrement(&job->num_dependencies);
	if (dcount != 0) {
		job->state = JOB_STUCK_ON_DEPENDENT;
		return;
	}

	gJobSystem->queues[job->type].push(job);
	job->state = JOB_ENQUEUED;
}


void JobRelease(Job *job)
{
	AtomicDecrement(&job->num_dependencies);
	Signal *signal = gJobSystem->signals[job->type];
	if (nullptr != signal) {
		signal->signal_all();
	}
}


void JobWait(Job *job)
{
	while (job->state != JOB_FINISHED)
	{
	}
}


void JobWaitAndRelease(Job *job)
{
	Signal *signal = gJobSystem->signals[job->type];
	if (nullptr != signal) {
		signal->signal_all();
	}

	while (job->state != JOB_FINISHED)
	{

	}
}

void JobRun(eJobType type, job_work_cb cb, void *user_data)
{
	Job *job = new Job();
	job->type = type;
	job->work_cb = cb;
	job->user_data = user_data;
	job->num_dependencies = 1;
	job->reference_count = 1;
	job->state = JOB_CREATED;
	
	AtomicIncrement(&job->reference_count);
	job->work_cb(job->user_data);

	job->on_finish();
	return;
}

