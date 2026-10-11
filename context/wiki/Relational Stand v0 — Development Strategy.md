# Relational Stand v0 — Development Strategy

## Зачем меняем тактику

Фундамент движка уже начал складываться:

```text
Column<Name, T>
      ↓
Table<Cols...>
      ↓
tuple<Column...>
      ↓
vector<T> per column
```

Мы подробно исследовали отдельные механизмы C++ — templates, parameter packs, `std::apply`, forwarding, compile-time поиск колонки.

Это необходимо, но появляется риск слишком долго совершенствовать инфраструктуру, не имея перед глазами работающего реляционного движка целиком.

Поэтому следующий этап — **сначала собрать минимальный стенд всей реляционной алгебры, затем возвращаться к каждой операции и исследовать её глубоко.**

---

## 1. Цель Stand v0

Получить минимальную end-to-end машину:

```text
Relational expression
        ↓
Compile-time schema
        ↓
Typed operation
        ↓
Physical SoA storage
        ↓
Runtime execution
        ↓
Observable result
```

На этом этапе важна не идеальная реализация, а наличие полного пути от реляционной операции до физических `vector<T>`.

---

## 2. Что уже есть

```text
Schema
  Column<Name, T>                ✓
  Table<Cols...>                 ✓

Physical storage
  tuple<Column...>               ✓
  Column → vector<T>             ✓

Mutation
  insert_row(...)                ✓ prototype

Compile-time lookup
  find_column_index<Name,...>    ✓ experiment

Symbolic → physical access
  Name → index → std::get<I>     ← next
```

Таким образом, основа хранения уже существует.

Следующая граница — переход от инфраструктуры к **реляционной алгебре**.

---

## 3. Первый вертикальный срез: Projection π

Сначала завершаем цепочку:

```text
"name"
   ↓
find_column_index<"name">
   ↓
compile-time I
   ↓
std::get<I>(columns)
   ↓
physical Column
   ↓
vector<T>
```

После этого строим минимальную проекцию:

```cpp
project<"name">(table);
```

Идеальная концептуальная цепочка:

```text
π<"name">
    ↓
compile-time symbolic name
    ↓
compile-time column resolution
    ↓
concrete column type
    ↓
physical vector<T>
```

Это будет первая настоящая операция реляционной алгебры, проходящая через весь движок.

---

## 4. Затем строим весь Stand v0

После projection не углубляемся сразу в её оптимизацию.

Добавляем минимальные реализации остальных операций.

### Projection π

```cpp
project<"name">(table);
```

Выбор подмножества колонок.

### Selection σ

Например:

```cpp
select<"age">(table, predicate);
```

Фильтрация строк по условию над типизированной колонкой.

### Product ×

```cpp
product(table1, table2);
```

Минимальная реализация декартова произведения двух отношений.

### Union ∪

```cpp
union_(table1, table2);
```

Объединение отношений с compile-time проверкой совместимости схем.

---

## 5. На Stand v0 не оптимизируем преждевременно

На первом проходе сознательно откладываем:

- SIMD;
- sophisticated views;
- coroutine pipelines;
- сложные masks;
- query planner;
- runtime operator hierarchy;
- универсальные query nodes;
- тонкую оптимизацию allocations;
- окончательный public API.

Главный вопрос v0:

> **Работает ли наша модель реляционной алгебры целиком поверх compile-time schema и физического SoA storage?**

---

## 6. Второй проход — исследовательский

Когда четыре базовые операции проходят end-to-end, возвращаемся к каждой отдельно.

Для каждой операции исследуем:

```text
Semantics
    ↓
Compile-time transformation
    ↓
Physical representation
    ↓
Runtime algorithm
    ↓
Memory behaviour
    ↓
Generated machine code
```

И задаём конкретные вопросы.

### Projection

- materialization или view?
- можно ли полностью избежать копирования?
- что остаётся после compilation?

### Selection

- index vector или mask?
- materialized relation или lazy view?
- branch behaviour?
- SIMD?

### Product / Join

- физический алгоритм;
- layout;
- временные структуры;
- locality;
- специализация по типам.

### Union

- copy vs move;
- batch insertion;
- compatibility schemas;
- allocations.

---

## 7. Zero-overhead как экспериментальный критерий

`Zero-cost` не принимается как лозунг.

Для каждой абстракции проверяем:

> **Что реально осталось после компиляции?**

Цепочка исследования:

```text
C++ abstraction
      ↓
instantiation
      ↓
optimization
      ↓
assembly
      ↓
memory behaviour
      ↓
measurement
```

Если compile-time конструкция исчезла и осталась только необходимая физическая работа — абстракция выполняет свою задачу.

Если появилась дополнительная runtime machinery — выясняем почему.

---

## 8. Правило дальнейшей разработки

Каждый новый механизм C++ должен отвечать на вопрос:

> **Как он приближает нас к физическому исполнению реляционной операции над конкретно типизированной таблицей?**

Если ответа нет, механизм пока не нужен.

Мы не строим систему шаблонов ради шаблонов.

Мы строим:

```text
Relational algebra
        ↓
Schema as types
        ↓
Concrete Table
        ↓
Physical SoA
        ↓
Specialized computation
        ↓
Machine
```

---

## Ближайший маршрут

```text
1. Name → compile-time index
              ↓
2. index → std::get<I>
              ↓
3. Projection π
              ↓
4. Selection σ
              ↓
5. Product ×
              ↓
6. Union ∪
              ↓
7. Stand v0 complete
              ↓
8. Deep optimization / verification pass
```

Главный принцип этапа:

> **Сначала увидеть весь работающий реляционный двигатель. Затем разбирать и улучшать каждую его деталь.**