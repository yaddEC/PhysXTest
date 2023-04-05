#pragma once
#include <atomic>
#include <vector>
#include <queue>
#include <thread>
#include <functional>
#include "IResource.h"

using namespace std;
using namespace Resources;

namespace Threading
{
	class ThreadPool
	{

	//Signgleton structure
	public:
		ThreadPool(const ThreadPool&) = delete;

		static ThreadPool& GetInstance()
		{
			static ThreadPool instance;
			return instance;
		}

		static void AddTask(function<void()> object);

	private:
		atomic_bool active;
		queue<function<void()>> taskQueue;
		vector<thread> threads;
		vector<bool> working;
		atomic_flag flag = ATOMIC_FLAG_INIT;

		ThreadPool();
		~ThreadPool();
		
		void IAddTask(function<void()> object);
		void Worker();
	};
}