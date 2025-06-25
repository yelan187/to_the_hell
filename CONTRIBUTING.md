# 贡献指南

## 开发环境设置

1. 克隆仓库

2. 安装依赖

- 安装 g++ 17, git

- 安装 CMake 3.26 或更高版本

<!-- - 安装 [SFML 2.6.1](https://www.sfml-dev.org/download/sfml/2.6.1/) -->

3. 构建项目

```bash
mkdir build && cd build
cmake ..
make
```

## 代码规范

- 类名使用 PascalCase，例如 `PlayerCharacter`

- 函数名使用 camelCase，例如 `calculateScore`

- 变量名使用 snake_case，例如 `player_health`

- 常量使用全大写字母和下划线分隔，例如 `MAX_LEVEL`

- 每行代码尽量不超过 100 个字符

- 函数写清楚参数定义和返回值

- 注释少用中文

## 代码架构

采用 MVVM 设计模式,类似前后端分离的设计思想

- `src/`
    + `View/` - 界面设计
    + `Binding/` - 实现视图和模型的数据绑定
    + `ViewModel/` - 实现游戏数据到界面数据的转换
    + `Model/` - 游戏模型
    + `Core/` - 核心逻辑
    + `Utils/` - 工具类
    + `main.cpp` - 程序入口

- `include/` 
    与 `src/` 目录结构类似

## 资源文件结构

- `assets/`
    + `images/`
    + `sounds/`
    + `fonts/`