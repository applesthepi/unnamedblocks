{% include header.md %}
{% include header_docs.md %}

# Cappuccino Documentation

<ul>
	<li><a href="#preprocessor">PreProcessor</a></li>
	<li><a href="#configurations">Configurations</a></li>
	<li><a href="#compalation">Compalation</a></li>
	<li><a href="#modblockpass">ModBlockPass</a></li>
	<li><a href="#executionthread">ExecutionThread</a></li>
	<li><a href="#super_instructions">Super Instructions</a></li>
</ul>

## About

**Cappuccino** is a library that preforms the compiles, debugs, and runs ub programs. During early development **Cappuccino** did not exist. Since we started to really crack down on performance, we needed to create a dedicated runtime environment.

<h2 id="preprocessor">PreProcessor</h2>

The preprocessor stage takes place on the executable. Going though all blocks, it creates a **ModBlockData** for each one. The **ModBlockData** contains the following items:

```cpp
std::vector<void*> m_data;
std::vector<void*> m_preData;
std::vector<ModBlockDataType> m_types;
std::vector<ModBlockDataInterpretation> m_interpretations;
```

Every argument has one index inside this vector. All arguments that are in variable mode relay their's as an **std::string\***. Reals use **double\*** and booleans use **bool\***. This is the most basic form of information you can get from each block in the editor.

```cpp
std::vector<void*> m_data;
```

Predata is data that can be transfered from the initialization stages to the runtime stages. It is part of **ModBlockData** because this is how the **ModBlockPass** gets it's information. Predata does nothing during this stage.

```cpp
std::vector<void*> m_preData;
```

**ModBlockDataType** is an enum that declares whether or not it is a variable. This is to distinguish between a variable that always uses an **std::string\***, and a raw value that could use any pointer.

```cpp
std::vector<ModBlockDataType> m_types;
```
```cpp
enum class ModBlockDataType
{
	VAR, RAW
};

```
**ModBlockDataInterpretation** is an enum that defines the interpretation of an argument. This is only between preprocessor stages to state how to read and interpretate the data inside **ModBlockData**

```cpp
std::vector<ModBlockDataInterpretation> m_interpretations;
```
```cpp
enum class ModBlockDataInterpretation
{
	TEXT, STRING, REAL, BOOL
};
```

**TCC**, or **Tiny C Compiler** is not a well known compiler. Its purpose is to be a "middle man" from the executable and Cappuccino. The **PreProcessor** (executable side) works with **TCC** to do a few things. Firstly it links all mods with **Cappuccino** so it has access to the functions inside the mods. The **PreProcessor** defines these symbols though **TCC**:

```c
uint64_t functionMain;
uint64_t* functionCallCount;
uint64_t functionTotalCount;
void (***calls)(ModBlockPass*);
ModBlockData** functionData;
ModBlock*** modBlocks;
bool debugBuild;
uint8_t* superInstruction;
int64_t* superData;
void* superMutex;
```

One symbol you will reconize right away, **ModBlockData\*\* functionData**. This contains every blocks' data through the **ModBlockData**. A couple other easily understandable symbols are the **bool debugBuild** and **ModBlock\*\*\* modBlocks**. The **ModBlock\*\*\*** is understood as a 2d array of **ModBlock\***. See the (Espresso Documentation)[https://applesthepi.github.io/unnamedblocks/docs_espresso.html] for more information.

**uint64_t functionMain** contains what stack is marked as an entry (**vin_main**). **uint64_t functionTotalCount** denotes how many stacks there are and **uint64_t\* functionCallCount** is an array, each element defines the block count in a stack, each stack has an element.

**void (\*\*\*calls)(ModBlockPass\*)** looks very confusing, aspecialy if you dont know **C**. It is basically a pointer to a function that can be called. **void** is the return specifier, **ModBlockPass\*** is the parameter, **calls** is the name, and **\*\*\*** part is a multi-pointer. This is a 2d array of function pointers.

**uint8_t\* superInstruction** is an unsigned 8 byte integer that specifies the instruction that can be read and written to by the executable and **Cappuccino**. This is mutexed on both sides though a **std::mutex\***, descised as **void\* superMutex**. This is so **TCC** can compile it, but **Registration** (**Cappuccino** side) is fully aware of this being an **std::mutex\***. Similarly the **int64_t\* superData** carries the data next to the instruction.

<h2 id="configurations">Configurations</h2>

A very important feature of **Unnamed Blocks** are the debug and release build configurations. The debug configuration gives buffers and makes it really easy to find issues with a program with the cost of performance. It also alows the use of thread breaking and stepping. The debug configuration is only avilable with the editor attached.

The release configuration will take longer to compile, and is much less safe and prone to overflows and crashes. The release configuration does anything in its power to be as fast as possible during runtime. Its much faster than the debug configuration because of all the optomizations it puts in place and minimizes safty guards.

<h2 id="compalation">Compalation</h2>



<h2 id="modblockpass">ModBlockPass</h2>



<h2 id="executionthread">ExecutionThread</h2>



<h2 id="super_instructions">Super Instructions</h2>