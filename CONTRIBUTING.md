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

## 提交规范

- `feat`: 新功能
- `fix`: 修复 bug
- `docs`: 文档变更
- `style`: 代码格式（不影响功能的变更）
- `refactor`: 重构（既不是新增功能，也不是修复 bug）
- `perf`: 提升性能
- `chore`: 其他变更（例如构建过程或辅助工具的变更）
- `temp`: 临时提交（例如调试代码）

## 设计思路

采用 MVVM 设计模式.

- `View` 层负责界面设计和输入处理, 持有 `ViewModel` 

- `ViewModel` 层负责将输入转发到模型层,持有 `Model`

- `Model` 层负责保存数据和处理游戏逻辑,同时持有 `engine`,可以处理游戏状态

## 代码架构

- `src/`
    + `View/`
        + `MainMenuView.cpp` - 主菜单界面
        + `GameView.cpp` - 游戏界面
        + `UI/` - 界面组件
            - `Player.cpp` - 用户组件
            - `Platform.cpp` - 平台组件
    + `ViewModel/`
        + `MainMenuViewModel.cpp` - 主菜单视图模型
        + `GameViewModel.cpp` - 游戏视图模型
    + `Model/`
        + `MainMenuModel.cpp` - 主菜单模型
        + `GameModel.cpp` - 游戏模型
    + `Core/` - 核心逻辑
        + `Engine.cpp` - 游戏引擎,负责游戏循环和事件处理,处理页面切换
    <!-- + `Utils/` - 工具类 -->
    + `main.cpp` - 程序入口

- `include/` - 与 `src/` 目录结构类似

## 资源文件结构

- `assets/`
    + `images/`
    + `sounds/`
    + `fonts/`