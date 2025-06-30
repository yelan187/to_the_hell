# GameConfig 重构总结

## 重构内容

将 `Utils::GameConfig` 类从 private 成员变量 + getter/setter 函数的方式改为 public 成员变量直接访问的方式，简化使用。

## 主要改动

### 1. GameConfig.h 头文件
- 将所有配置参数改为 public 成员变量
- 使用大写命名规范：`PLATFORM_SIZE`, `PLAYER_SIZE`, `ENEMY_SIZE` 等
- 移除了所有 getter/setter 函数

### 2. GameConfig.cpp 实现文件
- 更新 `initializeDefaults()` 方法使用新的成员变量名
- 更新 `loadFromFile()` 方法的配置解析
- 更新 `saveToFile()` 方法的配置保存
- 更新 `adjustForDifficulty()` 方法

### 3. 所有使用配置的文件更新
- `SkillManager.cpp`: 从 `config.getPlayerSize()` 改为 `config.PLAYER_SIZE`
- `GeneratorManager.cpp`: 所有配置访问改为直接成员变量访问
- `EntityManager.cpp`: 技能冷却时间配置更新
- `GameViewModel.h/cpp`: 所有尺寸获取函数更新

## 新的使用方式

```cpp
// 旧方式
auto& config = Utils::GameConfig::getInstance();
sf::Vector2f size = config.getPlayerSize();
config.setPlayerSize(new_size);

// 新方式
auto& config = Utils::GameConfig::getInstance();
sf::Vector2f size = config.PLAYER_SIZE;
config.PLAYER_SIZE = new_size;
```

## 配置参数列表

### 实体尺寸
- `PLATFORM_SIZE`: 平台尺寸
- `PLAYER_SIZE`: 玩家尺寸  
- `ENEMY_SIZE`: 敌人尺寸
- `BULLET_SIZE`: 子弹尺寸
- `PICKUP_SIZE`: 豆子尺寸
- `ARROW_SIZE`: 箭矢尺寸

### 游戏速度
- `DEFAULT_SCROLL_SPEED`: 默认滚动速度
- `ARROW_SPEED`: 箭矢速度
- `SPRINT_DISTANCE`: 冲刺距离

### 生成间隔
- `PLATFORM_GENERATE_BASE_INTERVAL`: 平台生成基础间隔
- `ENEMY_GENERATE_BASE_INTERVAL`: 敌人生成基础间隔
- `PICKUP_GENERATE_BASE_INTERVAL`: 豆子生成基础间隔

### 技能冷却
- `ARROW_SKILL_COOLDOWN`: 箭矢技能冷却时间
- `SPRINT_SKILL_COOLDOWN`: 冲刺技能冷却时间

## 优势

1. **简化使用**: 直接访问成员变量，无需函数调用
2. **易于修改**: 直接赋值即可修改配置
3. **减少代码量**: 移除了大量的 getter/setter 函数
4. **性能提升**: 避免了函数调用开销

## 使用示例

```cpp
// 修改游戏难度
auto& config = Utils::GameConfig::getInstance();
config.ENEMY_GENERATE_BASE_INTERVAL = 2.0f;  // 更频繁生成敌人
config.DEFAULT_SCROLL_SPEED = 150.0f;        // 增加滚动速度

// 调整玩家尺寸
config.PLAYER_SIZE = sf::Vector2f(40.0f, 80.0f);

// 保存配置到文件
config.saveToFile("assets/config/game_config.ini");
```

所有相关文件已成功更新，配置系统现在更加简洁易用。
