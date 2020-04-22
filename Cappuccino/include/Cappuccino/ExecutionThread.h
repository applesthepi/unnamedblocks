#pragma once
#include "ModBlockPass.h"

#include <atomic>
#include <cinttypes>

typedef void(***executionFunctionStackList)(ModBlockPass*);
typedef void(*executionFunction)(ModBlockPass*);
typedef void(**executionFunctionStack)(ModBlockPass*);

class ExecutionThread
{
public:
	ExecutionThread(uint64_t functionStart, uint64_t* functionCallCount, executionFunctionStackList calls, ModBlockPass* pass);

	const uint64_t& GetFunctionStart();
	const uint64_t* GetFunctionCallCount();
	const executionFunctionStackList GetCalls();
	const std::atomic<bool>& GetFinished();
	ModBlockPass* GetPass();

	void SetFinished(bool finished);

	void End();
private:
	std::atomic<bool> m_finished;
	uint64_t m_functionStart;
	const uint64_t* m_functionCallCount;
	executionFunctionStackList m_calls;
	std::thread m_thread;
	ModBlockPass* m_pass;
};