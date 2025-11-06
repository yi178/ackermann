# Gaussian Mixture Model Component
# Represents one mode in a multi-modal prediction
# 高斯混合模型的一个分量

float64 weight              # Weight of this mode, sum of all weights = 1.0
                            # 该模式的权重，所有权重之和为1.0

# Predicted mean trajectory (56 time steps, dt=0.05s, total 2.8s)
# 预测的均值轨迹（56个时间步，dt=0.05秒，总共2.8秒）
float64[] mean_x            # X position means [m]
float64[] mean_y            # Y position means [m]

# Predicted covariance (uncertainty grows with time)
# 预测的协方差（不确定性随时间增长）
float64[] cov_xx            # Variance in X direction [m²]
float64[] cov_yy            # Variance in Y direction [m²]
float64[] cov_xy            # Covariance between X and Y [m²]
