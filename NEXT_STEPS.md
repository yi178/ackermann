# RAPA-MPPI 下一步操作指南

## 📋 项目现状总结

### ✅ 已完成的工作

1. **基础仿真环境**
   - Gazebo Harmonic + ROS 2 Jazzy 配置完成
   - Ackermann转向车辆模型就绪
   - 多个测试场景世界文件（空走廊、单/多行人）

2. **动态障碍物系统**
   - Physics-based Models 方案（使用TrajectoryFollower插件）
   - 行人以球体形式表示，支持位姿发布
   - 可通过修改SDF轻松调整行人数目和轨迹

3. **GMM预测系统**
   - `ground_truth_predictor` 节点（CV模型）
   - 订阅 `/model/walker_*/pose`
   - 发布 `/pedestrian_predictions` (PedestrianPredictions消息)
   - 预测时域：2.8秒，时间步长：0.05秒（56步）
   - RViz可视化器（轨迹、不确定性椭圆）

4. **基础MPPI系统**
   - Nav2 MPPI控制器配置
   - `saye_mppi` 包（实验性launch文件）
   - 绕过全局规划器的测试方案（FollowPath action）

### ❌ 待实现的核心功能

**RAPA-MPPI框架的三大支柱：**

1. **风险评估模块**（Phase 2-3）
   - ❌ CP（碰撞概率）计算
   - ❌ CVaR（条件风险值）评估
   - ❌ 两阶段混合风险评估流程

2. **RiskAwareCritic**（Phase 1-5）
   - ❌ Nav2 Critic插件骨架
   - ❌ 集成到MPPI控制器

3. **多辅助控制器采样**（Phase 4，可选）
   - ❌ Guide控制器（Pure Pursuit）
   - ❌ Brake控制器（安全冗余）

---

## 🚀 立即开始：Phase 1 快速启动

### 方式A：自动化脚本（推荐）

```bash
cd ~/ackermann_sim
./setup_phase1.sh
```

**脚本功能：**
- 创建 `saye_rapa_mppi` 包
- 生成框架代码（最小可编译版本）
- 配置 pluginlib 和参数文件
- 自动编译

**预期输出：**
```
✓ 工作空间路径正确
✓ 包创建成功
✓ 目录结构创建完成
✓ 配置文件创建完成
✓ 头文件创建完成
✓ 源文件创建完成
✓ 构建配置文件更新完成
[编译输出...]
Phase 1 设置成功完成！
```

### 方式B：手动创建

参考 `RAPA_MPPI_IMPLEMENTATION_PLAN.md` 的 Phase 1 部分。

---

## ✅ Phase 1 验证清单

### 1. 编译验证

```bash
cd ~/ackermann_sim
source install/setup.bash

# 检查包是否安装
ros2 pkg list | grep saye_rapa_mppi
# 应该输出: saye_rapa_mppi

# 检查插件是否注册
ros2 plugin list nav2_mppi_controller | grep RiskAwareCritic
# 应该输出: saye_rapa_mppi::RiskAwareCritic
```

### 2. 运行时验证

**终端1：启动仿真**
```bash
source install/setup.bash
ros2 launch saye_bringup_my saye_spawn.launch.py \
  world:=saye_moving_obstacles.sdf
```

**终端2：启动预测器**
```bash
source install/setup.bash
ros2 launch saye_prediction ground_truth_prediction.launch.py
```

**终端3：启动MPPI（带新Critic）**
```bash
source install/setup.bash

# 使用RAPA-MPPI配置文件
ros2 launch saye_mppi mppi_experiment.launch.py \
  controller_params:=$(ros2 pkg prefix saye_rapa_mppi)/share/saye_rapa_mppi/config/rapa_mppi_params.yaml
```

**终端4：监控日志**
```bash
ros2 topic echo /rosout | grep RiskAwareCritic
```

**预期看到：**
```
[controller_server]: RiskAwareCritic initialized (Phase 1 - Framework Only)
[controller_server]: RiskAwareCritic: Received predictions for 2 pedestrians
```

### 3. 功能验证

**检查预测数据订阅：**
```bash
ros2 topic info /pedestrian_predictions --verbose
```
应该看到 `controller_server` 节点在订阅列表中。

**检查critic权重：**
```bash
ros2 param get /controller_server FollowPath.RiskAwareCritic.cost_weight
# 应该输出: 50.0
```

---

## 📚 核心文档导航

### 1. 理论基础
- **1.md** - RAPA-MPPI完整理论框架（包含公式）
- **memory.md** - 项目开发历史和关键决策

### 2. 实施方案
- **RAPA_MPPI_IMPLEMENTATION_PLAN.md** ⭐
  - 完整的分阶段实施方案
  - 详细代码示例
  - 参数调优指南
  - 测试方案

### 3. 系统架构
- **SYSTEM_ARCHITECTURE.md** - GMM预测系统架构
- **CLAUDE.md** - 项目总体说明
- **READY_TO_TEST.md** - 当前系统测试指南

### 4. 测试场景
- **TEST_NOW.md** - 快速测试步骤
- **TEST_SCENARIOS.md** - 多场景测试流程
- **SCENARIO_DIAGRAM.md** - 场景可视化图解

---

## 🛠️ Phase 2-3：实现风险评估（接下来）

### Phase 2: CP碰撞概率计算（3-4天）

**任务清单：**

1. ✅ 创建 `collision_probability.hpp/cpp`
   - 参考 `RAPA_MPPI_IMPLEMENTATION_PLAN.md` 的 Phase 2 代码

2. ✅ 实现核心算法
   - `computeMaxCollisionProbability()` - 单轨迹CP
   - `evaluateJointGMMPDF()` - GMM联合概率密度
   - `evaluateGaussianPDF()` - 高斯PDF
   - 蒙特卡洛采样逻辑

3. ✅ 单元测试
   - 测试无行人场景（CP = 0）
   - 测试直接碰撞场景（CP > 0.5）
   - 测试近距离场景

4. ✅ 集成到 RiskAwareCritic
   - 在 `score()` 方法中调用CP计算
   - 实现剪枝逻辑（CP > σ_prune）

**验证标准：**
- 单元测试全部通过
- 在仿真中能检测到行人接近时CP增大
- 高CP轨迹被正确剪枝

### Phase 3: CVaR风险评估（3-4天）

**任务清单：**

1. ✅ 创建 `cvar_evaluator.hpp/cpp`
2. ✅ 实现嵌套蒙特卡洛采样
3. ✅ 实现成本评估函数
4. ✅ 单元测试
5. ✅ 集成到 RiskAwareCritic

**验证标准：**
- CVaR值能反映尾部风险
- 对精英轨迹的评估准确
- 计算效率满足20Hz控制频率

---

## 📊 关键性能指标

### 目标指标（来自1.md）

| 指标 | 当前MPPI | RAPA-MPPI目标 | 验证方法 |
|------|----------|--------------|---------|
| **碰撞率** | ~15% | <2% | 运行100次测试 |
| **冻结问题** | 频繁发生 | 基本消除 | 长时间运行观察 |
| **任务完成时间** | 基准 | +10-15% | 记录到达目标时间 |
| **控制频率** | 20Hz | ≥20Hz | 监控/controller_server频率 |
| **计算延迟** | - | <50ms/cycle | 测量score()耗时 |

### 监控工具

**实时性能监控：**
```bash
# 控制频率
ros2 topic hz /cmd_vel

# 计算延迟（需要在代码中添加计时）
ros2 topic echo /diagnostics --field status[0].values

# 风险评估统计
ros2 topic echo /rosout | grep "RiskAwareCritic" | grep "AvgCP\|AvgCVaR"
```

---

## 🧪 测试策略

### 渐进式测试方案

#### Level 1: 静态验证（Phase 1）
- ✅ 编译通过
- ✅ 插件加载
- ✅ 参数读取正确
- ✅ 预测数据订阅成功

#### Level 2: 功能验证（Phase 2-3）
- ⏳ CP计算正确性
- ⏳ CVaR评估准确性
- ⏳ 风险成本正确注入

#### Level 3: 集成测试（Phase 4）
- ⏳ 单行人场景：避障成功
- ⏳ 双行人交替：无冻结
- ⏳ 高密度场景：保持安全

#### Level 4: 性能测试（Phase 5）
- ⏳ 计算频率达标（≥20Hz）
- ⏳ 参数调优
- ⏳ 长时间稳定性

### 自动化测试脚本

```bash
# 快速功能测试（5分钟）
cd ~/ackermann_sim
./test_rapa_mppi_quick.sh

# 完整场景测试（30分钟）
./test_rapa_mppi_full.sh

# 性能压力测试（1小时）
./test_rapa_mppi_stress.sh
```

（脚本模板见 `RAPA_MPPI_IMPLEMENTATION_PLAN.md`）

---

## 🎯 里程碑时间表

| 里程碑 | 任务 | 预估时间 | 状态 |
|--------|------|---------|------|
| **M1** | Phase 1完成，框架可编译运行 | 2天 | ⏳ 进行中 |
| **M2** | Phase 2完成，CP计算可用 | +3天 | 🔜 待开始 |
| **M3** | Phase 3完成，CVaR评估可用 | +3天 | 🔜 待开始 |
| **M4** | 基础集成，单场景测试通过 | +2天 | 🔜 待开始 |
| **M5** | 参数调优，多场景测试 | +3天 | 🔜 待开始 |
| **M6** | 完整RAPA-MPPI系统 | +3天 | 🔜 待开始 |
| **总计** | | **16天** | |

---

## 🐛 已知问题与解决方案

### 问题1: Gazebo Actor不发布位姿
**现状**: 已解决
**方案**: 使用Physics-based Models（TrajectoryFollower插件）

### 问题2: 插件形式无法加载行人
**原因**: Gazebo Actor是视觉实体，不是物理实体
**方案**: 见本文档和 `memory.md`、`stege.md` 分析

### 问题3: 如何简单修改行人数目和轨迹
**方案**:
1. 复制 `<model>` 块修改name和waypoints
2. 更新预测器的 `actor_names` 配置
3. 更新 ros_gz_bridge 配置

详见 `RAPA_MPPI_IMPLEMENTATION_PLAN.md` 的"如何简单修改行人数目和轨迹"章节。

---

## 💡 开发技巧

### 1. 快速调试循环

```bash
# 编译特定包
colcon build --packages-select saye_rapa_mppi && source install/setup.bash

# 编译并立即测试
colcon build --packages-select saye_rapa_mppi && \
source install/setup.bash && \
ros2 launch saye_mppi mppi_experiment.launch.py \
  controller_params:=$(ros2 pkg prefix saye_rapa_mppi)/share/saye_rapa_mppi/config/rapa_mppi_params.yaml
```

### 2. 日志级别控制

```bash
# 启动时设置日志级别
ros2 launch saye_mppi mppi_experiment.launch.py \
  --ros-args --log-level controller_server:=debug

# 运行时修改
ros2 service call /controller_server/set_logger_level \
  rcl_interfaces/srv/SetLoggerLevels \
  "{logger_name: 'controller_server', level: 'DEBUG'}"
```

### 3. 参数实时调整

```bash
# 安装rqt_reconfigure
sudo apt install ros-jazzy-rqt-reconfigure

# 运行
ros2 run rqt_reconfigure rqt_reconfigure
```

### 4. 性能剖析

```bash
# 使用perf工具
perf record -g ros2 run controller_server controller_server
perf report
```

---

## 📞 获取帮助

### 文档资源
1. **Nav2 MPPI文档**: https://navigation.ros.org/configuration/packages/configuring-mppic.html
2. **DRA-MPPI论文**: 参考 `1.md` 中的引用
3. **Gazebo Harmonic文档**: https://gazebosim.org/docs/harmonic

### 项目文档
- 主要问题看 `memory.md`
- 理论原理看 `1.md`
- 实施细节看 `RAPA_MPPI_IMPLEMENTATION_PLAN.md`
- 快速测试看 `TEST_NOW.md`

### 调试检查清单

**编译错误：**
- [ ] 所有依赖包已安装
- [ ] CMakeLists.txt中的依赖声明正确
- [ ] package.xml中的依赖完整

**运行时错误：**
- [ ] `source install/setup.bash` 已执行
- [ ] 插件在 `plugins.xml` 中正确导出
- [ ] 参数文件路径正确
- [ ] 预测数据话题有发布者

**功能问题：**
- [ ] 检查日志输出
- [ ] 验证数据流（ros2 topic echo）
- [ ] 检查参数值（ros2 param get）
- [ ] 使用RViz可视化

---

## ✅ Phase 1 完成标志

**当你完成以下所有检查后，Phase 1即算完成：**

- [x] `setup_phase1.sh` 脚本运行成功
- [ ] 编译无错误无警告
- [ ] `ros2 plugin list` 能看到 RiskAwareCritic
- [ ] 启动MPPI时日志显示 "RiskAwareCritic initialized"
- [ ] 能订阅到 `/pedestrian_predictions` 话题
- [ ] 日志中每隔一段时间显示 "Received predictions for X pedestrians"
- [ ] 车辆能正常运动（即使critic暂未添加成本）

**完成后：**
1. ✅ 提交代码（如果使用git）
2. 📝 记录Phase 1完成时间
3. 🚀 开始Phase 2实现（CP计算模块）

---

**祝开发顺利！🎉**

**有问题随时查阅 `RAPA_MPPI_IMPLEMENTATION_PLAN.md` 获取详细代码示例和调试建议。**
