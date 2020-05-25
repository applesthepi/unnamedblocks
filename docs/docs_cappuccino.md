{% include header.md %}
{% include header_docs.md %}

# Cappuccino Documentation

### About

**Cappuccino** is a library that preforms the second phase of the preprocessor stage, compiles, debugs, and runs ub programs. During early development **Cappuccino** did not exist. Since we started to really crack down on performance, we needed to create a dedicated runtime environment.

### PreProcessor - Stage 1

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
Test