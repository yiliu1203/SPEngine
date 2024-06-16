## 环境配置

1. vs2022
2. xmake 和 vscode xmake 插件
3. clang-format工具
4. vcpackage (可能会用上)


## 常用命令行

1. xmake project -k compile_commands ： 生成 compile_commands.json （copy到.vscode下）
    
    - 生成CompileCommands, 避免vscode 中include 目录的问题报错，会根据compile_commands 找到 include 目录

2. xmake project -k vsxmake -m "debug,release"   : 生成 vs 工程
3. xmake test : 执行单元测试