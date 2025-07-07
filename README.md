# To The Hell

![版本](https://img.shields.io/badge/版本-2.0.0-blue.svg)
![许可证](https://img.shields.io/badge/许可证-MIT-green.svg)
![构建状态](https://img.shields.io/badge/构建-进行中-yellow.svg)

## 👋🏻 游戏简介

<p align="center">
    <img src="assets/gif/demo.gif" width="500" alt="游戏演示">
</p>

这是一个 Rougelike 风格的游戏,采用经典的"是男人就下100层"的玩法,玩家需要在地牢中不断探索,击败敌人,尽可能取得更高的分数。

## 🚀 快速开始

### 📒 安装说明

```bash
git clone https://hithub.com/yelan187/to_the_hell
cd to_the_hell
makedir build && cd build
cmake ..
make
```

注:初次执行`cmake ..` 可能需要安装一些依赖,耗时较长

### 🎮 运行游戏

```bash
cd build/bin
./ToTheHell
```

### 🕹️ 操作说明

- 主页面/得分页面:

    - **WS**/**方向上下**: 移动和控制选项

    - **Enter**/**J**: 确认选择

- 游戏页面:

    - **AD**: 移动角色

    - **W**: 跳跃,初始最大跳跃次数为 2

    - **J**: 技能 1,射出箭矢攻击敌人

    - **U**: 技能 2,向前冲刺一段距离
        + 技能 2 在冷却时,击杀两个敌人会刷新技能冷却时间

    - **S**: 技能 3,穿越到当前所处平台的下方
        + 若当前不在平台上,则会提升下落速度(没有冷却时间)

    - **1/2/3**: 选择升级效果

    - 游戏共有 5 种平台
        + 普通平台:砖块
        + 脆弱平台:玻璃,会在被踩到之后一段时间内消失
        + 弹跳平台:绿色粘液块
        + 滚动平台:带有箭头,方向和速度随机
        + 尖刺平台:红色岩浆块,直接杀死玩家

    - 游戏会随机刷新敌人,敌人会自动攻击,击败敌人可以获得分数

    - 游戏会随机刷新道具,捡到可以增加分数
        + 圆形道具:必定刷新到平台上,捡到加 2 分
        + 星形道具:随机刷新,捡到加 5 分

    - 游戏随时间增大难度,随分数给出升级机会,升级可以增加玩家属性

## 🛠️ 开发者指南

请查阅 [CONTRIBUTING.md](CONTRIBUTING.md) 文件。

## 🏗️ 技术架构

游戏使用 C++ 语言开发,采用了 **MVVM** 的设计模式。主要使用了以下技术栈:

- C++17
- SFML 用于图形渲染和音频处理
- CMake 用于构建系统

## 📚 项目历史

本项目最初于2023年由 [Eclipsky](https://github.com/Yitian26) 和 [yelan](https://github.com/yelan187) 以Python版本开发并发布于[原始仓库](https://github.com/Yitian26/to_the_hell)。

现版本（C++重写）作为浙江大学计算机学院"课程综合实践II"的项目，由以下成员共同开发：
- [yelan](https://github.com/yelan187)
- [WuYan](https://github.com/wuyan1345)
- [XuebaStudy](https://github.com/XuebaStudy)

## 👏🏻 致谢

感谢以下(上文未提到的)项目支持：

- [Fusion Pixel Font](https://github.com/TakWolf/fusion-pixel-font) - 开源像素字体

- [くらげ工匠](http://www.kurage-kosho.info) - 提供了游戏所需音效

- [game-icons](https://game-icons.net) - 提供了图标

- [open game art](https://opengameart.org) - 提供了纹理

### ✨ 角色设计灵感

游戏主角的形象设计灵感来源于B站UP主 [Warma](https://space.bilibili.com/53456)。本项目是非商业性质的学习作品，仅作学习交流用途。如有侵权，请联系我们删除。

## 📄 许可证

本项目采用 [MIT 许可证](LICENSE) 开源。您可以自由使用、修改和分发代码，但请保留原作者的版权声明和许可证信息。