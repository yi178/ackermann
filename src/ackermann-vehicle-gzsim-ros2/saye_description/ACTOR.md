# ACTOR.md - Gazebo行人Actor配置指南

本文档说明如何在Gazebo仿真中配置和修改行人（Actor）的行为。

## 📖 目录

- [系统概述](#系统概述)
- [配置文件结构](#配置文件结构)
- [参数详解](#参数详解)
- [修改行人数量](#修改行人数量)
- [修改行人轨迹](#修改行人轨迹)
- [修改行人位姿朝向](#修改行人位姿朝向)
- [修改循环和自动启动](#修改循环和自动启动)
- [实际配置示例](#实际配置示例)
- [朝向角度参考](#朝向角度参考)
- [常见问题排查](#常见问题排查)
- [快速命令参考](#快速命令参考)

---

## 系统概述

### 工作原理

本项目使用**模板生成系统**来管理Gazebo行人配置：

```
YAML配置文件 → Python生成脚本 → SDF World文件 → Gazebo加载
   (易于编辑)      (自动转换)        (最终文件)      (仿真运行)
```

### 文件作用

| 文件类型 | 路径 | 作用 | 编辑频率 |
|---------|------|------|---------|
| **YAML配置** | `config/pedestrians*.yaml` | 定义行人参数（数量、位置、轨迹） | 经常 ✏️ |
| **Jinja2模板** | `worlds/templates/*.sdf.jinja2` | SDF世界文件模板 | 很少 |
| **生成脚本** | `scripts/generate_world.py` | 将YAML转换为SDF | 不需要 |
| **生成的World** | `worlds/scenario*.sdf` | 最终的仿真世界文件 | 自动生成 |

**重要**: 只需要编辑YAML配置文件，其他文件会自动生成！

---

## 配置文件结构

### 可用的配置文件

```
config/
├── pedestrians.yaml                    # 默认配置 (2人)
├── pedestrians_4ped_crossing.yaml      # 场景1: 4人交叉
├── pedestrians_8ped_mixed.yaml         # 场景2: 8人混合
├── pedestrians_12ped_crowded.yaml      # 场景3: 12人拥挤
├── pedestrians_6ped_static.yaml        # 场景4: 6人静态
└── pedestrians_6ped_random.yaml        # 场景5: 6人随机
```

### YAML文件基本结构

```yaml
pedestrians:
  - name: walker_1              # 行人名称（必须唯一）
    skin: model://...           # 3D模型路径
    animation: walking          # 动画名称
    loop: true                  # 是否循环轨迹
    auto_start: true            # 是否自动开始
    spawn_pose: [x, y, z, roll, pitch, yaw]  # 初始位置和朝向
    trajectory:                 # 轨迹路点列表
      - time: 0.0               # 时间戳（秒）
        pose: [x, y, z, roll, pitch, yaw]  # 相对位置和朝向
      - time: 5.0
        pose: [...]
```

---

## 参数详解

### 1. 基本信息参数

#### `name` (必需)
- **类型**: 字符串
- **说明**: 行人的唯一标识符
- **示例**: `walker_center`, `pedestrian_001`
- **注意**: 同一世界中不能有重复名称

#### `skin` (必需)
- **类型**: 字符串（模型路径）
- **说明**: 行人的3D模型文件路径
- **默认值**: `model://walking_actor/meshes/walk.dae`
- **注意**: Gazebo内置walking_actor模型，通常不需要修改

#### `animation` (必需)
- **类型**: 字符串
- **说明**: 动画名称
- **默认值**: `walking`
- **注意**: 必须与skin模型中的动画名称匹配

---

### 2. 行为控制参数

#### `loop` (必需)
- **类型**: 布尔值 (true/false)
- **说明**: 轨迹是否循环执行
- **true**: 到达最后一个路点后返回第一个路点
- **false**: 到达最后一个路点后停止

**示例**:
```yaml
loop: true   # 行人会持续循环运动
loop: false  # 行人只走一次轨迹后停止
```

#### `auto_start` (必需)
- **类型**: 布尔值 (true/false)
- **说明**: 是否在仿真开始时自动移动
- **true**: 仿真启动时行人就开始移动
- **false**: 行人保持静止（可用于模拟静态障碍物）

**示例**:
```yaml
auto_start: true   # 正常移动的行人
auto_start: false  # 站立不动的静态障碍物
```

---

### 3. 位置和姿态参数

#### `spawn_pose` (必需)
- **类型**: 数组 [x, y, z, roll, pitch, yaw]
- **说明**: 行人在世界中的初始绝对位置和朝向
- **单位**:
  - `x, y, z`: 米 (m)
  - `roll, pitch, yaw`: 弧度 (rad)

**坐标系说明**:
```
        +Y (左)
         ↑
         |
         |
+X (前) ←─┼─→ -X (后)
         |
         |
         ↓
        -Y (右)
```

**示例**:
```yaml
# 在x=10, y=2的位置，朝向左侧
spawn_pose: [10.0, 2.0, 1.0, 0.0, 0.0, 1.57079632679]
#           └─x   └─y  └─z  └───roll, pitch, yaw────┘
```

#### `trajectory` (必需)
- **类型**: 列表，每个元素包含time和pose
- **说明**: 行人运动的轨迹路点序列
- **最少路点数**: 2个

**路点结构**:
```yaml
trajectory:
  - time: 0.0                    # 第1个路点：起始时刻
    pose: [0.0, 0.0, 0.0, 0.0, 0.0, 1.57]  # 相对于spawn_pose的位置
  - time: 5.0                    # 第2个路点：5秒后
    pose: [5.0, 0.0, 0.0, 0.0, 0.0, 1.57]  # 相对移动了5米
```

**重要**:
- trajectory中的pose是**相对于spawn_pose**的偏移量
- 行人的实际位置 = spawn_pose + trajectory.pose

---

## 修改行人数量

### 增加行人

在YAML文件的`pedestrians:`列表中添加新的行人配置：

```yaml
pedestrians:
  # 现有行人
  - name: walker_1
    # ... 配置 ...

  # 新增行人 - 直接复制粘贴并修改
  - name: walker_new        # ⚠️ 改成唯一的名称
    skin: model://walking_actor/meshes/walk.dae
    animation: walking
    loop: true
    auto_start: true
    spawn_pose: [15.0, 0.0, 1.0, 0.0, 0.0, 0.0]  # ⚠️ 修改位置
    trajectory:
      - time: 0.0
        pose: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
      - time: 5.0
        pose: [5.0, 0.0, 0.0, 0.0, 0.0, 0.0]
      - time: 10.0
        pose: [0.0, 0.0, 0.0, 0.0, 0.0, 3.14159]
```

### 删除行人

直接删除或注释掉整个行人配置块：

```yaml
pedestrians:
  # 保留这个行人
  - name: walker_1
    # ...

  # 删除这个行人 - 直接删除或加#注释
  # - name: walker_2
  #   skin: ...
  #   ...
```

### 快速复制模板

```yaml
  # === 复制这个模板添加新行人 ===
  - name: walker_NAME           # 改名称
    skin: model://walking_actor/meshes/walk.dae
    animation: walking
    loop: true
    auto_start: true
    spawn_pose: [X, Y, 1.0, 0.0, 0.0, YAW]  # 改位置和朝向
    trajectory:
      - time: 0.0
        pose: [0.0, 0.0, 0.0, 0.0, 0.0, YAW]
      - time: T1
        pose: [DX, DY, 0.0, 0.0, 0.0, YAW]
      - time: T2
        pose: [0.0, 0.0, 0.0, 0.0, 0.0, YAW+3.14159]
```

---

## 修改行人轨迹

### 简单直线运动

**前后移动** (沿X轴):
```yaml
spawn_pose: [10.0, 0.0, 1.0, 0.0, 0.0, 0.0]  # 初始朝向前方
trajectory:
  - time: 0.0
    pose: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]     # 起点
  - time: 5.0
    pose: [5.0, 0.0, 0.0, 0.0, 0.0, 0.0]     # 向前走5米
  - time: 6.0
    pose: [5.0, 0.0, 0.0, 0.0, 0.0, 3.14159] # 转身
  - time: 11.0
    pose: [0.0, 0.0, 0.0, 0.0, 0.0, 3.14159] # 走回起点
  - time: 12.0
    pose: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]     # 转身朝前
```

**左右移动** (沿Y轴):
```yaml
spawn_pose: [10.0, 0.0, 1.0, 0.0, 0.0, 1.57079632679]  # 初始朝向左侧
trajectory:
  - time: 0.0
    pose: [0.0, 0.0, 0.0, 0.0, 0.0, 1.57079632679]     # 起点
  - time: 4.0
    pose: [0.0, 3.0, 0.0, 0.0, 0.0, 1.57079632679]     # 向左走3米
  - time: 4.5
    pose: [0.0, 3.0, 0.0, 0.0, 0.0, -1.57079632679]    # 转身朝右
  - time: 8.5
    pose: [0.0, 0.0, 0.0, 0.0, 0.0, -1.57079632679]    # 走回起点
  - time: 9.0
    pose: [0.0, 0.0, 0.0, 0.0, 0.0, 1.57079632679]     # 转身朝左
```

### 对角线运动

```yaml
spawn_pose: [10.0, 0.0, 1.0, 0.0, 0.0, 0.785398]  # 朝向东北45°
trajectory:
  - time: 0.0
    pose: [0.0, 0.0, 0.0, 0.0, 0.0, 0.785398]
  - time: 5.0
    pose: [3.0, 3.0, 0.0, 0.0, 0.0, 0.785398]      # 向东北移动
  - time: 6.0
    pose: [3.0, 3.0, 0.0, 0.0, 0.0, -2.356194]     # 转向西南
  - time: 11.0
    pose: [0.0, 0.0, 0.0, 0.0, 0.0, -2.356194]     # 回到起点
```

### 矩形路径

```yaml
spawn_pose: [10.0, 0.0, 1.0, 0.0, 0.0, 0.0]
trajectory:
  - time: 0.0
    pose: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]           # 起点，朝前
  - time: 3.0
    pose: [3.0, 0.0, 0.0, 0.0, 0.0, 0.0]           # 向前3米
  - time: 3.5
    pose: [3.0, 0.0, 0.0, 0.0, 0.0, 1.5708]        # 转向左
  - time: 5.5
    pose: [3.0, 2.0, 0.0, 0.0, 0.0, 1.5708]        # 向左2米
  - time: 6.0
    pose: [3.0, 2.0, 0.0, 0.0, 0.0, 3.14159]       # 转向后
  - time: 9.0
    pose: [0.0, 2.0, 0.0, 0.0, 0.0, 3.14159]       # 向后3米
  - time: 9.5
    pose: [0.0, 2.0, 0.0, 0.0, 0.0, -1.5708]       # 转向右
  - time: 11.5
    pose: [0.0, 0.0, 0.0, 0.0, 0.0, -1.5708]       # 向右2米
  - time: 12.0
    pose: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]           # 转向前
```

### 调整速度

速度 = 距离 / 时间

**慢速移动 (0.5 m/s)**:
```yaml
  - time: 0.0
    pose: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
  - time: 10.0              # 10秒走5米 = 0.5 m/s
    pose: [5.0, 0.0, 0.0, 0.0, 0.0, 0.0]
```

**正常速度 (1.2 m/s)**:
```yaml
  - time: 0.0
    pose: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
  - time: 5.0               # 5秒走6米 = 1.2 m/s
    pose: [6.0, 0.0, 0.0, 0.0, 0.0, 0.0]
```

**快速移动 (2.0 m/s)**:
```yaml
  - time: 0.0
    pose: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
  - time: 5.0               # 5秒走10米 = 2.0 m/s
    pose: [10.0, 0.0, 0.0, 0.0, 0.0, 0.0]
```

---

## 修改行人位姿朝向

### 朝向与运动方向必须一致！

**错误示例** ❌:
```yaml
spawn_pose: [10.0, 0.0, 1.0, 0.0, 0.0, 0.0]      # 朝向前方
trajectory:
  - time: 0.0
    pose: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]         # 朝向前方
  - time: 5.0
    pose: [0.0, 3.0, 0.0, 0.0, 0.0, 0.0]         # ❌ 向左移动但朝向还是前方
                                                 # 结果：横着走/月球漫步
```

**正确示例** ✅:
```yaml
spawn_pose: [10.0, 0.0, 1.0, 0.0, 0.0, 1.5708]  # 朝向左侧
trajectory:
  - time: 0.0
    pose: [0.0, 0.0, 0.0, 0.0, 0.0, 1.5708]     # 朝向左侧
  - time: 5.0
    pose: [0.0, 3.0, 0.0, 0.0, 0.0, 1.5708]     # ✅ 向左移动且朝向左侧
```

### Yaw角度计算

Yaw角度决定行人的朝向，使用**弧度制**：

| 方向 | Yaw (弧度) | Yaw (角度) | 说明 |
|------|-----------|-----------|------|
| 前 (+X) | `0.0` | 0° | 走廊前进方向 |
| 左 (+Y) | `1.57079632679` | 90° | π/2 |
| 后 (-X) | `3.14159265359` | 180° | π |
| 右 (-Y) | `-1.57079632679` | -90° | -π/2 |
| 东北 | `0.785398` | 45° | π/4 |
| 西北 | `2.356194` | 135° | 3π/4 |
| 东南 | `-0.785398` | -45° | -π/4 |
| 西南 | `-2.356194` | -135° | -3π/4 |

### 转身动作

行人转身需要在同一位置添加两个路点，只改变yaw：

```yaml
trajectory:
  - time: 5.0
    pose: [5.0, 0.0, 0.0, 0.0, 0.0, 0.0]         # 到达位置，朝前
  - time: 5.5                                    # 0.5秒转身时间
    pose: [5.0, 0.0, 0.0, 0.0, 0.0, 3.14159]     # 同一位置，转身朝后
  - time: 10.5
    pose: [0.0, 0.0, 0.0, 0.0, 0.0, 3.14159]     # 向后走
```

---

## 修改循环和自动启动

### Loop循环行为

#### 循环轨迹 (loop: true)
```yaml
loop: true
auto_start: true
trajectory:
  - time: 0.0
    pose: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
  - time: 10.0
    pose: [10.0, 0.0, 0.0, 0.0, 0.0, 0.0]
# 到达最后一个路点后，自动返回第一个路点继续循环
```

#### 单次轨迹 (loop: false)
```yaml
loop: false
auto_start: true
trajectory:
  - time: 0.0
    pose: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
  - time: 10.0
    pose: [10.0, 0.0, 0.0, 0.0, 0.0, 0.0]
# 到达最后一个路点后，停止不动
```

### Auto_start自动启动

#### 动态行人 (auto_start: true)
```yaml
auto_start: true    # 仿真开始时就移动
loop: true
```

#### 静态障碍物 (auto_start: false)
```yaml
auto_start: false   # 保持静止，模拟静态障碍物
loop: false
trajectory:
  # 即使有轨迹也不会移动
  - time: 0.0
    pose: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
  - time: 100.0
    pose: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]  # 两个相同的路点
```

### 组合使用场景

| loop | auto_start | 效果 |
|------|-----------|------|
| true | true | 正常循环运动的行人 |
| false | true | 走一次轨迹后停止 |
| true | false | 静止不动（轨迹被忽略） |
| false | false | 静止不动（轨迹被忽略） |

---

## 实际配置示例

### 示例1: 快速横穿行人

```yaml
- name: fast_crossing_pedestrian
  skin: model://walking_actor/meshes/walk.dae
  animation: walking
  loop: true
  auto_start: true
  spawn_pose: [15.0, 2.5, 1.0, 0.0, 0.0, -1.5708]  # 右侧，朝右
  trajectory:
    # 快速横穿 (2.0 m/s)
    - time: 0.0
      pose: [0.0, 0.0, 0.0, 0.0, 0.0, -1.5708]
    - time: 2.5                            # 5米/2.5秒 = 2 m/s
      pose: [0.0, -5.0, 0.0, 0.0, 0.0, -1.5708]
    - time: 3.0
      pose: [0.0, -5.0, 0.0, 0.0, 0.0, 1.5708]  # 转身
    - time: 5.5
      pose: [0.0, 0.0, 0.0, 0.0, 0.0, 1.5708]
    - time: 6.0
      pose: [0.0, 0.0, 0.0, 0.0, 0.0, -1.5708]
```

### 示例2: 巡逻行人

```yaml
- name: patrol_guard
  skin: model://walking_actor/meshes/walk.dae
  animation: walking
  loop: true
  auto_start: true
  spawn_pose: [12.0, 1.5, 1.0, 0.0, 0.0, 0.0]  # 左侧，朝前
  trajectory:
    # 短距离来回巡逻
    - time: 0.0
      pose: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
    - time: 3.0
      pose: [4.0, 0.0, 0.0, 0.0, 0.0, 0.0]     # 向前4米
    - time: 3.5
      pose: [4.0, 0.0, 0.0, 0.0, 0.0, 3.14159] # 转身
    - time: 6.5
      pose: [0.0, 0.0, 0.0, 0.0, 0.0, 3.14159] # 向后4米
    - time: 7.0
      pose: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]     # 转身
```

### 示例3: 静态障碍行人

```yaml
- name: standing_obstacle
  skin: model://walking_actor/meshes/walk.dae
  animation: walking
  loop: false
  auto_start: false    # 关键：不自动启动
  spawn_pose: [14.0, 0.0, 1.0, 0.0, 0.0, 0.0]
  trajectory:
    # 两个相同的路点，保持静止
    - time: 0.0
      pose: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
    - time: 100.0
      pose: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
```

### 示例4: 慢速老人

```yaml
- name: slow_elder
  skin: model://walking_actor/meshes/walk.dae
  animation: walking
  loop: true
  auto_start: true
  spawn_pose: [16.0, -1.0, 1.0, 0.0, 0.0, 1.5708]  # 朝左
  trajectory:
    # 非常慢 (0.3 m/s)
    - time: 0.0
      pose: [0.0, 0.0, 0.0, 0.0, 0.0, 1.5708]
    - time: 10.0                           # 3米/10秒 = 0.3 m/s
      pose: [0.0, 3.0, 0.0, 0.0, 0.0, 1.5708]
    - time: 11.0
      pose: [0.0, 3.0, 0.0, 0.0, 0.0, -1.5708]
    - time: 21.0
      pose: [0.0, 0.0, 0.0, 0.0, 0.0, -1.5708]
    - time: 22.0
      pose: [0.0, 0.0, 0.0, 0.0, 0.0, 1.5708]
```

---

## 朝向角度参考

### 常用角度速查表

```yaml
# 主要方向（正交）
yaw: 0.0             # 0°   - 前方 (+X)
yaw: 1.57079632679   # 90°  - 左侧 (+Y)
yaw: 3.14159265359   # 180° - 后方 (-X)
yaw: -1.57079632679  # -90° - 右侧 (-Y)

# 对角方向（45°）
yaw: 0.785398        # 45°   - 东北
yaw: 2.356194        # 135°  - 西北
yaw: -2.356194       # -135° - 西南
yaw: -0.785398       # -45°  - 东南

# 其他常用角度
yaw: 0.523599        # 30°   - π/6
yaw: 1.047197        # 60°   - π/3
yaw: 2.617993        # 150°  - 5π/6
```

### 角度转换公式

- **角度 → 弧度**: `弧度 = 角度 × π / 180`
- **弧度 → 角度**: `角度 = 弧度 × 180 / π`

**在线计算器**:
- Python: `import math; math.radians(90)`
- 计算器: 90 × 3.14159 / 180 = 1.5708

### 可视化参考

```
           +Y (左, 1.5708)
                 ↑
                 |
                 |
 +X (前, 0.0) ←──┼──→ -X (后, 3.14159)
                 |
                 |
                 ↓
           -Y (右, -1.5708)
```

---

## 常见问题排查

### 问题1: 行人横着走/月球漫步

**症状**: 行人的朝向和移动方向不一致

**原因**: trajectory中的yaw角度与运动方向不匹配

**解决方案**:
```yaml
# 错误 ❌
trajectory:
  - time: 0.0
    pose: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]      # 朝前
  - time: 5.0
    pose: [0.0, 3.0, 0.0, 0.0, 0.0, 0.0]      # 向左走但朝向还是前

# 正确 ✅
trajectory:
  - time: 0.0
    pose: [0.0, 0.0, 0.0, 0.0, 0.0, 1.5708]   # 朝左
  - time: 5.0
    pose: [0.0, 3.0, 0.0, 0.0, 0.0, 1.5708]   # 向左走且朝向左
```

### 问题2: 行人不出现

**可能原因**:
1. spawn_pose超出了世界范围
2. 名称与其他行人重复
3. YAML格式错误（缩进不对）

**检查方法**:
```bash
# 验证YAML格式
python3 -c "import yaml; yaml.safe_load(open('config/pedestrians.yaml'))"

# 查看生成的SDF
grep -A 5 "actor name=" worlds/scenario1_4ped_crossing.sdf
```

### 问题3: 行人轨迹错误

**错误**: `trajectory must have at least 2 waypoints`

**解决方案**: 确保每个行人至少有2个路点
```yaml
trajectory:
  - time: 0.0
    pose: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
  - time: 5.0          # 至少需要第2个路点
    pose: [5.0, 0.0, 0.0, 0.0, 0.0, 0.0]
```

### 问题4: 修改后不生效

**原因**: 没有重新构建

**解决方案**:
```bash
# 修改YAML后必须重新构建
colcon build --packages-select saye_description
source install/setup.bash
```

### 问题5: 行人速度太快/太慢

**调整方法**: 修改时间间隔

```yaml
# 太快 (3 m/s) - 增加时间
- time: 0.0
  pose: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
- time: 1.0    # 改为 3.0
  pose: [3.0, 0.0, 0.0, 0.0, 0.0, 0.0]

# 太慢 (0.5 m/s) - 减少时间
- time: 0.0
  pose: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
- time: 10.0   # 改为 4.0
  pose: [5.0, 0.0, 0.0, 0.0, 0.0, 0.0]
```

---

## 快速命令参考

### 编辑配置
```bash
# 编辑默认配置
nano src/ackermann-vehicle-gzsim-ros2/saye_description/config/pedestrians.yaml

# 编辑特定场景
nano src/ackermann-vehicle-gzsim-ros2/saye_description/config/pedestrians_4ped_crossing.yaml
```

### 构建生成
```bash
# 重新生成所有world文件
colcon build --packages-select saye_description --allow-overriding saye_description
source install/setup.bash
```

### 测试验证
```bash
# 检查生成的行人数量
grep -c "<actor name=" install/saye_description/share/saye_description/worlds/scenario1_4ped_crossing.sdf

# 查看生成的行人列表
grep "actor name=" install/saye_description/share/saye_description/worlds/scenario1_4ped_crossing.sdf
```

### 运行仿真
```bash
# 启动默认场景
ros2 launch saye_bringup_my saye_spawn.launch.py

# 启动特定场景
ros2 launch saye_bringup_my saye_spawn.launch.py world:=scenario1_4ped_crossing.sdf
ros2 launch saye_bringup_my saye_spawn.launch.py world:=scenario2_8ped_mixed.sdf
ros2 launch saye_bringup_my saye_spawn.launch.py world:=scenario3_12ped_crowded.sdf
ros2 launch saye_bringup_my saye_spawn.launch.py world:=scenario4_6ped_static.sdf
ros2 launch saye_bringup_my saye_spawn.launch.py world:=scenario5_6ped_random.sdf
```

### 快速调试
```bash
# 手动测试生成脚本
python3 src/ackermann-vehicle-gzsim-ros2/saye_description/scripts/generate_world.py \
  --template src/ackermann-vehicle-gzsim-ros2/saye_description/worlds/templates/saye_dynamic_corridor.sdf.jinja2 \
  --config src/ackermann-vehicle-gzsim-ros2/saye_description/config/pedestrians.yaml \
  --output /tmp/test_world.sdf

# 查看生成结果
cat /tmp/test_world.sdf | grep -A 10 "actor name"
```

---

## 总结

### 修改流程

1. **编辑YAML配置文件** (`config/pedestrians*.yaml`)
   - 修改现有行人参数
   - 添加/删除行人
   - 调整轨迹和朝向

2. **重新构建**
   ```bash
   colcon build --packages-select saye_description
   source install/setup.bash
   ```

3. **启动仿真测试**
   ```bash
   ros2 launch saye_bringup_my saye_spawn.launch.py world:=scenario1_4ped_crossing.sdf
   ```

### 关键要点

- ✅ **只编辑YAML文件**，不要手动编辑生成的SDF
- ✅ **朝向必须与运动方向一致**，否则会横着走
- ✅ **至少2个路点**，否则会报错
- ✅ **修改后必须重新构建**，否则不生效
- ✅ **名称必须唯一**，否则会冲突

### 推荐速度参考

- 静态障碍: `auto_start: false`
- 慢速老人: 0.3 - 0.5 m/s
- 正常行走: 1.0 - 1.5 m/s
- 快速行走: 1.5 - 2.0 m/s
- 小跑: 2.0 - 3.0 m/s

---

## 相关文档

- **README.md** - 项目总体说明
- **CLAUDE.md** - Claude Code开发指南
- **memory.md** - 项目进展记录
- **task.md** - 定制化摘要

## 技术支持

如有问题，检查：
1. YAML语法是否正确（缩进、格式）
2. 是否重新构建了项目
3. 行人名称是否唯一
4. 朝向角度是否与运动方向匹配
5. 查看Gazebo日志输出

---

**最后更新**: 2025-10-30
