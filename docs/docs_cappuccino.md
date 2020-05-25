{% include header.md %}

{% include header_docs.md %}

# Cappuccino Documentation

### About

Cappuccino is a library that preforms the second phase of preprocessing stage, compiles, debugs, and runs ub programs. During early development Cappuccino did not exist. Since we started to really crack down on performance, we needed to create a dedicated runtime environment.

### PreProcessor - Stage 1

The first stage of the PreProcessor takes place on the executable. Going though all blocks, it creates a `ModBlockData` for each one. The `ModBlockData` contains the following items:
<ol>
	<li>data</li>
	<li>preData</li>
	<li>types</li>
	<li>interpretations</li>
<ol>
