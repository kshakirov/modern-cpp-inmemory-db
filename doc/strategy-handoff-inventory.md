# Modern C++ In-Memory DB — Strategy Handoff Inventory

**Repository:** https://github.com/kshakirov/modern-cpp-inmemory-db

**Branch:** `dev`

**Inspected commit:** `77297ffcdae146083972d37469a9863d0d7c3533`

**Inventory date:** 2026-10-11

## Strategic frame

The project is an experimental C++ computation engine over finite multisets of tuples using bag semantics.

Current development order:

1. Preserve and extend the existing codebase rather than rewriting it.
2. Build a correct reference executor for relational algebra.
3. Add alternative algorithms, measurements and optimizations afterward.
4. Treat Datalog as the next stage: rules, recursion and fixed-point evaluation.
5. Keep SQL as a separate research direction.
6. Study C++ and physical execution down to generated machine code.

Core principles:

- SoA remains the current physical representation.
- Multiplicities of equal tuples must be preserved.
- Correctness precedes measurement, hypotheses and experiments.
- Every relational operator needs a mathematical contract and a test.
- The reference executor remains available as an oracle for optimized implementations.

## Inventory

| Operation | File | State | Test | Notes |
|---|---|---|---|---|
| Row insertion | `lib/lattice_storage/lattice_storage.hpp` | Working prototype | `test_templates` passes | Populates SoA columns through `insert_row`; multiplicities are preserved through `push_back` |
| Compile-time column lookup | `lib/lattice_storage/lattice_storage.hpp` | Experimental implementation | Compile-time `static_assert` passes | Converts a symbolic name into a tuple index; a missing name is represented as `size_t(-1)` |
| Value-position lookup | `lib/lattice_storage/lattice_storage.hpp` | Working primitive | `test_templates` passes | Returns only the first matching position; this is not relational selection |
| Single-value column lookup | `lib/lattice_storage/lattice_storage.hpp` | Working primitive | Exercised by `main` | `select_one` returns one value and does not preserve a complete tuple or result multiplicity |
| Selection `σ` | `lib/rel_operator/rel_operator.C` | Stub | Executes but has no semantic test | `Filter` only prints a message and does not filter data |
| Projection `π` | — | Not implemented | None | No projection operation over `Table` exists |
| Cartesian product `×` | `test/test_cartesian_product.C` | Narrow working prototype | The `1×5` case passes | Supports one column on each side and materializes five pairs; result values are only partially asserted |
| Table-schema composition | `test/test_cartesian_product_2.C` | Compiling prototype | Smoke test passes | Produces `Table<LCols..., RCols...>` and concatenates column tuples; it does not multiply rows, so column lengths may differ |
| Recursive composition of N tables | `test/test_cartesian_product_2.C` | Prototype | Two-table case compiles and runs | The recursive structure exists, but there is no three-table test or invariant check |
| Union `∪` | — | Not implemented | None | Schema compatibility and multiplicity-preserving concatenation are not defined |
| Predicate join | — | Not implemented | None | Functions currently named `Join` investigate Cartesian product or schema composition, not a predicate join |

## Verification performed

The following targets or programs compile and run successfully at the inspected commit:

- the main executable;
- `test_templates`;
- Cartesian product prototype v1;
- Cartesian/schema-composition prototype v2.

There is no unified automated test runner yet. Verification currently combines:

- compile-time `static_assert` checks;
- runtime `assert` checks;
- successful compilation and smoke execution.

## First available execution chain

The repository currently supports this limited end-to-end path:

```text
Table
  → insert_row
  → single-column Cartesian product prototype
  → materialized Table
  → direct column inspection / get_index
```

This is not yet a complete relational-algebra pipeline.

The most natural next reference operator is selection over a `Table`:

```text
Table
  → resolve predicate column
  → evaluate predicate per row
  → copy complete matching tuples
  → result Table with the same schema
```

Its contract must preserve tuple integrity, input order for the reference implementation, and the multiplicity of every matching tuple.

## Main distinction established by the prototypes

Compile-time schema composition and runtime relational execution are separate problems:

```text
Table<LCols...> + Table<RCols...>
              ↓ compile time
Table<LCols..., RCols...>

rows(R), rows(S)
              ↓ runtime
all tuple pairs from R × S
```

The second Cartesian prototype currently solves only the upper, schema-composition part.
