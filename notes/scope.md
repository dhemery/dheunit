# Scope

## Problem

I want to declare variables in a `describe` block,
so that `before` blocks can set them
and `test` blocks can refer to them.

**Problem:**
- Variables declared in a `describe` block
  go out of scope when the `describe()` function returns,
  which happens before the `before` and test blocks run.
- Test blocks and `before` blocks run after `describe()` returns.


## Solutions

1. Put `describe` block variables on the heap.
   Then allocate them in `before` blocks 
   and delete them in `after` blocks.
2. Let `before` and `test` lambdas
   copy the variables using copy capture (`=`) by default.
   This will work only if the variables are pointer-like.
   So maybe all such variables must be pointers or `std::shared_ptr<T>`.
