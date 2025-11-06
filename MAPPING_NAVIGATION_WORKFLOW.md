# SLAM建图与动态导航完整工作流程

本文档说明如何使用**无行人world进行SLAM建图**，然后使用**有行人world进行导航测试**的完整流程。

---

## 📖 目录

- [工作流程概览](#工作流程概览)
- [为什么需要分开建图和导航](#为什么需要分开建图和导航)
- [可用的World文件](#可用的world文件)
- [阶段1: SLAM建图（无行人）](#阶段1-slam建图无行人)
- [阶段2: 保存地图](#阶段2-保存地图)
- [阶段3: 动态导航测试（有行人）](#阶段3-动态导航测试有行人)
- [阶段4: MPPI实验](#阶段4-mppi实验)
- [完整命令参考](#完整命令参考)
- [常见问题](#常见问题)

---

## 工作流程概览

```
┌─────────────────────────────────────────────────────────────────┐
│                     完整工作流程                                  │
└─────────────────────────────────────────────────────────────────┘

阶段1: SLAM建图 (无行人)
┌──────────────────────────────────────────────────────┐
│ corridor_empty.sdf (无行人)                           │
│                                                       │
│  [机器人] → SLAM Toolbox → 建立地图                   │
│                                                       │
│  手动控制机器人遍历整个走廊                              │
└──────────────────────────────────────────────────────┘
                        ↓
                   保存地图文件
        (corridor_map.yaml + corridor_map.pgm)
                        ↓
阶段2: 动态导航测试 (有行人)
┌──────────────────────────────────────────────────────┐
│ scenario1_4ped_crossing.sdf (4人)                     │
│                                                       │
│  [机器人] + [行人] → Nav2 + MPPI → 动态避障            │
│                                                       │
│  使用之前保存的地图 + AMCL定位                          │
└──────────────────────────────────────────────────────┘
```

---

## 为什么需要分开建图和导航

### ❌ 错误做法：在有行人的world中建图

**问题**:
1. 行人会被当作静态障碍物记录到地图中
2. 地图中会有行人的"影子"
3. 导航时实际行人位置与地图不符，导致路径规划错误
4. AMCL定位会被行人干扰，定位不准

### ✅ 正确做法：分开建图和导航

**优势**:
1. 地图只包含静态环境（墙壁、alcoves等）
2. 行人是实时感知的动态障碍物
3. MPPI可以正确预测和避让行人
4. 定位准确，基于静态地图特征

---

## 可用的World文件

### 建图专用（无行人）

| World文件 | 行人数 | 环境 | 用途 |
|----------|-------|------|------|
| **corridor_empty.sdf** ⭐ | 0人 | 完整走廊+alcoves | **建图首选** |
| saye_long_corridor.sdf | 0人 | 简单走廊 | 简单测试 |

### 导航测试（有行人）

| World文件 | 行人数 | 场景特点 | 难度 |
|----------|-------|---------|------|
| **scenario1_4ped_crossing.sdf** | 4人 | 基础交叉 | ⭐ 简单 |
| **scenario2_8ped_mixed.sdf** | 8人 | 混合运动 | ⭐⭐ 中等 |
| **scenario3_12ped_crowded.sdf** | 12人 | 高密度拥挤 | ⭐⭐⭐ 困难 |
| **scenario4_6ped_static.sdf** | 6人 | 静态+慢速 | ⭐ 简单 |
| **scenario5_6ped_random.sdf** | 6人 | 随机运动 | ⭐⭐⭐ 困难 |

---

## 阶段1: SLAM建图（无行人）

### 步骤1.1: 启动无行人仿真

**终端1** - 启动Gazebo和机器人：
```bash
cd ~/ackermann_sim
source install/setup.bash

# 启动无行人的走廊world
ros2 launch saye_bringup_my saye_spawn.launch.py world:=corridor_empty.sdf
```

**验证**:
- Gazebo中应该只看到机器人和走廊
- **没有任何行人**
- 走廊中有alcoves（凹进去的区域）

### 步骤1.2: 启动SLAM

**终端2** - 启动SLAM Toolbox：
```bash
cd ~/ackermann_sim
source install/setup.bash

ros2 launch saye_bringup_my slam.launch.py
```

**验证**:
- RViz2窗口打开
- 看到机器人模型
- 开始看到激光雷达扫描点云
- 地图逐渐显示

### 步骤1.3: 手动遍历走廊

**终端3** - 键盘控制：
```bash
cd ~/ackermann_sim
source install/setup.bash

ros2 run teleop_twist_keyboard teleop_twist_keyboard
```

**建图要点**:
1. **慢速移动**: 速度太快会导致地图模糊
2. **完整覆盖**:
   - 沿走廊从头走到尾
   - 进入每个alcove凹进区域
   - 尽量靠近墙壁以获得清晰边界
3. **旋转扫描**: 在关键位置旋转360°以获得完整扫描
4. **重复经过**: 多次经过同一区域提高地图质量

**建议路径**:
```
起点 (x=5) → 向前移动 → 进入左侧alcove1 → 退出 →
继续前进 → 进入右侧alcove1 → 退出 →
继续前进 → 进入左侧alcove2 → 退出 →
继续前进 → 进入右侧alcove2 → 退出 →
到达终点 (x=23) → 原路返回
```

**预计时间**: 5-10分钟

---

## 阶段2: 保存地图

### 步骤2.1: 检查地图质量

在RViz2中观察地图：
- ✅ 墙壁边界清晰
- ✅ Alcoves区域完整
- ✅ 没有孤立的噪点
- ✅ 走廊宽度一致

### 步骤2.2: 保存地图

**终端4** - 保存地图：
```bash
cd ~/ackermann_sim
source install/setup.bash

# 保存到maps目录
ros2 run nav2_map_server map_saver_cli -f \
  src/ackermann-vehicle-gzsim-ros2/saye_bringup_my/maps/corridor_map
```

**生成的文件**:
```
saye_bringup_my/maps/
├── corridor_map.yaml    # 地图元数据
└── corridor_map.pgm     # 地图图像
```

### 步骤2.3: 验证地图文件

```bash
# 查看地图文件
ls -lh src/ackermann-vehicle-gzsim-ros2/saye_bringup_my/maps/

# 查看地图元数据
cat src/ackermann-vehicle-gzsim-ros2/saye_bringup_my/maps/corridor_map.yaml
```

**地图文件说明**:
- `corridor_map.yaml`: 分辨率、原点、阈值等参数
- `corridor_map.pgm`: 黑白图像（白色=自由空间，黑色=障碍物）

---

## 阶段3: 动态导航测试（有行人）

### 步骤3.1: 关闭SLAM窗口

**重要**: 先关闭阶段1的所有终端（Gazebo、SLAM、teleop）

```bash
# 在每个终端按 Ctrl+C 停止
```

### 步骤3.2: 启动有行人仿真

**终端1** - 启动带行人的world：
```bash
cd ~/ackermann_sim
source install/setup.bash

# 选择一个场景启动
ros2 launch saye_bringup_my saye_spawn.launch.py \
  world:=scenario1_4ped_crossing.sdf
```

**场景选择建议**:
- 初次测试: `scenario1_4ped_crossing.sdf` (4人，简单)
- 压力测试: `scenario3_12ped_crowded.sdf` (12人，困难)

**验证**:
- Gazebo中看到机器人
- **看到行人在移动**
- 环境与建图时完全相同（只是多了行人）

### 步骤3.3: 修改Nav2参数使用新地图

编辑导航参数文件：
```bash
nano src/ackermann-vehicle-gzsim-ros2/saye_bringup_my/config/nav2_params.yaml
```

找到`map_server`部分，修改地图路径：
```yaml
map_server:
  ros__parameters:
    yaml_filename: "corridor_map.yaml"  # 使用你保存的地图
```

或者使用绝对路径：
```yaml
map_server:
  ros__parameters:
    yaml_filename: "/home/yiyi/ackermann_sim/src/ackermann-vehicle-gzsim-ros2/saye_bringup_my/maps/corridor_map.yaml"
```

### 步骤3.4: 启动导航

**终端2** - 启动Nav2导航栈：
```bash
cd ~/ackermann_sim
source install/setup.bash

ros2 launch saye_bringup_my navigation_bringup.launch.py
```

**验证**:
- RViz2窗口打开
- 加载了你保存的地图
- 看到行人在地图上移动（作为动态障碍物）
- AMCL粒子云显示定位

### 步骤3.5: 设置初始位姿

在RViz2中：
1. 点击顶部的 **"2D Pose Estimate"** 按钮
2. 在地图上点击机器人的**实际位置**
3. 拖动箭头设置朝向
4. 松开鼠标

**验证定位成功**:
- AMCL粒子云收敛到机器人周围
- 激光雷达扫描与地图墙壁对齐

### 步骤3.6: 发送导航目标

在RViz2中：
1. 点击顶部的 **"2D Goal Pose"** 按钮
2. 在地图上点击**目标位置**
3. 拖动箭头设置目标朝向
4. 松开鼠标

**观察机器人行为**:
- ✅ 规划全局路径（绿色线）
- ✅ 生成局部路径（蓝色线）
- ✅ 绕过行人（动态避障）
- ✅ 平滑运动到目标

### 步骤3.7: 测试不同场景

逐步提高难度：

**场景1**: 基础测试
```bash
ros2 launch saye_bringup_my saye_spawn.launch.py world:=scenario1_4ped_crossing.sdf
```

**场景2**: 中等密度
```bash
ros2 launch saye_bringup_my saye_spawn.launch.py world:=scenario2_8ped_mixed.sdf
```

**场景3**: 高密度压力测试
```bash
ros2 launch saye_bringup_my saye_spawn.launch.py world:=scenario3_12ped_crowded.sdf
```

---

## 阶段4: MPPI实验

### 使用FollowPath测试MPPI

**终端1** - 启动MPPI实验：
```bash
cd ~/ackermann_sim
source install/setup.bash

ros2 launch saye_mppi mppi_experiment.launch.py
```

**注意**:
- `mppi_experiment.launch.py` 目前使用默认world
- 需要修改launch文件添加world参数支持

### 修改MPPI实验支持world选择

编辑launch文件：
```bash
nano src/ackermann-vehicle-gzsim-ros2/saye_mppi/launch/mppi_experiment.launch.py
```

在文件开头添加：
```python
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration

def generate_launch_description():
    # 添加world参数
    world_arg = DeclareLaunchArgument(
        'world',
        default_value='corridor_empty.sdf',
        description='World file to load'
    )

    # ... 其余代码
```

然后在`saye_sim`的`IncludeLaunchDescription`中传递参数：
```python
saye_sim = IncludeLaunchDescription(
    PythonLaunchDescriptionSource(
        os.path.join(pkg_saye_bringup, 'launch', 'saye_spawn.launch.py')),
    launch_arguments={'world': LaunchConfiguration('world')}.items()
)
```

### 运行MPPI实验（修改后）

```bash
# 无行人建图
ros2 launch saye_mppi mppi_experiment.launch.py world:=corridor_empty.sdf

# 4人场景测试
ros2 launch saye_mppi mppi_experiment.launch.py world:=scenario1_4ped_crossing.sdf

# 12人压力测试
ros2 launch saye_mppi mppi_experiment.launch.py world:=scenario3_12ped_crowded.sdf
```

---

## 完整命令参考

### 快速建图流程

```bash
# 终端1: 启动无行人仿真
ros2 launch saye_bringup_my saye_spawn.launch.py world:=corridor_empty.sdf

# 终端2: 启动SLAM
ros2 launch saye_bringup_my slam.launch.py

# 终端3: 手动控制建图
ros2 run teleop_twist_keyboard teleop_twist_keyboard

# 终端4: 保存地图（建图完成后）
ros2 run nav2_map_server map_saver_cli -f \
  src/ackermann-vehicle-gzsim-ros2/saye_bringup_my/maps/corridor_map
```

### 快速导航测试流程

```bash
# 终端1: 启动有行人仿真
ros2 launch saye_bringup_my saye_spawn.launch.py world:=scenario1_4ped_crossing.sdf

# 终端2: 启动导航（确保nav2_params.yaml中地图路径正确）
ros2 launch saye_bringup_my navigation_bringup.launch.py

# RViz2中：
# 1. 设置初始位姿 (2D Pose Estimate)
# 2. 发送导航目标 (2D Goal Pose)
```

### World文件选择速查

```bash
# 建图
world:=corridor_empty.sdf         # 无行人，完整环境

# 导航测试
world:=scenario1_4ped_crossing.sdf    # 4人交叉
world:=scenario2_8ped_mixed.sdf       # 8人混合
world:=scenario3_12ped_crowded.sdf    # 12人拥挤
world:=scenario4_6ped_static.sdf      # 6人静态
world:=scenario5_6ped_random.sdf      # 6人随机
```

---

## 常见问题

### Q1: 建图时地图很模糊怎么办？

**解决方案**:
1. 降低机器人速度，慢速移动
2. 在关键区域停留并旋转
3. 多次经过同一区域
4. 确保激光雷达正常工作

### Q2: 导航时机器人冲向行人？

**原因**: MPPI参数不合适或地图不准确

**解决方案**:
1. 检查地图是否正确加载
2. 调整MPPI参数（`saye_mppi/config/mppi_nav2_params.yaml`）
3. 增加安全距离参数

### Q3: 定位不准确怎么办？

**解决方案**:
1. 确保使用正确的地图
2. 手动设置初始位姿更准确
3. 增加AMCL粒子数量
4. 检查里程计数据是否正常

### Q4: 如何验证地图质量？

```bash
# 在RViz2中加载地图
ros2 launch nav2_bringup rviz_launch.py

# 或使用image viewer查看pgm文件
eog src/ackermann-vehicle-gzsim-ros2/saye_bringup_my/maps/corridor_map.pgm
```

### Q5: 切换场景后需要重新建图吗？

**不需要！** 所有场景使用相同的静态环境：
- ✅ 同样的墙壁
- ✅ 同样的alcoves
- ✅ 同样的尺寸

只是行人数量和轨迹不同，使用同一张地图即可。

### Q6: 如何评估不同场景的MPPI性能？

**建议指标**:
1. **完成时间**: 从起点到终点的时间
2. **路径长度**: 实际行驶距离
3. **最小安全距离**: 与行人的最近距离
4. **碰撞次数**: 是否发生碰撞
5. **轨迹平滑度**: 速度和角速度变化率

**记录方法**:
```bash
# 记录机器人轨迹
ros2 bag record /odom /tf /scan /cmd_vel

# 后期分析
ros2 bag play <bag_file>
```

---

## 总结

### 核心流程

1. **建图**: 使用 `corridor_empty.sdf` (无行人) + SLAM
2. **保存地图**: `map_saver_cli` 保存地图文件
3. **导航测试**: 使用 `scenario*.sdf` (有行人) + Nav2 + 保存的地图
4. **MPPI实验**: 测试不同场景下的控制器性能

### 关键原则

- ✅ **建图必须无行人** - 保证地图纯净
- ✅ **导航使用静态地图** - 地图只包含静态环境
- ✅ **行人作为动态障碍** - 实时感知和避让
- ✅ **环境保持一致** - 建图和导航使用相同的静态环境

### 实验建议

1. 先用简单场景（scenario1）验证系统工作
2. 逐步增加难度到复杂场景（scenario3）
3. 记录每个场景的性能指标
4. 调整MPPI参数优化性能

---

**相关文档**:
- `ACTOR.md` - 行人配置详细说明
- `CLAUDE.md` - 项目开发指南
- `memory.md` - 项目进展记录

**最后更新**: 2025-10-30
