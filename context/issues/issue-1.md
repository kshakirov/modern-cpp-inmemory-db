# Issue #1 — Designing the Compile-Time Columnar Storage Layout (SoA) via Metaprogramming


**State:** OPEN
**Author:** @kshakirov
**Created:** 2026-08-02T15:14:21Z
**Updated:** 2026-08-23T19:26:27Z
**URL:** https://github.com/kshakirov/modern-cpp-inmemory-db/issues/1

---

### Задача #1: Создать тип таблицы для хранения данных по колонкам

#### Что нужно сделать (Критерии приемки):
1. **Колонка** — это обычный `std::vector`. Данные одной колонки лежат в памяти подряд.
2. **Имя колонки** — строка. В коде пишем `"id"` или `"name"`. Для этого пишем обертку `FixedString`, чтобы компилятор разрешил передавать текст в угловые скобки шаблона `Column<"id", int>`.
3. **Таблица (`LocalStorage`)** — объединяет эти векторы через `std::tuple`. Нам нужно научить её автоматически создавать правильный набор векторов из списка колонок с помощью трех точек `...`.

#### На каком шаге остановились:
Пункты 1 и 2 готовы и проверены компилятором. Остановились на пункте 3: нужно научить таблицу находить нужный вектор по текстовому имени колонки во время компиляции.


---

## Comments


### @kshakirov — 2026-08-02T15:37:52Z

### Issue #1: Designing the Compile-Time Columnar Storage Layout (SoA) via Metaprogramming

> 💡 **ARCHITECTURAL NOTE / COMMENTARY:**
> This storage engine is NOT an external or run-time database manager (like SQLite or Postgres). 
> It is a highly specialized, compile-time Data Structure Type. 
> Its primary purpose is to embed data directly within the program's type system, enabling 
> zero-overhead Relational Algebra operators (such as selection, projection, and joins) 
> in an embedded, declarative DSL style similar to C# LINQ or Haskell HINQ.

#### 1. Mathematical & Theoretical Foundation (Harper)
* Define a relation as a product type (Product Type) where the schema is fully fixed and validated at compile time.
* Prevent any run-time type erasure or dynamic type dispatch.

#### 2. Hardware-Driven Constraints (Stroustrup)
* Implement underlying storage using the **Structure of Arrays (SoA)** memory layout.
* Ensure all elements of a single column are stored contiguously in memory to achieve optimal L1/L2 cache line utilization and enable automatic compiler loop vectorization (SIMD).

#### 3. C++ Modern Architecture Design
* Use **`std::tuple`** of **`std::vector<T>`** as the static, heterogeneous container for columns.
* Map human-readable string column names (e.g., `"age"`, `"id"`) directly to integer tuple indices at compile-time.
* Utilize **C++20 Non-Type Template Parameters (NTTP)** wrapped in a custom **`FixedString`** structural type wrapper to perform zero-cost compile-time string-to-index resolution.


---

### @kshakirov — 2026-08-02T16:49:44Z

 Compile-time строки (FixedString) и Structural Types — Выполнено и верифицировано компилятором GCC 15.

---

### @kshakirov — 2026-08-23T19:26:27Z


### **Status Update / Progress Report**

**Реализовано и проверено:**

1. **Каркас реляционной таблицы (`Table`):**
* Внедрена структура `Table<Cols...>`, хранящая разнородный набор колонок в виде `std::tuple<Cols...>`.


2. **Обобщённый метод вставки (`insert_row`):**
* Реализована метафункция вставки строк `template<typename... Args> void insert_row(Args&&... args)`, поддерживающая произвольное количество аргументов и авто-вывод типов.
* Использован `std::apply` и механизм C++17 Fold Expression (`((cols.insert(std::forward<Args>(args))), ...);`) для последовательной рассылки данных по векторам колонок.
* Реализована передача аргументов через универсальные ссылки (`Args&&...`) с сохранением категорий значений (Perfect Forwarding via `std::forward`).


3. **Практическая верификация (Runtime):**
* В `main.C` успешно проверена запись строки `"kirill", 1` в таблицу `Table<Column<"name", std::string>, Column<"id", int>>` и прямое извлечение данных из `vector` через `std::get<0>`.





**Планы на следующий шаг:**

* Проектирование метафункции `get_column<Name>()` для обращения к колонке по её строковому имени на этапе компиляции (`FixedString Name`) без использования числовых индексов `std::get<N>`.
* Детальный разбор категорий значений (`lvalue`/`rvalue`, `prvalue`, `xvalue`) и работы `std::forward`.



---
