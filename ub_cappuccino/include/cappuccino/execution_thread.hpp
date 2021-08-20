#pragma once
#include "config.h"
#include "cap_config.h"

#include "cappuccino/mod_block/pass.hpp"

#include "cappuccino/utils.hpp"

typedef void(***executionFunctionStackList)(ModBlockPass*);
typedef void(*executionFunction)(ModBlockPass*);
typedef void(**executionFunctionStack)(ModBlockPass*);

class ExecutionThread
{
public:
	CAP_DLL ExecutionThread(u64 functionStart, u64* functionCallCount, executionFunctionStackList calls, ModBlockPass* pass);

	u64 GetFunctionStart();
	const u64* GetFunctionCallCount();
	const executionFunctionStackList& GetCalls();
	const std::atomic<bool>& GetFinished();
	const std::atomic<bool>& GetKill();
	const std::atomic<bool>& GetBreaked();
	const std::atomic<bool>* GetResume();
	const std::atomic<bool>& GetStep();

	CAP_DLL ModBlockPass* GetPass();

	void SetFinished(bool finished);
	void SetStep(bool step);
	void SetBreak(bool breaked);

	CAP_DLL void End();
	void Break(std::atomic<bool>* resume);
	void Step();
private:
	std::atomic<bool> m_finished;
	std::atomic<bool> m_kill;
	std::atomic<bool> m_ended;

	std::atomic<bool> m_breaked;
	std::atomic<bool>* m_resume;
	std::atomic<bool> m_step;

	const u64 m_functionStart;
	const u64* m_functionCallCount;
	executionFunctionStackList m_calls;
	std::thread m_thread;
	ModBlockPass* m_pass;
};
