#pragma once
#include "CommonTools.h"

#include <utility>
#include <functional>
#include <vector>
#include <memory>
#include <cassert>
#include <array>
#include <queue>
#include <future>
#include <iostream>

namespace ErrorHandling
{
	using namespace std;


	template <typename T>
	struct ScopeGuard
	{
		T t;
		bool dismissed = false;
		bool finalized = false;
		ScopeGuard(T&& t)
			: t(move(t))
		{
		}
		~ScopeGuard()
		{
			Finalize();
		}

		ScopeGuard(ScopeGuard const&) = delete;
		ScopeGuard& operator=(ScopeGuard const&) = delete;

		ScopeGuard(ScopeGuard&&) = default;
		ScopeGuard& operator=(ScopeGuard&&) = default;

		void Dissmiss()
		{
			dismissed = true;
		}

		void Finalize()
		{
			if (!dismissed)
			{
				if (!finalized)
				{
					t();
					finalized = true;
				}
			}
		}
	};

	template <typename T>
	ScopeGuard<T> CreateScopeGuard(T&& onExit)
	{
		return ScopeGuard<T>(move(onExit));
	}

	enum class ErrorCode
	{
		NoError,

		NoTask,
		WrongTask,

		NoProcessor,
		ProcessorInitializeError,
		ProcessorProcessingError,
	};
	
	struct ITask
	{
		virtual uint32_t GetTaskType() const = 0;
	};

	struct TaskA : ITask
	{
		virtual uint32_t GetTaskType() const
		{
			return 42;
		}
	};

	struct ITaskData
	{
	};

	struct ITaskProcessor
	{
		virtual ErrorCode InitializeForTask(ITask* task) = 0;
		virtual ErrorCode Process(ITask* task, ITaskData* data) = 0;
		virtual void Finalize() = 0;
	};

	struct TaskProcessorA : ITaskProcessor
	{
		virtual ErrorCode InitializeForTask(ITask* task) override
		{
			return ErrorCode::NoError;
		}
		
		virtual ErrorCode Process(ITask* task, ITaskData* data) override
		{
			if (task->GetTaskType() == 42)
			{
				// process task and data
				return ErrorCode::NoError;
			}
			else
			{
				return ErrorCode::WrongTask;
			}
		}

		virtual void Finalize() override
		{
		}
	};

	// a terribly implemented example
	struct TaskScheduler
	{
		static int const UseVersion = 1;

		void Run()
		{
			while (!tasks.empty())
			{
				pair<ITask*, ITaskData*> task = tasks.front();
				tasks.pop();

				future<void> f = async(launch::async, [this, task]
				{
					ErrorCode error = ConsumeTask(task.first, task.second);
				});
				f.wait();
			}
		}

		ITaskProcessor* CreateProcessor(ITask* task)
		{
			if (task->GetTaskType() == 42)
			{
				return new TaskProcessorA;
			}
			return nullptr;
		}


		ErrorCode ConsumeTask(ITask* task, ITaskData* data)
		{
			switch (UseVersion)
			{
			case 1:
				return ConsumeTaskVersion1(task, data);
				break;
			case 2:
				return ConsumeTaskVersion2(task, data);
				break;
			case 3:
				return ConsumeTaskVersion3(task, data);
				break;
			default:
				return ErrorCode::NoError;
				break;
			}
		}

		// a buggy one
		ErrorCode ConsumeTaskVersion1(ITask* task, ITaskData* data)
		{
			if (task == nullptr)
			{
				delete data;
				return ErrorCode::NoTask;
			}
			ITaskProcessor* processor = CreateProcessor(task);
			if (processor == nullptr)
			{
				delete data;
				return ErrorCode::NoProcessor;
			}
			ErrorCode initializeError = processor->InitializeForTask(task);
			if (initializeError != ErrorCode::NoError)
			{
				processor->Finalize();
				delete data;
				return ErrorCode::ProcessorInitializeError;
			}

			ErrorCode processError = processor->Process(task, data);
			if (processError != ErrorCode::NoError)
			{
				processor->Finalize();
				delete processor;
				delete data;
				return ErrorCode::ProcessorProcessingError;
			}

			processor->Finalize();
			delete processor;

			delete task;
			delete data;

			return ErrorCode::NoError;
		}

		ErrorCode ConsumeTaskVersion2(ITask* task, ITaskData* data)
		{
			auto taskAndDataGuard = CreateScopeGuard([&]
			{
				delete task;
				delete data;
			});

			if (task == nullptr)
			{
				return ErrorCode::NoTask;
			}
			ITaskProcessor* processor = CreateProcessor(task);
			auto processorGuard = CreateScopeGuard([&]
			{
				if (processor)
				{
					processor->Finalize();
					delete processor;
				}
			});
			if (processor == nullptr)
			{
				return ErrorCode::NoProcessor;
			}
			ErrorCode initializeError = processor->InitializeForTask(task);
			if (initializeError != ErrorCode::NoError)
			{
				return ErrorCode::ProcessorInitializeError;
			}

			ErrorCode processError = processor->Process(task, data);
			if (processError != ErrorCode::NoError)
			{
				return ErrorCode::ProcessorProcessingError;
			}

			return ErrorCode::NoError;
		}

		ErrorCode ConsumeTaskVersion3(ITask* task, ITaskData* data)
		{
			auto taskP = unique_ptr<ITask>(task);
			auto dataP = unique_ptr<ITaskData>(data);

			if (taskP == nullptr)
			{
				return ErrorCode::NoTask;
			}
			auto pd = [] (ITaskProcessor* p)
			{
				p->Finalize();
				delete p;
			};
			auto processor = unique_ptr<ITaskProcessor, decltype(pd)>(CreateProcessor(task), pd);
			if (processor == nullptr)
			{
				return ErrorCode::NoProcessor;
			}
			ErrorCode initializeError = processor->InitializeForTask(task);
			if (initializeError != ErrorCode::NoError)
			{
				return ErrorCode::ProcessorInitializeError;
			}

			ErrorCode processError = processor->Process(task, data);
			if (processError != ErrorCode::NoError)
			{
				return ErrorCode::ProcessorProcessingError;
			}

			return ErrorCode::NoError;
		}

		queue<pair<ITask*, ITaskData*>> tasks;

	};



	void DirectErrorHandling()
	{
		TaskScheduler scheduler;
		scheduler.tasks.push(make_pair(new TaskA, nullptr));
		scheduler.Run();
	}

	void Run()
	{
		DirectErrorHandling();
	}
}