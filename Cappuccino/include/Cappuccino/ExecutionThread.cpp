#include "ExecutionThread.h"
#include "Registration.h"

#include <exception>
#include <ctime>

static void ThreadExecution(ExecutionThread* thr)
{
	const executionFunctionStackList calls = thr->GetCalls();
	const uint64_t* functionCallCount = thr->GetFunctionCallCount();
	executionFunctionStack localCallStack;
	
	std::vector<uint64_t> callstackBlockIdx;
	std::vector<uint64_t> callstackStackIdx;

	bool successful = false;
	const std::atomic<bool>& finished = thr->GetFinished();

	ModBlockData** regData = Registration::GetData();
	ModBlockPass* pass = thr->GetPass();

	pass->SetCallstackStack(&callstackStackIdx);
	pass->SetCallstackBlock(&callstackBlockIdx);
	pass->SetData(regData);
	pass->SetSuccessful(&successful);
	pass->SetFinished((std::atomic<bool>*)&finished);
	pass->SetCallstackLocal(&localCallStack);
	pass->SetCalls(calls);

	pass->AddCallstack(thr->GetFunctionStart(), 0);
	
	while (!finished)
	{
loop:
		if (callstackBlockIdx.back() >= functionCallCount[callstackStackIdx.back()])
		{
			callstackStackIdx.pop_back();
			callstackBlockIdx.pop_back();

			if (callstackBlockIdx.size() == 0)
			{
				thr->SetFinished(true);
				successful = true;
				break;
			}

			callstackBlockIdx.back()++;
			localCallStack = calls[callstackStackIdx.back()];

			continue;
		}

		localCallStack[callstackBlockIdx.back()](pass);
		callstackBlockIdx.back()++;
	}

	if (thr->GetBreaked())
	{
		thr->SetFinished(false);

		while (!*thr->GetResume())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			
			if (thr->GetKill())
			{
				printf("killed thread during break\n");
				return;
			}

			if (thr->GetStep())
			{
				thr->SetStep(false);
				thr->SetFinished(true);
				break;
			}
		}

		if (*thr->GetResume())
			thr->SetBreak(false);

		goto loop;
	}

	pass->PerformDeallocationCallbacks();

	if (successful)
	{
		Registration::UnRegisterPass(pass);
		Registration::UnRegisterExecutionThread(thr, successful);
	}
}

ExecutionThread::ExecutionThread(const uint64_t& functionStart, uint64_t* functionCallCount, executionFunctionStackList calls, ModBlockPass* pass)
	:m_functionStart(functionStart), m_functionCallCount(functionCallCount), m_calls(calls), m_pass(pass)
{
	m_finished = false;
	m_kill = false;
	m_ended = false;
	m_breaked = false;
	m_resume = nullptr;
	m_step = false;

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

const executionFunctionStackList& ExecutionThread::GetCalls()
{
	return m_calls;
}

const std::atomic<bool>& ExecutionThread::GetFinished()
{
	return m_finished;
}

const std::atomic<bool>& ExecutionThread::GetKill()
{
	return m_kill;
}

const std::atomic<bool>& ExecutionThread::GetBreaked()
{
	return m_breaked;
}

const std::atomic<bool>* ExecutionThread::GetResume()
{
	return m_resume;
}

const std::atomic<bool>& ExecutionThread::GetStep()
{
	return m_step;
}

ModBlockPass* ExecutionThread::GetPass()
{
	return m_pass;
}

void ExecutionThread::SetFinished(bool finished)
{
	m_finished = finished;
}

void ExecutionThread::SetStep(bool step)
{
	m_step = step;
}

void ExecutionThread::SetBreak(bool breaked)
{
	m_breaked = breaked;
}

void ExecutionThread::End()
{
	if (m_ended)
		return;
	else
		m_ended = true;

	m_kill = true;
	m_finished = true;

	if (m_thread.joinable())
		m_thread.join();
	else
		printf("tried to join unjoinable thread\n");
}

void ExecutionThread::Break(std::atomic<bool>* resume)
{
	m_resume = resume;
	m_breaked = true;
	m_finished = true;
}

void ExecutionThread::Step()
{
	m_step = true;

	while (m_step)
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
}