#pragma once
#include "Cappuccino/PreProcessorData.h"

#include <vector>

class ModBlockAction
{
public:
	virtual void Execute(PreProcessorData& /*data*/) {};
	void* Data;
};

// ===================================================================================================
// global containers
// ===================================================================================================
/*
class ModBlockActionCreateGlobalContainer : public ModBlockAction
{
public:
	class DataContainer
	{
	public:
		std::string Arg0;
	};

	ModBlockActionCreateGlobalContainer(const std::string& container)
	{
		Data = (void*)new DataContainer();
		((DataContainer*)Data)->Arg0 = container;
	}

	~ModBlockActionCreateGlobalContainer()
	{
		delete (DataContainer*)Data;
	}
	
	void Execute(PreProcessorData& data) override
	{
		data.AddGlobalContainer(((DataContainer*)Data)->Arg0);
	}
};

class ModBlockActionAddGlobalContainer : public ModBlockAction
{
public:
	class DataContainer
	{
	public:
		std::string Arg0;
		std::string Arg1;
		PreprocessorCell Arg2;
	};

	ModBlockActionAddGlobalContainer(const std::string& container, const std::string& name, const PreprocessorCell& cell)
	{
		Data = (void*)new DataContainer();
		((DataContainer*)Data)->Arg0 = container;
		((DataContainer*)Data)->Arg1 = name;
		((DataContainer*)Data)->Arg2 = cell;
	}

	~ModBlockActionAddGlobalContainer()
	{
		delete (DataContainer*)Data;
	}

	void Execute(PreProcessorData& data) override
	{
		data.AddGlobalValue(((DataContainer*)Data)->Arg0, ((DataContainer*)Data)->Arg1, ((DataContainer*)Data)->Arg2);
	}
};

class ModBlockActionGetGlobalContainer : public ModBlockAction
{
public:
	class DataContainer
	{
	public:
		std::string Arg0;
		std::string Arg1;
		void(*Arg2)(const PreprocessorCell& cell);
	};
	
	ModBlockActionGetGlobalContainer(const std::string& container, const std::string& name, void(*callback)(const PreprocessorCell&))
	{
		Data = (void*)new DataContainer();
		((DataContainer*)Data)->Arg0 = container;
		((DataContainer*)Data)->Arg1 = name;
		((DataContainer*)Data)->Arg2 = callback;
	}

	~ModBlockActionGetGlobalContainer()
	{
		delete (DataContainer*)Data;
	}

	void Execute(PreProcessorData& data) override
	{
		if (((DataContainer*)Data)->Arg2 == nullptr)
		{
			Logger::Error("failed to get global container; callback function is nullptr");
			return;
		}

		((DataContainer*)Data)->Arg2(data.GetGlobalValue(((DataContainer*)Data)->Arg0, ((DataContainer*)Data)->Arg1));
	}
};

// ===================================================================================================
// local containers
// ===================================================================================================

class ModBlockActionCreateLocalContainer : public ModBlockAction
{
public:
	class DataContainer
	{
	public:
		uint64_t Arg0;
		std::string Arg1;
	};

	ModBlockActionCreateLocalContainer(const uint64_t& idx, const std::string& container)
	{
		Data = (void*)new DataContainer();
		((DataContainer*)Data)->Arg0 = idx;
		((DataContainer*)Data)->Arg1 = container;
	}

	~ModBlockActionCreateLocalContainer()
	{
		delete (DataContainer*)Data;
	}

	void Execute(PreProcessorData& data) override
	{
		data.AddLocalContainer(((DataContainer*)Data)->Arg0, ((DataContainer*)Data)->Arg1);
	}
};

class ModBlockActionAddLocalContainer : public ModBlockAction
{
public:
	class DataContainer
	{
	public:
		uint64_t Arg0;
		std::string Arg1;
		std::string Arg2;
		PreprocessorCell Arg3;
	};

	ModBlockActionAddLocalContainer(const uint64_t& idx, const std::string& container, const std::string& name, const PreprocessorCell& cell)
	{
		Data = (void*)new DataContainer();
		((DataContainer*)Data)->Arg0 = idx;
		((DataContainer*)Data)->Arg1 = container;
		((DataContainer*)Data)->Arg2 = name;
		((DataContainer*)Data)->Arg3 = cell;
	}

	~ModBlockActionAddLocalContainer()
	{
		delete (DataContainer*)Data;
	}

	void Execute(PreProcessorData& data) override
	{
		data.AddLocalValue(((DataContainer*)Data)->Arg0, ((DataContainer*)Data)->Arg1, ((DataContainer*)Data)->Arg2, ((DataContainer*)Data)->Arg3);
	}
};

class ModBlockActionGetLocalContainer : public ModBlockAction
{
public:
	class DataContainer
	{
	public:
		uint64_t Arg0;
		std::string Arg1;
		std::string Arg2;
		void(*Arg3)(const PreprocessorCell& cell);
	};

	ModBlockActionGetLocalContainer(const uint64_t& idx, const std::string& container, const std::string& name, void(*callback)(const PreprocessorCell&))
	{
		Data = (void*)new DataContainer();
		((DataContainer*)Data)->Arg0 = idx;
		((DataContainer*)Data)->Arg1 = container;
		((DataContainer*)Data)->Arg2 = name;
		((DataContainer*)Data)->Arg3 = callback;
	}

	~ModBlockActionGetLocalContainer()
	{
		delete (DataContainer*)Data;
	}

	void Execute(PreProcessorData& data) override
	{
		if (((DataContainer*)Data)->Arg3 == nullptr)
		{
			Logger::Error("failed to get global container; callback function is nullptr");
			return;
		}

		((DataContainer*)Data)->Arg3(data.GetLocalValue(((DataContainer*)Data)->Arg0, ((DataContainer*)Data)->Arg1, ((DataContainer*)Data)->Arg2));
	}
};
*/
// ===================================================================================================
// logging
// ===================================================================================================

class ModBlockActionLogInfo : public ModBlockAction
{
public:
	class DataContainer
	{
	public:
		std::string Arg0;
	};

	ModBlockActionLogInfo(const std::string& value)
	{
		Data = (void*)new DataContainer();
		((DataContainer*)Data)->Arg0 = value;
	}

	~ModBlockActionLogInfo()
	{
		delete (DataContainer*)Data;
	}

	void Execute(PreProcessorData&) override
	{
		Logger::Info(((DataContainer*)Data)->Arg0);
	}
};

class ModBlockActionLogWarning : public ModBlockAction
{
public:
	class DataContainer
	{
	public:
		std::string Arg0;
	};

	ModBlockActionLogWarning(const std::string& value)
	{
		Data = (void*)new DataContainer();
		((DataContainer*)Data)->Arg0 = value;
	}

	~ModBlockActionLogWarning()
	{
		delete (DataContainer*)Data;
	}

	void Execute(PreProcessorData&) override
	{
		Logger::Warn(((DataContainer*)Data)->Arg0);
	}
};

class ModBlockActionLogError : public ModBlockAction
{
public:
	class DataContainer
	{
	public:
		std::string Arg0;
	};

	ModBlockActionLogError(const std::string& value)
	{
		Data = (void*)new DataContainer();
		((DataContainer*)Data)->Arg0 = value;
	}

	~ModBlockActionLogError()
	{
		delete (DataContainer*)Data;
	}

	void Execute(PreProcessorData&) override
	{
		Logger::Error(((DataContainer*)Data)->Arg0);
	}
};

class ModBlockActions
{
public:
	ModBlockActions();

	void AddAction(const ModBlockAction& action);
	const std::vector<ModBlockAction>& PullActions() const;
private:
	std::vector<ModBlockAction> m_actions;
};
