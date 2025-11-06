#ifndef SAYE_DESCRIPTION_PEDESTRIAN_MANAGER_SYSTEM_HPP_
#define SAYE_DESCRIPTION_PEDESTRIAN_MANAGER_SYSTEM_HPP_

#include <array>
#include <optional>
#include <string>
#include <vector>
#include <unordered_map>

#include <gz/sim/System.hh>
#include <gz/transport/Node.hh>

namespace saye_description
{

struct TrajectoryPoint
{
  double time{0.0};
  std::array<double, 6> pose{};  // x y z roll pitch yaw
};

struct PedestrianSpec
{
  std::string name;
  std::string skin_uri;
  std::string animation;
  bool loop{true};
  bool auto_start{true};
  std::array<double, 6> spawn_pose{};
  std::vector<TrajectoryPoint> trajectory;
};

class PedestrianManagerSystem final : public gz::sim::System,
                                      public gz::sim::ISystemConfigure,
                                      public gz::sim::ISystemPreUpdate,
                                      public gz::sim::ISystemPostUpdate
{
public:
  void Configure(const gz::sim::Entity &entity,
                 const std::shared_ptr<const sdf::Element> &sdf,
                 gz::sim::EntityComponentManager &ecm,
                 gz::sim::EventManager &eventMgr) override;

  void PreUpdate(const gz::sim::UpdateInfo &info,
                 gz::sim::EntityComponentManager &ecm) override;

  void PostUpdate(const gz::sim::UpdateInfo &info,
                  const gz::sim::EntityComponentManager &ecm) override;

private:
  bool LoadConfig(const std::string &config_path);
  std::string BuildActorSdf(const PedestrianSpec &spec) const;
  void RefreshActorEntities(const gz::sim::EntityComponentManager &ecm);

  gz::sim::Entity world_entity_{gz::sim::kNullEntity};
  gz::sim::EventManager *event_mgr_{nullptr};
  bool actors_spawned_{false};
  std::vector<PedestrianSpec> specs_;
  std::unordered_map<std::string, gz::sim::Entity> actor_entities_;
  std::unordered_map<std::string, gz::transport::Node::Publisher> publishers_;
  gz::transport::Node node_;
};

}  // namespace saye_description

#endif  // SAYE_DESCRIPTION_PEDESTRIAN_MANAGER_SYSTEM_HPP_
