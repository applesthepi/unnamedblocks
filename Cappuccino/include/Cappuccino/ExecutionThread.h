#pragma once
#include "ModBlockPass.h"

#include <atomic>
#include <cinttypes>
#include <thread>

typedef void(***executionFunctionStackList)(ModBlockPass*);
typedef void(*executionFunction)(ModBlockPass*);
typedef void(**executionFunctionStack)(ModBlockPass*);

class ExecutionThread
{
public:
	CAP_DLL ExecutionThread(const uint64_t& functionStart, uint64_t* functionCallCount, executionFunctionStackList calls, ModBlockPass* pass);

	const uint64_t& GetFunctionStart();
	const uint64_t* GetFunctionCallCount();
	const executionFunctionStackList& GetCalls();
	const std::atomic<bool>& GetFinished();
	const std::atomic<bool>& GetKill();
	const std::atomic<bool>& GetBreaked();
	const std::atomic<bool>* GetResume();
	const std::atomic<bool>& GetStep();

	ModBlockPass* GetPass();

	void SetFinished(bool finished);
	void SetStep(bool step);
	void SetBreak(bool breaked);

	void End();
	void Break(std::atomic<bool>* resume);
	void Step();
private:
	std::atomic<bool> m_finished;
	std::atomic<bool> m_kill;
	std::atomic<bool> m_ended;

	std::atomic<bool> m_breaked;
	std::atomic<bool>* m_resume;
	std::atomic<bool> m_step;

	const uint64_t& m_functionStart;
	const uint64_t* m_functionCallCount;
	executionFunctionStackList m_calls;
	std::thread m_thread;
	ModBlockPass* m_pass;
};
