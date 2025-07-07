# 贡献指南

## 开发环境设置

1. 克隆仓库

2. 安装依赖

- 安装 g++ 17, git

- 安装 CMake 3.26 或更高版本

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

- `View` 层负责界面设计和输入处理

- `ViewModel` 层负责将输入转发到模型层,持有 `Model`

- `Model` 层负责保存数据和处理游戏逻辑

- `App` 层负责页面管理以及以上三层的组装

- `Common` 层提供全局设定

## 代码架构

- `include/`
    + `App/` - 应用层
        + `GameApp.h` - 游戏应用程序头文件
    + `Common/` - 公共层
        + `Config/` - 数值定义
            - `GameConfig.h` - 游戏配置头文件
            - `GlobalConfig.h` - 全局配置头文件
            - `Config.h` - 导入以上两个文件,便于其他模块导入
        + `ChoicesInfo.h` - 升级选项信息
        + `CommandBase.h` - 命令基类头文件
        + `FrameInfo.h` - 帧信息头文件,用于更新画面
        + `NotificationBase.h` - 通知基类头文件,用于事件通知
        + `PageState.h` - 定义页面状态
        + `PlatformType.h` - 定义平台类型
        + `SkillID.h` - 定义技能 ID
        + `Trigger.h` - 触发器头文件,用于事件触发
    + `Model/` - 模型层
        + `Entities/` - 实体层
            - `Animation.h` - 动画实体头文件
            - `Bullet.h` - 子弹实体头文件
            - `Effect.h` - 效果实体头文件,用于在选择时提升用户属性
            - `Enemy.h` - 敌人实体头文件
            - `Pickup.h` - 道具实体头文件
            - `Platform.h` - 平台实体头文件
            - `Player.h` - 玩家实体头文件
            - `Skill.h` - 技能实体头文件
        + `GameModel.h` - 游戏模型头文件,用于保存游戏状态和逻辑
        + `MainMenuModel.h` - 主菜单模型头文件,保留
        + `ScoreModel.h` - 得分页面模型头文件,用于保存得分信息
        + `Model.h` - Model 基类
    + `ViewModel/` - 视图模型层
        + `GameViewModel.h` - 游戏视图模型头文件,用于处理游戏逻辑和用户输入
        + `MainMenuViewModel.h` - 主菜单视图模型头文件,处理动画效果
        + `ScoreViewModel.h` - 得分页面视图模型头文件,用于处理得分逻辑
        + `ViewModel.h` - ViewModel 基类
    + `View/` - 视图层
        + `UI/`
            - `Animation.h` - 动画 UI 头文件
            - `Bullet.h` - 子弹 UI 头文件
            - `Choice.h` - 选项 UI 头文件
            - `Enemy.h` - 敌人 UI 头文件
            - `Pickup.h` - 道具 UI 头文件
            - `Platform.h` - 平台 UI 头文件
            - `Player.h` - 玩家 UI 头文件
            - `Skill.h` - 技能 UI 头文件
            - `SkillBar.h` - 技能栏 UI 头文件
        + `GameView.h` - 游戏视图头文件,用于显示游戏界面
        + `MainMenuView.h` - 主菜单视图头文件,用于显示主菜单界面
        + `ScoreView.h` - 得分页面视图头文件,用于显示得分信息
        + `Page.h` - View 基类

- `src/` - 与 `include/` 目录结构类似

## 资源文件结构

- `assets/`
    + `dll/` - openal32 动态链接库
    + `images/`
        - `backgrounds/` - 背景图片
        - `effect/` - 升级选项图片
        - `enemy/` - 敌人图片
        - `platform/` - 平台图片
        - `player/` - 玩家图片
        - `skill/` - 技能图片
        - `others/` - 其他图片,如受击特效
    + `gif/` - 游戏演示
    + `sounds/` - 音效
    + `music/` - 背景音乐
    + `fonts/` - 游戏字体文件