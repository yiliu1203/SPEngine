## 环境配置

1. vs2022
2. xmake 和 vscode xmake 插件
3. clang-format工具 (llvm 附带)
4. vcpackage (可能会用上)
5. clang+llvm(生成反射用) (clang+llvm-18.1.8-x86_64-pc-windows-msvc.tar)


## 常用命令行

1. xmake project -k compile_commands ： 生成 compile_commands.json （copy到.vscode下）
    
    - 生成CompileCommands, 避免vscode 中include 目录的问题报错，会根据compile_commands 找到 include 目录

2. xmake project -k vsxmake -m "debug,release"   : 生成 vs 工程
3. xmake test : 执行单元测试  (https://www.cnblogs.com/tboox/p/17816608.html)



## External

1. [catch2](https://github.com/catchorg/Catch2) a unit testing framework for C++
2. [inja](https://github.com/pantor/inja?tab=readme-ov-file)
3. [json]

## Tools
1. 生成反射文件 [參考1](https://feiqi3.cn/blog/124) ， [參考2](https://cloud.tencent.com/developer/article/2168328), [参考3](https://github.com/huangfeidian/meta) [参考4](https://shaharmike.com/cpp/libclang/)