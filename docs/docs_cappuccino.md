{% include header.md %}
{% include header_docs.md %}

# Cappuccino Documentation

<ul>
	<li><a href="#pre_1">PreProcessor - Stage 1</a></li>
</ul>

## About

**Cappuccino** is a library that preforms the second phase of the preprocessor stage, compiles, debugs, and runs ub programs. During early development **Cappuccino** did not exist. Since we started to really crack down on performance, we needed to create a dedicated runtime environment.

<h2 id="pre_1">PreProcessor - Stage 1</h2>

The first preprocessor stage takes place on the executable. Going though all blocks, it creates a **ModBlockData** for each one. The **ModBlockData** contains the following items:
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