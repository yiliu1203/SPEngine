# EngineFrom0


## 参考

- https://www.youtube.com/playlist?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT

## 环境搭建

0. vscode 开发环境， 需要安装编译器 vs2022 或者minGW，作为编译器，或则开发环境
1. xmake 的vscode 插件
2. vcpackage ：rttr 官方 的c++20 编译由问题， 用了vcpkg, 所以需要安装 vcpackage, [vcpackage](https://learn.microsoft.com/zh-cn/vcpkg/get_started/get-started?pivots=shell-cmd)

## 设置 option 
xmake f --use_gl=n --use_glfw=n --use_dx11=n

## 命名规范

类名：      AbcDef
成员变量：  m_abc_def;
成员方法：  abc_def

静态变量：  s_abc_def
全局变量：  g_abc_def
局部变量    abc_def



## xmake 使用

### [参考1](https://zhuanlan.zhihu.com/p/548735703) 

1. 生成compile_commands.json 
   
   ```
   xmake project -k compile_commands
   
   ```
2. 生成vs工程
   
   ``` xmake project -k vsxmake -m "debug,release" ```

3. ImGui 导入

    https://www.cnblogs.com/water-wells/p/16728845.html

    https://blog.csdn.net/think_A_lot/article/details/88290310


### 其他命令

    [参考](https://xmake.io/mirror/zh-cn/plugin/builtin_plugins.html)


- 生成vs解决方案
  
    ``` xmake project -k vsxmake -m "debug,release"```

- 生成CMakelist.txt 
  
    ```$ xmake project -k cmakelists ``

- 生成makefile 
  
    ```$ xmake project -k makefile``

- 生成CompileCommands, 避免vscode 中include 目录的问题报错，会根据compile_commands 找到 include 目录

``` xmake project -k compile_commands ```


- 默认安装的静态库， 如果要启用动态库， 可以配置如下：

``` add_requires("zlib", {configs = {shared = true}}) ```
当然，前提是这个包的定义里面，有对 package:config("shared") 判断处理，官方 xmake-repo 仓库里面，通常都是严格区分支持的。
    [参考](https://xmake.io/mirror/zh-cn/manual/global_interfaces.html)



### xmake 获取内置变量

https://xmake.io/mirror/zh-cn/manual/builtin_variables.html

https://zhuanlan.zhihu.com/p/611388172



## 依赖库

### glew

    使用静态链接, 要 加宏 GLEW_STATIC

### glfw

    使用静态链接

### imgui

    git submodule add git@github.com:yiliu1203/imgui.git Source/External/imgui


### submodule

- 删除submodule 方法1 https://www.w3docs.com/snippets/git/how-to-remove-a-git-submodule.html
- 删除submodule 方法2

```
To remove a submodule you need to:

Delete the relevant section from the .gitmodules file.
Stage the .gitmodules changes git add .gitmodules
Delete the relevant section from .git/config.
Run git rm --cached path_to_submodule (no trailing slash).
Run rm -rf .git/modules/path_to_submodule (no trailing slash).
Commit git commit -m "Removed submodule "
Delete the now untracked submodule files rm -rf path_to_submodule

```



<!-- 生成 compile_commands 里面没有头文件，没有没有对应的cpp文件， 则不会出现在compile_comands.json 里面， 无法include 外部库。。 -->


## Math

copy from flimant.

1. attribute((pure))
   
   用pure属性修饰的函数用来说明该函数除了返回值之外没有其他任何效果，并且该函数所返回的值仅仅依赖于函数的形参以及/或全局对象。 用pure属性所修饰的函数可以用来辅助编译器做消除公共子表达式以及帮助做循环优化，使用这种函数就好比使用算术操作符一般。


2. __declspec(empty_bases) 
   
   优化继承空对象时的内存布局 [参考](https://devblogs.microsoft.com/cppblog/optimizing-the-layout-of-empty-base-classes-in-vs2015-update-2-3/)



