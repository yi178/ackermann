#pragma once

#include <chrono>
#include <memory>
#include <string>
#include <unordered_map>

#include <gz/msgs/pose.pb.h>
#include <gz/msgs/pose_v.pb.h>
#include <gz/sim/System.hh>
#include <gz/transport/Node.hh>

namespace saye_description
{

/// \brief System 插件：收集 Gazebo 场景中的 Actor 位姿，并以 Pose 消息形式发布。
class ActorPosePublisherSystem final
  : public gz::sim::System,
    public gz::sim::ISystemConfigure,
    public gz::sim::ISystemPostUpdate
{
public:
  /// \brief 解析 SDF 参数。
  void Configure(const gz::sim::Entity &entity,
                 const std::shared_ptr<const sdf::Element> &sdf,
                 gz::sim::EntityComponentManager &ecm,
                 gz::sim::EventManager &eventMgr) override;

  /// \brief 在每次仿真后更新阶段发布 Actor 位姿。
  void PostUpdate(const gz::sim::UpdateInfo &info,
                  const gz::sim::EntityComponentManager &ecm) override;

private:
  /// \brief 获取（或创建）某个 Actor 的单独 Publisher。
  gz::transport::Node::Publisher &GetOrCreatePublisher(const std::string &actor_name);

  /// \brief 聚合所有 Actor 位姿的 Publisher。
  void EnsureAggregatePublisher();

  /// \brief 世界实体。
  gz::sim::Entity world_entity_{gz::sim::kNullEntity};

  /// \brief 发布主题前缀，默认 `/actor_pose`。
  std::string topic_prefix_{"/actor_pose"};

  /// \brief 发布频率（Hz），默认 20 Hz；值 <=0 表示每个仿真步都发布。
  double publish_rate_hz_{20.0};

  /// \brief 仿真时间的上一发布时间戳。
  std::chrono::steady_clock::duration last_publish_time_{0};

  /// \brief 是否已经完成初次发布。
  bool first_publish_{true};

  /// \brief 发布周期（仿真时间）。
  std::chrono::steady_clock::duration publish_period_{0};

  /// \brief Gazebo Transport 节点。
  gz::transport::Node node_;

  /// \brief 每个 Actor 的单独 Publisher。
  std::unordered_map<std::string, gz::transport::Node::Publisher> actor_publishers_;

  /// \brief 聚合 Pose_V Publisher。
  gz::transport::Node::Publisher aggregate_pub_;

  /// \brief 聚合主题名称。
  std::string aggregate_topic_;
};

}  // namespace saye_description

