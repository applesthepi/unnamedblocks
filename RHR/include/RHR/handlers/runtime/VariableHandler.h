#pragma once

#include <string>
#include <vector>
#include <shared_mutex>
#include <mutex>

class StackLayer
{
public:
	StackLayer(uint64_t idx);
	~StackLayer();

	uint64_t Idx;
	uint64_t* StackIdx;
	std::vector<uint64_t> HeapIdx;

	double* StackReal;
	std::string* StackString;
	uint8_t* StackBool;

	std::vector<double> HeapReal;
	std::vector<std::string> HeapString;
	std::vector<uint8_t> HeapBool;
};

class VariableHandler
{
public:
	VariableHandler();
	~VariableHandler();
	void Reset();

	bool StackReal(uint64_t layerIdx, uint64_t variableIdx);
	bool StackString(uint64_t layerIdx, uint64_t variableIdx);
	bool StackBool(uint64_t layerIdx, uint64_t variableIdx);

	void HeapVariable(uint64_t layerIdx, uint64_t variableIdx);
	bool FreeVariable(uint64_t layerIdx, uint64_t variableIdx);

	const double* GetReal(uint64_t layerIdx, uint64_t variableIdx);
	const std::string* GetString(uint64_t layerIdx, uint64_t variableIdx);
	const uint8_t* GetBool(uint64_t layerIdx, uint64_t variableIdx);

	bool SetReal(const uint64_t& layerIdx, const uint64_t& variableIdx, const double& value);
	bool SetString(const uint64_t& layerIdx, const uint64_t& variableIdx, const std::string& value);
	bool SetBool(const uint64_t& layerIdx, const uint64_t& variableIdx, const uint8_t& value);

	uint64_t PushStack();
	void PopStack(const uint64_t& idx);

	VariableHandler& operator=(const VariableHandler& other);
private:
	uint64_t FindLayerIdx(const uint64_t& idx);

	std::shared_timed_mutex* m_mutex;
	std::vector<StackLayer> m_layers;
};
