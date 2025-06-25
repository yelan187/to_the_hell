# 贡献指南

## 开发环境设置

1. 克隆仓库

2. 安装依赖

- 安装 g++, git

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

- `src/`
    + `main.cpp` 
    + `game/` 
    + `ui/` 
    + `utils/` 

- `include/` 
    + `game/`
    + `ui/`
    + `utils/`

## 资源文件结构

- `assets/`
    + `images/`
    + `sounds/`