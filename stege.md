## gnn预测模块
好的，我来给出完整的执行指令、文件说明和 Gazebo
  Actor 失败原因的调研总结。

  1. 执行指令

  启动完整预测系统

  # 终端 1: 启动 Gazebo 仿真（带移动障碍物世界）
  cd ~/ackermann_sim
  source install/setup.bash
  ros2 launch saye_bringup_my saye_spawn.launch.py
  world:=saye_moving_obstacles.sdf

  # 终端 2: 启动 GMM 预测器
  cd ~/ackermann_sim
  source install/setup.bash
  ros2 run saye_prediction ground_truth_predictor

  # 终端 3 (可选): 查看预测话题
  ros2 topic echo /pedestrian_predictions

  # 终端 4 (可选): 查看发布频率
  ros2 topic hz /pedestrian_predictions

  # 终端 5 (可选): 启动可视化器 (RViz)
  ros2 run saye_prediction prediction_visualizer

  测试单个 walker 的预测输出

  # 查看简化输出（只显示 ID 和速度）
  ros2 topic echo /pedestrian_predictions --once |
  grep -E "(id:|current_speed:)"

  验证预测准确性

  # 运行预测验证器（计算 ADE/FDE 误差）
  ros2 run saye_prediction prediction_verifier

  2. 关键文件作用说明

  世界文件

  saye_description/worlds/saye_moving_obstacles.sdf
  - 作用: 定义包含移动障碍物的仿真世界
  - 内容:
    - 2 个移动球体模型 (walker_center, walker_east)
    - 使用 TrajectoryFollower 插件实现循环运动
    - 添加了 PosePublisher 插件（虽然最终未使用）
    - 走廊墙壁和地面
  - 关键参数:
  walker_center: 初始位置 (8, -3, 0.5)，轨迹 Y: -3 ↔
  3
  walker_east:   初始位置 (8, 3, 0.5)，轨迹 Y: 3 ↔ -3
  Z = 0.5 但重力作用后稳定在 Z ≈ 0.3

  预测器核心代码

  saye_prediction/saye_prediction/ground_truth_predict
  or.py
  - 作用: GMM 预测器主节点，实现 Constant Velocity
  模型
  - 订阅: /world/moving_obstacles_world/pose/info
  (TFMessage)
  - 发布: /pedestrian_predictions
  (PedestrianPredictions)
  - 算法:
    a. 通过 Z≈0.3m 过滤识别 walkers
    b. Transform index 建立持久映射
    c. 历史位置队列估计速度 (线性回归)
    d. CV 模型预测: x(t) = x₀ + vₓ·t
    e. 协方差增长: Σ(t) = Σ₀ + Q·t²
  - 关键函数:
    - pose_stream_callback(): 识别和追踪 walkers
    - estimate_velocity(): 速度估计
    - predict_trajectory_cv(): CV 模型预测
    - publish_predictions(): 定时发布

  saye_prediction/saye_prediction/prediction_visualize
  r.py
  - 作用: 在 RViz 中可视化预测结果
  - 订阅: /pedestrian_predictions
  - 发布: /pedestrian_prediction_markers (MarkerArray)
  - 可视化元素:
    - 红色球体: 当前位置
    - 蓝色线条: 预测轨迹 (2.8秒)
    - 绿色椭圆: 1.4秒时刻的不确定性
    - 黄色箭头: 速度向量

  saye_prediction/saye_prediction/prediction_verifier.
  py
  - 作用: 离线评估预测准确性
  - 指标:
    - ADE (Average Displacement Error): 平均位移误差
    - FDE (Final Displacement Error): 终点位移误差
  - 方法: 对比预测轨迹与实际观测轨迹

  辅助脚本（最终未使用）

  saye_prediction/scripts/world_pose_bridge.py
  - 原始目的: 从 world pose/info 提取 walker
  数据并单独发布
  - 状态: 已创建但未使用（直接在 predictor 中处理）
  - 问题: TFMessage 中 child_frame_id 为空，无法识别
  walker

  saye_prediction/scripts/gazebo_model_pose_publisher.
  py
  - 原始目的: 使用 Gazebo state 服务查询并发布 walker
  位姿
  - 状态: 创建但失败
  - 失败原因: GetEntityState 服务在 ROS 2 Jazzy
  中不存在

  saye_prediction/scripts/gazebo_actor_pose_publisher.
  py
  - 原始目的: 为 Gazebo Actor 查询并发布位姿
  - 状态: 与 gazebo_model_pose_publisher.py 相同问题

  消息定义

  saye_msgs/msg/GMMComponent.msg
  float64 weight              # GMM 权重 (CV模型=1.0)
  float64[] mean_x            # X 位置均值序列 [55步]
  float64[] mean_y            # Y 位置均值序列
  float64[] cov_xx            # X 方差序列
  float64[] cov_yy            # Y 方差序列
  float64[] cov_xy            # XY 协方差序列

  saye_msgs/msg/PedestrianPrediction.msg
  string id                   # 行人 ID
  geometry_msgs/Pose current_pose
  float64 current_speed
  GMMComponent[] gmm_modes    # GMM 分量数组 (CV=1个)

  saye_msgs/msg/PedestrianPredictions.msg
  std_msgs/Header header
  PedestrianPrediction[] predictions  # 所有行人预测

  配置文件

  saye_prediction/config/ground_truth_predictor.yaml
  ground_truth_predictor:
    ros__parameters:
      prediction_horizon: 2.8    # 预测时域 (秒)
      prediction_dt: 0.05        # 时间步长
      actor_names: ['walker_center', 'walker_east']
      history_size: 5            # 速度估计历史窗口
      sigma_0: 0.1              # 初始不确定性 (m)
      sigma_q: 0.5              # 过程噪声强度 (m/s)
      publish_rate: 10.0        # 发布频率 (Hz)

  saye_bringup_my/config/ros_gz_bridge.yaml (关键部分)
  # 新增：移动障碍物世界的 pose/info 桥接
  - ros_topic_name:
  "/world/moving_obstacles_world/pose/info"
    gz_topic_name:
  "/world/moving_obstacles_world/pose/info"
    ros_type_name: "tf2_msgs/msg/TFMessage"
    gz_type_name: "gz.msgs.Pose_V"
    direction: GZ_TO_ROS

  3. Gazebo Actor 失败原因调研

  尝试历程

  方案 1: 使用 Gazebo Actor（失败）

  文件:
  saye_description/worlds/saye_dynamic_corridor.sdf

  配置:
  <actor name="walker_center">
    <skin>
      <filename>walk.dae</filename>
    </skin>
    <script>
      <loop>true</loop>
      <trajectory id="0" type="walking">
        <waypoint>
          <time>0</time>
          <pose>8 -3 1 0 0 0</pose>
        </waypoint>
        <waypoint>
          <time>5</time>
          <pose>8 3 1 0 0 0</pose>
        </waypoint>
      </trajectory>
    </script>
  </actor>

  问题:
  1. ✅ Actor 加载成功 - 在 Gazebo GUI 中可见
  2. ✅ 轨迹运动正常 - Actor 按预期移动
  3. ❌ 不发布 pose 话题 - /model/walker_center/pose
  超时
  4. ❌ 不出现在 world pose/info - TFMessage 中无
  Actor 数据

  根本原因:
  Gazebo Harmonic 的 Actor 是视觉实体，不是物理实体
  - Actor 只用于动画展示，没有物理碰撞
  - Actor 不参与 SceneBroadcaster 的 pose 发布
  - Actor 不会被 PosePublisher 插件识别

  方案 2: 为 Actor 添加 PosePublisher 插件（失败）

  尝试:
  <actor name="walker_center">
    <!-- ... -->
    <plugin filename="gz-sim-pose-publisher-system"
            name="gz::sim::systems::PosePublisher">
      <publish_model_pose>true</publish_model_pose>
      <update_frequency>10</update_frequency>
    </plugin>
  </actor>

  错误信息:
  [Err] [PosePublisher.cc:192] PosePublisher plugin
  should be attached to a model entity. Failed to
  initialize.

  原因: PosePublisher 只能附加到 <model> 实体，不支持
  <actor>

  方案 3: 下载官方 walking_actor 模型（失败）

  操作:
  # 从 Gazebo Fuel 下载官方 actor 模型
  wget https://fuel.gazebosim.org/1.0/OpenRobotics/mod
  els/walking_actor/tip/files/walk.dae
  # 存放在 ~/.gz/models/walking_actor/

  结果: 即使使用官方模型，Actor 仍不发布 pose 数据

  方案 4: 使用 GetEntityState 服务查询（失败）

  代码: gazebo_actor_pose_publisher.py
  from ros_gz_interfaces.srv import GetEntityState
  self.state_client = self.create_client(
      GetEntityState,
      f'/world/{self.world_name}/state'
  )

  错误:
  ImportError: cannot import name 'GetEntityState'
  from 'ros_gz_interfaces.srv'

  原因: ROS 2 Jazzy 的 ros_gz_interfaces 包中不存在
  GetEntityState 服务

  确认:
  ros2 interface list | grep -i entity
  # 输出为空

  方案 5: 直接使用 Gazebo Transport 订阅（失败）

  尝试: 使用 Gazebo 原生 topic
  gz topic -l | grep walker
  # 输出: /model/walker_center/pose (topic 存在)

  gz topic -e -t /model/walker_center/pose -n 1
  # 超时，无数据

  原因: Actor 的 topic 虽然被创建，但从不发布数据

  最终解决方案: 使用 Physics-based Models

  方案: 用带 TrajectoryFollower 插件的普通 <model>
  替代 Actor

  文件: saye_moving_obstacles.sdf

  配置:
  <model name="walker_center">
    <link name="link">
      <inertial><mass>70.0</mass></inertial>
      <collision>
        <geometry><sphere><radius>0.3</radius></sphere
  ></geometry>
      </collision>
      <visual>
        <geometry><sphere><radius>0.3</radius></sphere
  ></geometry>
        <material>
          <ambient>1 0 0 1</ambient>  <!-- 红色 -->
        </material>
      </visual>
    </link>

    <plugin
  filename="gz-sim-trajectory-follower-system"

  name="gz::sim::systems::TrajectoryFollower">
      <link_name>link</link_name>
      <loop>true</loop>
      <waypoints>
        <waypoint>0 -3</waypoint>
        <waypoint>0 3</waypoint>
      </waypoints>
    </plugin>
  </model>

  优势:
  1. ✅ 自动出现在 world pose/info - 无需额外插件
  2. ✅ 通过 ros_gz_bridge 自动桥接到 ROS 2
  3. ✅ 有物理碰撞 - 可与车辆交互
  4. ✅ 可通过 Z 坐标过滤识别 (Z≈0.3m)

  数据流:
  Gazebo TrajectoryFollower
      ↓
  Gazebo SceneBroadcaster
      ↓
  /world/moving_obstacles_world/pose/info
  (gz.msgs.Pose_V)
      ↓
  ros_gz_bridge
      ↓
  /world/moving_obstacles_world/pose/info
  (tf2_msgs/TFMessage)
      ↓
  ground_truth_predictor.py

  核心技术要点

  识别策略 (因为 TFMessage 的 child_frame_id 为空):
  1. Z 坐标过滤: 0.25 < z < 0.35 识别 walkers
  2. Transform Index 映射: 首次检测时建立 {index:
  actor_name} 映射
  3. 持久追踪: 后续根据 index 直接获取对应 actor

  代码片段:
  for idx, transform in enumerate(msg.transforms):
      z = transform.transform.translation.z

      # 过滤 walkers
      if not (0.25 < z < 0.35):
          continue

      # 检查是否已映射
      if idx in self.walker_index_map:
          actor_name = self.walker_index_map[idx]
      else:
          # 新 walker，分配给下一个未追踪的 actor
          actor_name = needed_actors.pop(0)
          self.walker_index_map[idx] = actor_name

  对比总结

  | 特性       | Gazebo Actor | Physics Model +
  TrajectoryFollower |
  |----------|--------------|-------------------------
  -----------|
  | 视觉效果     | ✅ 真实人形模型     | ⚠️ 简单球体
                            |
  | 位姿发布     | ❌ 不发布        | ✅ 自动发布
                          |
  | 物理碰撞     | ❌ 无碰撞        | ✅ 有碰撞
                         |
  | ROS 2 集成 | ❌ 需复杂方案      | ✅ 原生支持
                          |
  | 适用场景     | 视觉展示         | 仿真测试
                         |
  | 推荐用于预测   | ❌            | ✅
                     |

  这就是为什么最终选择 Physics-based Models 而非
  Gazebo Actors 的完整原因。


## 进展1

当前使用gazebo输出行人位姿
ros2 launch saye_bringup_my actor_pose_test.launch.py \
     world:=saye_dynamic_corridor_test.sdf


 ros2 run ros_gz_bridge parameter_bridge \
      '/actor_pose/walker_center@geometry_msgs/msg/Pose[gz.msgs.Pose' \
      '/actor_pose/walker_east@geometry_msgs/msg/Pose[gz.msgs.Pose' \
      --ros-args --param lazy:=false \
                 --param qos.reliability:=reliable \
                 --param qos.history:=keep_last \
                 --param qos.depth:=10


 ros2 topic echo /actor_pose/walker_center --once \
      --qos-history keep_last --qos-depth 10 --qos-reliability reliable
