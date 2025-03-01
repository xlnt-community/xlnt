## Worksheet

### Comparisons

Comparisons that use the `operator==` (e.g. `worksheet1 == worksheet2`) do pointer comparisons internally. While this might seem unusual compared to other C++ classes, it aligns with XLNT's [Memory model](/docs/basics/MemoryModel.md) and ensures the best performance, while being good enough for most use cases. If you instead want to perform a deep comparison of all underlying objects, use `compare()` instead.
