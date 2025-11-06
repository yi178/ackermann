# Prediction for a single pedestrian
# 单个行人的预测

string id                           # Unique identifier (e.g., "walker_center")
                                    # 唯一标识符（例如："walker_center"）

geometry_msgs/Pose current_pose     # Current observed pose
                                    # 当前观测位姿

float64 current_speed               # Estimated current speed [m/s]
                                    # 估计的当前速度 [m/s]

# GMM representation of future trajectory
# 未来轨迹的GMM表示
# For CV model: K=1 (single mode)
# For IMM model: K=3 (CV, CA, CT modes)
# CV模型：K=1（单模态）
# IMM模型：K=3（CV、CA、CT模式）
GMMComponent[] gmm_modes
