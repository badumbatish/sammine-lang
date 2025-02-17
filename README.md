<h1>sammine-lang</h1>

![](https://github.com/badumbatish/sammine-lang/blob/main/img.png)
How I pictured sammine-lang in my head


<h2>Dev</h2>
Run
```bash
cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

for configuration. We need llvm and FileCheck

Run
```bash
cmake --build build -j --target unit-tests e2e-tests
```
in the project root folder to cycle through the development process.




Run
```bash
rm -rf build
```
to erase the build folder (similar to make clean).
