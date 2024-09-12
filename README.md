<h1>sammine-lang</h1>

![](https://github.com/badumbatish/sammine-lang/blob/main/img.png)
How I pictured sammine-lang in my head



The section is written with codes in triple quotes to utilize Clion run-in-markdown capabilities.

<h2>Dev</h2>
Run
```bash
cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

for configuration. We need llvm and FileCheck

Run
```bash
cmake --build build -j4 --target unit-tests e2e-tests
```
in the project root folder to cycle through the development process.


Run
```bash
python3 docs.py
```
to generate documentation for the project and have your default web browser open it.

The default markdown-based documentation and all related is named `docs`

The default generated documentation folder for `docs.py` is named `generated_docs`.

Run
```bash
rm -rf build
```
to erase the build folder (similar to make clean).
