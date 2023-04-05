#include "ThreadPool.h"

using namespace Threading;
using namespace Resources;

void ThreadPool::AddTask(function<void()> object)
{
	GetInstance().IAddTask(object);
}

Threading::ThreadPool::ThreadPool()
{
	active = true;

	if (thread::hardware_concurrency() >= 6)
	{
		for (int i = 0; i < 6; i++)
		{
			threads.push_back(thread(&ThreadPool::Worker, this));
		}
	}
	else
	{
		for (int i = 0; i < thread::hardware_concurrency(); i++)
		{
			threads.push_back(thread(&ThreadPool::Worker, this));
		}
	}
	
}

Threading::ThreadPool::~ThreadPool()
{
	active = false;
	for (int i = 0; i < threads.size(); i++)
	{
		threads[i].join();
	}
}

void ThreadPool::IAddTask(function<void()> object)
{
	taskQueue.push( object );
}

void ThreadPool::Worker()
{  
	while (active)
	{
		while(flag.test_and_set()){}
		if (taskQueue.size() > 0)
		{
			function<void()> task = taskQueue.front();
			cout << "\nSet Flag [" << this_thread::get_id() << "]";
			taskQueue.pop();
			cout << "\nClear Flag [" << this_thread::get_id() << "]\n";
			flag.clear();
			//this_thread::sleep_for(std::chrono::seconds(1));
			if (task != NULL)
				task();
				
		}
		else
		{
			flag.clear();
		}
	}
}
