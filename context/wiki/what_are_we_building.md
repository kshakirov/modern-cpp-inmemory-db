
# What We Are Building

## Short Definition

**Modern C++ In-Memory DB is an embedded, strongly typed, in-memory relational algebra engine whose relation schemas and query structure are known at compile time, while the actual data exists and changes at runtime.**

The project is not an attempt to reproduce PostgreSQL, SQLite, or another conventional DBMS in miniature.

It investigates a different question:

> What does a relational engine look like when as much structural knowledge as possible is moved from runtime into the C++ type system, while the actual data remains ordinary runtime data stored close to the hardware?

---

## 1. The Fundamental Separation

The most important distinction in the project is:

```text
COMPILE TIME                         RUNTIME

relation schema                     relation values
column names                        rows / column contents
column types                        std::vector<T> data
valid relational operations         insertion of values
query structure                     execution of queries
operator compatibility              traversal of memory
```

We do **not** put database values into the C++ type system.

We put the **structure and contracts of relations and operations** into the type system.

For example:

```cpp
Table<
    Column<"name", std::string>,
    Column<"id", int>
>
```

describes the schema at compile time.

But:

```text
"name" column:
    "Kirill"
    "Alice"
    "Bob"

"id" column:
    1
    2
    3
```

is ordinary runtime data.

This distinction is fundamental.

---

## 2. What the Engine Should Do

The engine should provide relational operations over typed in-memory relations.

The conceptual language is relational algebra.

The core operations will eventually include things such as:

```text
selection       σ
projection      π
join            ⋈
```

and other operations only when they become necessary.

The C++ representation should preserve a recognizable correspondence between the mathematical operation and the executable program.

Conceptually:

```text
Relational Algebra
        |
        v
Typed C++ Expression
        |
        v
Compile-Time Specialization
        |
        v
Concrete Machine Code
        |
        v
Runtime Data
```

---

## 3. Embedded Rather Than External

The engine is intended to be embedded directly into a C++ program.

It is not primarily designed around:

```text
application
    |
    v
SQL string
    |
    v
database server
    |
    v
parser
    |
    v
runtime query planner
    |
    v
generic executor
```

Instead, the experiment moves toward:

```text
C++ program
    |
    v
typed relational expression
    |
    v
C++ compiler
    |
    v
specialized executable code
```

The C++ compiler therefore becomes an important part of the relational engine.

---

## 4. Schema Is a Type-Level Object

A relation schema should be known to the compiler whenever possible.

For example:

```cpp
Table<
    Column<"name", std::string>,
    Column<"age", int>
>
```

contains compile-time knowledge that:

```text
name -> std::string
age  -> int
```

Consequently an expression referring to:

```cpp
get_column<"age">()
```

should be resolvable by the compiler.

There should be no need for a runtime structure such as:

```text
map<string, column>
```

merely to discover which column `"age"` denotes.

Ideally:

```text
"age"
   |
   | compile time
   v
tuple index
   |
   | compilation
   v
direct access
```

The symbolic name exists for the programmer and the type system.

The runtime program should pay no lookup cost for it.

---

## 5. Data Is Runtime and Physical

Although schemas are compile-time structures, the database contents are runtime objects.

The current physical direction is columnar storage:

```text
Relation
   |
   +---- Column<"name", string>
   |        |
   |        +---- vector<string>
   |
   +---- Column<"age", int>
            |
            +---- vector<int>
```

This is a Structure of Arrays (SoA) representation.

It is chosen not merely as a convenient abstraction but because physical execution matters.

We want to study:

* contiguous memory access;
* cache locality;
* cache-line utilization;
* vectorization;
* SIMD opportunities;
* allocation behavior;
* traversal cost.

The path from mathematics to silicon is therefore part of the experiment:

```text
relational operation
        ↓
C++ abstraction
        ↓
memory representation
        ↓
generated instructions
        ↓
hardware behavior
```

---

## 6. Queries Should Become Specialized Programs

A central hypothesis of the project is that a relational query does not necessarily need to remain a generic runtime object.

If the schema and query structure are known during compilation, then a query expression can potentially become a specialized C++ program.

Conceptually:

```text
select age > 40
project name
```

should not necessarily require a universal runtime query interpreter.

Instead:

```text
typed query expression
        ↓
template/type machinery
        ↓
compiler optimization
        ↓
specialized loop/pipeline
```

The experiment is therefore concerned with removing machinery that exists only because conventional systems must discover information at runtime that our program may already know at compile time.

---

## 7. No Universal Runtime Machinery Without Necessity

The project deliberately avoids introducing conventional database machinery merely because conventional databases contain it.

We do not automatically need:

* runtime schemas;
* runtime column-name dictionaries;
* virtual operator hierarchies;
* type erasure;
* generic runtime query nodes;
* a universal execution engine;
* SQL parsing;
* a query planner.

Some of these mechanisms may eventually become necessary.

If so, they should appear because an experiment demonstrates their necessity.

They should not be introduced in advance.

The development rule is:

```text
need
 ↓
experiment
 ↓
constraint
 ↓
abstraction
```

rather than:

```text
DBMS architecture
 ↓
copy its abstractions
 ↓
implement them
```

---

## 8. Relational Algebra Is the Semantic Core

The project should distinguish between:

```text
WHAT is being computed
```

and:

```text
HOW it is executed
```

Relational algebra describes the first.

Storage layout, batching, coroutines, SIMD and cache behavior belong primarily to the second.

For example, selection:

```text
σ_P(R)
```

has a mathematical meaning independent of whether its implementation uses:

* scalar iteration;
* SIMD;
* index vectors;
* masks;
* batches;
* coroutines.

This separation allows us to change physical execution without changing relational semantics.

---

## 9. Zero-Cost Is a Question, Not a Slogan

The project uses the zero-overhead principle as an experimental criterion.

For every abstraction we should ask:

> What remains from this abstraction after compilation?

For example:

```cpp
table.get_column<"age">()
```

is valuable if the symbolic machinery disappears during compilation and the generated program performs essentially the same direct access we would have written manually.

The project should verify such claims through:

* generated assembly;
* benchmarks;
* allocation observation;
* cache behavior;
* compiler diagnostics and optimization reports where useful.

An abstraction is not zero-cost merely because its source code looks elegant.

---

## 10. Current Construction

The current experiment has reached the following representation:

```cpp
template<FixedString Name, typename T>
struct Column {
    std::vector<T> column;
};
```

and:

```cpp
template<typename... Cols>
struct Table {
    std::tuple<Cols...> columns;
};
```

Thus:

```cpp
Table<
    Column<"name", std::string>,
    Column<"id", int>
>
```

already represents the beginning of the intended separation:

```text
compile-time schema
        +
runtime columnar data
```

The immediate next problem is to replace positional access such as:

```cpp
std::get<0>(table.columns)
```

with compile-time symbolic access:

```cpp
table.get_column<"name">()
```

without introducing runtime name lookup.

This is the current experimental frontier.

---

## 11. Long-Term Target

The long-term target is therefore not simply:

> an in-memory database written in modern C++.

A more precise description is:

> **An embedded, strongly typed, columnar in-memory relational algebra engine in which schemas and query structure participate in compile-time reasoning, relational expressions compile into specialized executable code, and runtime machinery is introduced only where it is demonstrated to be necessary.**

Or, in the shortest form:

```text
compile-time relational structure
            +
runtime in-memory data
            +
specialized execution
```

That is what we are building.
