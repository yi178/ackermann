# Actor Pose Test Workflow

本文记录了使用 `saye_dynamic_corridor_test.sdf` 测试世界获取 Gazebo 行人实时位姿的完整流程，适用于后续 GNN 预测、风险评估等模块的输入调试。

## 1. 准备条件

- 工作区已构建并安装最新的 `saye_description` 与 `saye_bringup_my` 包：
  ```bash
  colcon build --packages-select saye_description saye_bringup_my
  ```
- 确认 `saye_dynamic_corridor_test.sdf` 和两个插件已安装到 `install/`：
  - `install/saye_description/share/saye_description/worlds/saye_dynamic_corridor_test.sdf`
  - `install/saye_description/lib/libactor_pose_publisher_system.so`
  - `install/saye_description/lib/libtest_actor_driver_system.so`

## 2. 启动仿真

建议每次运行前先清理旧的 Gazebo / bridge 进程，以免 `/clock`、`/stats` 被抢占：

```bash
pkill -f "gz sim"
pkill -f g-transport-topic
pkill -f ros_gz_bridge
```

然后在终端 A 中执行：

```bash
source ~/ackermann_sim/install/setup.bash
ros2 launch saye_bringup_my actor_pose_test.launch.py \
  world:=saye_dynamic_corridor_test.sdf enable_bridge:=false
```

> 说明：launch 文件会以 `-r` 方式运行 Gazebo，世界中已经加载 `ActorPosePublisherSystem`（负责发布 `/actor_pose/*`）以及 `TestActorDriverSystem`（让行人按预设路径运动）。

## 3. 验证 Gazebo 话题

在终端 B 中查看 Gazebo 侧是否发布了行人位姿：

```bash
source ~/ackermann_sim/install/setup.bash
gz topic -e -t /actor_pose/walker_center -n 10
```

输出中 `position.x/y` 与 `orientation` 应持续变化，表示行人沿修改后的路点运动。

## 4. 按需开启 ROS 桥接

如需在 ROS 侧消费位姿数据，在终端 C 启动单向桥接（GZ → ROS）：

```bash
source ~/ackermann_sim/install/setup.bash
ros2 run ros_gz_bridge parameter_bridge \
  '/actor_pose/walker_center@geometry_msgs/msg/Pose[gz.msgs.Pose'
```

随后在终端 D 监听：

```bash
source ~/ackermann_sim/install/setup.bash
ros2 topic echo /actor_pose/walker_center
```

如需桥接多个行人，可为每个行人追加一条同格式参数，例如 `'/actor_pose/walker_east@geometry_msgs/msg/Pose[gz.msgs.Pose'`。

## 5. 调整行人轨迹

测试世界文件存放在 `src/ackermann-vehicle-gzsim-ros2/saye_description/worlds/saye_dynamic_corridor_test.sdf`。每个 `<actor>` 使用 `TestActorDriverSystem` 插件，在 `<plugin>` 块内的 `<waypoints>` 定义路点：

- `<time>`：到达该航点的仿真时间（秒），必须严格递增。
- `<pose>`：相对于行人初始位姿的 `x y z roll pitch yaw`；若需掉头或停顿，可插入只改变 yaw 或时间的航点。
- `<loop>`：决定是否循环播放；设为 `false` 时会在最后一个航点保持不动。

修改完成后，记得同步到安装目录或重新构建：

```bash
cp src/ackermann-vehicle-gzsim-ros2/saye_description/worlds/saye_dynamic_corridor_test.sdf \
   install/saye_description/share/saye_description/worlds/
# 或者重新 colcon build
```

按照以上流程，即可稳定获取 Gazebo 行人位姿并将其桥接到 ROS，用于后续 GNN 预测与 MPPI 风险控制测试。*** End Patch
