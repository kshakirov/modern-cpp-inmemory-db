# Issue #2 — Designing a Vectorized Lazy Query Operator via C++20 Coroutines


**State:** OPEN
**Author:** @kshakirov
**Created:** 2026-08-02T15:16:28Z
**Updated:** 2026-08-02T15:16:28Z
**URL:** https://github.com/kshakirov/modern-cpp-inmemory-db/issues/2

---

### Issue #2: Designing a Vectorized Lazy Query Operator via C++20 Coroutines

#### 1. Mathematical & Theoretical Foundation
* Implement a relational selection operator ($\sigma_{predicate}$) as a lazy evaluation stream.
* Ensure the operator is orthogonal to the underlying storage type.

#### 2. Hardware-Driven Constraints (Cache-Locality)
* Avoid the Volcano model (tuple-at-a-time) to prevent instruction cache misses.
* Avoid full realization to prevent out-of-memory errors on large datasets.
* Pass data chunks between operators inside the L1/L2 cache boundaries.

#### 3. C++ Modern Architecture Design
* Use **`std::generator`** (C++23) to build a lazy, suspendable pipeline.
* Make the coroutine yield a **`std::vector<size_t>`** containing a batch (chunk) of filtered indices.
* Enforce **Move Semantics (`std::move`)** to transfer index chunks out of the coroutine without copying memory.


---

## Comments

