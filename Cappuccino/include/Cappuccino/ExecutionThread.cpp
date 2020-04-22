#include "ExecutionThread.h"

#include <exception>

void ThreadExecution(ExecutionThread* thr)
{
	const executionFunctionStackList calls = thr->GetCalls();
	const uint64_t* functionCallCount = thr->GetFunctionCallCount();
	executionFunctionStack localCallStack = calls[thr->GetFunctionStart()];
	
	std::vector<uint64_t> callstackBlockIdx;
	std::vector<uint64_t> callstackStackIdx;

	callstackBlockIdx.push_back(1);
	callstackStackIdx.push_back(thr->GetFunctionStart());

	bool successful = false;

	while (!thr->GetFinished())
	{
		if (functionCallCount[callstackStackIdx.back()] == callstackBlockIdx.back())
		{
			callstackStackIdx.pop_back();
			callstackBlockIdx.pop_back();

			if (callstackBlockIdx.size() == 0)
			{
				thr->SetFinished(true);
				successful = true;
				continue;
			}
		}

		localCallStack[callstackBlockIdx.back()](thr->GetPass());
		callstackBlockIdx.back()++;
	}

	if (!successful)
		thr->GetPass()->LogInfo("thread terminated successfully");
}

ExecutionThread::ExecutionThread(uint64_t functionStart, uint64_t* functionCallCount, executionFunctionStackList calls, ModBlockPass* pass)
	:m_finished(false), m_functionStart(functionStart), m_functionCallCount(functionCallCount), m_calls(calls), m_pass(pass)
{
	m_thread = std::thread(ThreadExecution, this);
}

const uint64_t& ExecutionThread::GetFunctionStart()
{
	return m_functionStart;
}

const uint64_t* ExecutionThread::GetFunctionCallCount()
{
	return m_functionCallCount;
}

const executionFunctionStackList ExecutionThread::GetCalls()
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