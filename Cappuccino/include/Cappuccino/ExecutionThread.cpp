#include "ExecutionThread.h"
#include "Registration.h"

#include <exception>

void ThreadExecution(ExecutionThread* thr)
{
	const executionFunctionStackList calls = thr->GetCalls();
	const uint64_t* functionCallCount = thr->GetFunctionCallCount();
	executionFunctionStack localCallStack = calls[*thr->GetFunctionStart()];
	
	std::vector<uint64_t> callstackBlockIdx;
	std::vector<uint64_t> callstackStackIdx;

	callstackBlockIdx.push_back(1);
	callstackStackIdx.push_back(*thr->GetFunctionStart());

	bool successful = false;
	const std::atomic<bool>& finished = thr->GetFinished();

	ModBlockData** regData = Registration::GetData();
	ModBlockPass* pass = thr->GetPass();

	printf("starting thread\n");

	while (!finished)
	{
		if (functionCallCount[callstackStackIdx.back()] == callstackBlockIdx.back())
		{
			callstackStackIdx.pop_back();
			callstackBlockIdx.pop_back();

			if (callstackBlockIdx.size() == 0)
			{
				thr->SetFinished(true);
				successful = true;
				break;
			}
		}

		printf("updating pass\n");

		pass->SetData(regData[callstackStackIdx.back()][callstackBlockIdx.back()].GetCData());
		
		printf("executing\n");
		
		localCallStack[callstackBlockIdx.back()](pass);
		callstackBlockIdx.back()++;
	}

	thr->SetFinished(true);

	printf("ending thread\n");

	if (!successful)
		thr->GetPass()->LogInfo("thread terminated successfully");

	Registration::UnRegisterPass(thr->GetPass());
	Registration::UnRegisterExecutionThread(thr);
}

ExecutionThread::ExecutionThread(uint64_t* functionStart, uint64_t* functionCallCount, executionFunctionStackList calls, ModBlockPass* pass)
	:m_finished(false), m_functionStart(functionStart), m_functionCallCount(functionCallCount), m_calls(calls), m_pass(pass)
{
	m_thread = std::thread(ThreadExecution, this);
}

const uint64_t* ExecutionThread::GetFunctionStart()
{
	return m_functionStart;
}

const uint64_t* ExecutionThread::GetFunctionCallCount()
{
	return m_functionCallCount;
}

const executionFunctionStackList& ExecutionThread::GetCalls()
{
	return m_calls;
}

const std::atomic<bool>& ExecutionThread::GetFinished()
{
	return m_finished;
}

ModBlockPass* ExecutionThread::GetPass()
{
	return m_pass;
}

void ExecutionThread::SetFinished(bool finished)
{
	m_finished = finished;
}

void ExecutionThread::End()
{
	m_finished = true;

	if (m_thread.joinable())
		m_thread.join();
	else
		printf("tried to join unjoinable thread");
}
