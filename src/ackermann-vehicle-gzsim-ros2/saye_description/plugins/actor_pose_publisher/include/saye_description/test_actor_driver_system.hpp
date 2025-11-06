#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <gz/math/Pose3.hh>
#include <gz/sim/System.hh>

namespace saye_description
{

/// \brief 行人驱动器：支持圆形轨迹或自定义路点。
class TestActorDriverSystem final
  : public gz::sim::System,
    public gz::sim::ISystemConfigure,
    public gz::sim::ISystemPreUpdate
{
public:
  void Configure(const gz::sim::Entity &entity,
                 const std::shared_ptr<const sdf::Element> &sdf,
                 gz::sim::EntityComponentManager &ecm,
                 gz::sim::EventManager &eventMgr) override;

  void PreUpdate(const gz::sim::UpdateInfo &info,
                 gz::sim::EntityComponentManager &ecm) override;

private:
  struct Waypoint
  {
    double time;
    gz::math::Pose3d pose;
  };

  void UpdateCircular(double sim_time,
                      gz::sim::EntityComponentManager &ecm);
  void UpdateWaypoints(double sim_time,
                       gz::sim::EntityComponentManager &ecm);

  gz::sim::Entity world_entity_{gz::sim::kNullEntity};
  gz::sim::Entity actor_entity_{gz::sim::kNullEntity};

  std::string actor_name_{"test_actor"};

  // 圆轨迹参数（在未提供路点时生效）
  double radius_{2.0};
  double speed_{0.5};
  double height_{1.0};

  // 路点配置
  bool use_waypoints_{false};
  bool loop_path_{true};
  double path_duration_{0.0};
  std::vector<Waypoint> waypoints_;

  std::optional<gz::math::Pose3d> initial_pose_;
  bool has_traj_pose_{false};
};

}  // namespace saye_description
