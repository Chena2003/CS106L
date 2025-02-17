

# CS106L Assignment

+ [HomeWork 2025](./HomeWork 2025/)：CS106L 2025 Winter Assignment. 作业地址：https://github.com/cs106l/cs106l-assignments/tree/main。课程主页：https://web.stanford.edu/class/cs106l/
+ [HashMap](./HashMap/)：实现一个类似于`unordeder_map`的哈希表
+ [GapBuffer](./GapBuffer/)：实现GapBuffer



其中`HashMap`和`GapBuffer`均来自[仓库](https://github.com/wengwz/CS106L-Self-Learning/tree/main)。由于在`windows`下编译，修改了`CMakeLists.txt`，并且使用下面编译命令，指定`g++`生成`makefile`：

```cmake
cmake -G "MinGW Makefiles" -DCMAKE_C_COMPILER="C:\Program Files\MSYS2\ucrt64\bin\gcc.exe" -DCMAKE_CXX_COMPILER="C:\Program Files\MSYS2\ucrt64\bin\g++.exe" ..
```

