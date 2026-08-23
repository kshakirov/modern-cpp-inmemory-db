# Project Principles

## Purpose

Modern C++ In-Memory DB is not intended to be another database implementation built by reproducing an existing DBMS architecture.

The project is an engineering and research laboratory for exploring how a strictly typed, compile-time relational algebra engine can be expressed in modern C++ while remaining close to the physical behavior of the machine.

The implementation is therefore both a working system and an experimental instrument.

---

## 1. Relational Algebra Before Database Machinery

The conceptual foundation of the system is relational algebra.

Database mechanisms should not be introduced merely because conventional database systems contain them.

Each abstraction must appear only when it becomes necessary for expressing or executing relational operations.

The implementation should preserve a visible correspondence between:

- mathematical relational operations;
- their representation in the C++ type system;
- their execution on physical hardware.

---

## 2. Compile-Time Structure

Schemas and relational contracts should be known to the compiler whenever possible.

Column names and column types are part of the type-level description of a relation.

Invalid operations should therefore preferably become compilation errors rather than runtime failures.

The current direction uses C++ compile-time facilities, including:

- templates;
- variadic templates;
- Non-Type Template Parameters (NTTP);
- compile-time string representations;
- heterogeneous products such as `std::tuple`.

The objective is not metaprogramming for its own sake.

Compile-time machinery is justified only when it removes runtime machinery or makes an invariant explicit and mechanically checkable.

---

## 3. Data and Algorithms Are Orthogonal

The project follows the generic-programming tradition associated with Alexander Stepanov.

Data structures and algorithms should not be artificially bound together through object hierarchies.

In particular, the project avoids using runtime object-oriented mechanisms as its basic architectural tool.

The default direction is:

- no virtual dispatch in the relational core;
- no unnecessary runtime polymorphism;
- no hidden object hierarchy;
- no type erasure unless a concrete necessity for it is demonstrated.

Algorithms should operate over structures satisfying explicit contracts.

---

## 4. Physical Representation Matters

An abstraction is not considered successful merely because its interface is elegant.

Its physical representation and execution cost matter.

The current storage direction is column-oriented:

```text
Relation
   |
   +-- Column<A> -> contiguous storage
   +-- Column<B> -> contiguous storage
   +-- Column<C> -> contiguous storage