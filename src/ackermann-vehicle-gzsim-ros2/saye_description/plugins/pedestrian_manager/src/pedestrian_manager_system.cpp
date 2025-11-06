#include "saye_description/pedestrian_manager_system.hpp"

#include <algorithm>
#include <array>
#include <filesystem>
#include <sstream>
#include <string>
#include <unordered_set>

#include <ament_index_cpp/get_package_share_directory.hpp>
#include <gz/common/Filesystem.hh>
#include <gz/common/StringUtils.hh>
#include <gz/plugin/Register.hh>
#include <gz/msgs/pose.pb.h>
#include <gz/msgs/Utility.hh>
#include <gz/sim/EntityComponentManager.hh>
#include <gz/sim/EventManager.hh>
#include <gz/sim/SdfEntityCreator.hh>
#include <gz/sim/Util.hh>
#include <gz/sim/World.hh>
#include <gz/sim/components/Actor.hh>
#include <gz/sim/components/Name.hh>
#include <gz/sim/components/Pose.hh>
#include <sdf/Actor.hh>
#include <sdf/sdf.hh>
#include <yaml-cpp/yaml.h>

namespace
{
constexpr const char *kDefaultSkinUri = "model://walking_actor/meshes/walk.dae";
constexpr const char *kDefaultAnimation = "walking";
}  // namespace

namespace saye_description
{

using gz::sim::Entity;
using gz::sim::EntityComponentManager;
using gz::sim::SdfEntityCreator;
using gz::sim::World;
using gz::sim::UpdateInfo;

namespace
{
std::array<double, 6> ReadPoseArray(const YAML::Node &node)
{
  std::array<double, 6> values{};
  if (!node || !node.IsSequence() || node.size() != 6)
  {
    throw std::runtime_error("Pose array must contain 6 numeric elements");
  }
  for (std::size_t i = 0; i < 6; ++i)
  {
    values[i] = node[i].as<double>();
  }
  return values;
}
}  // namespace

void PedestrianManagerSystem::Configure(const Entity &entity,
                                        const std::shared_ptr<const sdf::Element> &sdf,
                                        EntityComponentManager &,
                                        gz::sim::EventManager &eventMgr)
{
  this->world_entity_ = entity;
  this->event_mgr_ = &eventMgr;

  std::string config_file;
  if (sdf && sdf->HasElement("config_file"))
  {
    config_file = sdf->Get<std::string>("config_file");
  }

  if (config_file.empty())
  {
    gzerr << "pedestrian_manager_system: <config_file> parameter is required." << std::endl;
    return;
  }

  std::filesystem::path resolved_path(config_file);
  if (!resolved_path.is_absolute())
  {
    try
    {
      auto share_dir = ament_index_cpp::get_package_share_directory("saye_description");
      resolved_path = std::filesystem::path(share_dir) / resolved_path;
    }
    catch (const std::exception &ex)
    {
      gzerr << "pedestrian_manager_system: Failed to locate package share directory: "
            << ex.what() << std::endl;
      return;
    }
  }

  try
  {
    if (!this->LoadConfig(resolved_path.string()))
    {
      gzmsg << "pedestrian_manager_system: No pedestrians loaded from " << resolved_path << std::endl;
    }
    else
    {
      gzmsg << "pedestrian_manager_system: Loaded " << this->specs_.size()
            << " pedestrians from " << resolved_path << std::endl;
    }
  }
  catch (const std::exception &ex)
  {
    gzerr << "pedestrian_manager_system: Failed to parse config file: " << ex.what() << std::endl;
    this->specs_.clear();
  }
}

void PedestrianManagerSystem::PreUpdate(const UpdateInfo &info,
                                        EntityComponentManager &ecm)
{
  if (info.paused || this->actors_spawned_ || this->specs_.empty())
  {
    return;
  }

  if (!this->event_mgr_)
  {
    gzerr << "pedestrian_manager_system: Event manager not available; cannot spawn actors." << std::endl;
    return;
  }

  World world(this->world_entity_);
  auto nameOpt = world.Name(ecm);
  const std::string worldName = nameOpt.value_or(std::string("<unknown>"));
  gzmsg << "pedestrian_manager_system: Spawning pedestrians in world " << worldName << std::endl;

  SdfEntityCreator creator(ecm, *this->event_mgr_);

  for (const auto &spec : this->specs_)
  {
    const std::string sdfText = this->BuildActorSdf(spec);
    sdf::Root root;
    sdf::Errors errors = root.LoadSdfString(sdfText);
    if (!errors.empty())
    {
      gzerr << "pedestrian_manager_system: Failed to parse actor SDF for " << spec.name << std::endl;
      continue;
    }
    const sdf::Actor *actor = root.Actor();
    if (!actor)
    {
      gzerr << "pedestrian_manager_system: Parsed SDF has no actor element for " << spec.name << std::endl;
      continue;
    }
    creator.CreateEntities(actor);
    gzmsg << "pedestrian_manager_system: Spawned actor " << spec.name << std::endl;
  }

  this->actors_spawned_ = true;
  this->RefreshActorEntities(ecm);
}

bool PedestrianManagerSystem::LoadConfig(const std::string &config_path)
{
  this->specs_.clear();

  YAML::Node root = YAML::LoadFile(config_path);
  if (!root || !root["pedestrians"])
  {
    return false;
  }

  const auto list = root["pedestrians"];
  if (!list.IsSequence())
  {
    throw std::runtime_error("'pedestrians' must be a sequence");
  }

  for (const auto &entry : list)
  {
    PedestrianSpec spec;
    spec.name = entry["name"].as<std::string>();
    spec.skin_uri = entry["skin"].as<std::string>(kDefaultSkinUri);
    spec.animation = entry["animation"].as<std::string>(kDefaultAnimation);
    spec.loop = entry["loop"].as<bool>(true);
    spec.auto_start = entry["auto_start"].as<bool>(true);
    spec.spawn_pose = ReadPoseArray(entry["spawn_pose"]);

    const auto traj = entry["trajectory"];
    if (!traj || !traj.IsSequence() || traj.size() < 2)
    {
      throw std::runtime_error("trajectory must be a sequence with at least two waypoints");
    }

    for (const auto &wpNode : traj)
    {
      TrajectoryPoint wp;
      wp.time = wpNode["time"].as<double>();
      wp.pose = ReadPoseArray(wpNode["pose"]);
      spec.trajectory.emplace_back(std::move(wp));
    }

    std::sort(spec.trajectory.begin(), spec.trajectory.end(),
              [](const auto &a, const auto &b) { return a.time < b.time; });

    this->specs_.push_back(std::move(spec));
  }

  return !this->specs_.empty();
}

void PedestrianManagerSystem::PostUpdate(const UpdateInfo &info,
                                         const EntityComponentManager &ecm)
{
  if (!this->actors_spawned_ || this->specs_.empty())
  {
    return;
  }

  if (this->actor_entities_.size() < this->specs_.size())
  {
    this->RefreshActorEntities(ecm);
  }

  for (const auto &spec : this->specs_)
  {
    auto it = this->actor_entities_.find(spec.name);
    if (it == this->actor_entities_.end())
    {
      continue;
    }

    const Entity entity = it->second;
    const auto *poseComp = ecm.Component<gz::sim::components::Pose>(entity);
    if (!poseComp)
    {
      continue;
    }

    const auto &pose = poseComp->Data();

    gz::msgs::Pose msg;
    msg.set_name(spec.name);
    gz::msgs::Set(msg.mutable_position(), pose.Pos());
    gz::msgs::Set(msg.mutable_orientation(), pose.Rot());
    gz::msgs::Set(msg.mutable_header()->mutable_stamp(), info.simTime);

    const std::string topic = "/model/" + spec.name + "/pose";
    auto pubIt = this->publishers_.find(spec.name);
    if (pubIt == this->publishers_.end())
    {
      auto publisher = this->node_.Advertise<gz::msgs::Pose>(topic);
      if (!publisher)
      {
        gzerr << "pedestrian_manager_system: Failed to advertise pose topic " << topic << std::endl;
        continue;
      }
      pubIt = this->publishers_.emplace(spec.name, std::move(publisher)).first;
    }

    pubIt->second.Publish(msg);
  }
}

void PedestrianManagerSystem::RefreshActorEntities(const EntityComponentManager &ecm)
{
  if (this->specs_.empty())
  {
    return;
  }

  std::unordered_set<std::string> expected;
  expected.reserve(this->specs_.size());
  for (const auto &spec : this->specs_)
  {
    expected.insert(spec.name);
  }

  ecm.Each<gz::sim::components::Actor, gz::sim::components::Name>(
      [&](const Entity &entity,
          const gz::sim::components::Actor *,
          const gz::sim::components::Name *nameComp) -> bool
      {
        if (!nameComp)
        {
          return true;
        }

        const std::string &name = nameComp->Data();
        if (expected.count(name) == 0)
        {
          return true;
        }

        if (this->actor_entities_.count(name) == 0)
        {
          this->actor_entities_[name] = entity;
        }
        return true;
      });
}

std::string PedestrianManagerSystem::BuildActorSdf(const PedestrianSpec &spec) const
{
  std::ostringstream oss;
  oss << "<sdf version=\"1.8\">\n";
  oss << "  <actor name=\"" << spec.name << "\">\n";
  oss << "    <pose>" << spec.spawn_pose[0] << ' ' << spec.spawn_pose[1] << ' ' << spec.spawn_pose[2]
      << ' ' << spec.spawn_pose[3] << ' ' << spec.spawn_pose[4] << ' ' << spec.spawn_pose[5] << "</pose>\n";
  oss << "    <skin>\n";
  oss << "      <filename>" << spec.skin_uri << "</filename>\n";
  oss << "      <scale>1.0</scale>\n";
  oss << "    </skin>\n";
  oss << "    <animation name=\"" << spec.animation << "\">\n";
  oss << "      <filename>" << spec.skin_uri << "</filename>\n";
  oss << "      <interpolate_x>true</interpolate_x>\n";
  oss << "    </animation>\n";
  oss << "    <script>\n";
  oss << "      <loop>" << (spec.loop ? "true" : "false") << "</loop>\n";
  oss << "      <auto_start>" << (spec.auto_start ? "true" : "false") << "</auto_start>\n";
  oss << "      <trajectory id=\"0\" type=\"" << spec.animation << "\" tension=\"0.0\">\n";
  for (const auto &wp : spec.trajectory)
  {
    oss << "        <waypoint>\n";
    oss << "          <time>" << wp.time << "</time>\n";
    oss << "          <pose>" << wp.pose[0] << ' ' << wp.pose[1] << ' ' << wp.pose[2]
        << ' ' << wp.pose[3] << ' ' << wp.pose[4] << ' ' << wp.pose[5] << "</pose>\n";
    oss << "        </waypoint>\n";
  }
  oss << "      </trajectory>\n";
  oss << "    </script>\n";
  oss << "  </actor>\n";
  oss << "</sdf>\n";
  return oss.str();
}

}  // namespace saye_description

namespace saye
{
using PedestrianManagerSystem = saye_description::PedestrianManagerSystem;
}  // namespace saye

GZ_ADD_PLUGIN(saye_description::PedestrianManagerSystem,
              gz::sim::System,
              saye_description::PedestrianManagerSystem::ISystemConfigure,
              saye_description::PedestrianManagerSystem::ISystemPreUpdate,
              saye_description::PedestrianManagerSystem::ISystemPostUpdate)

GZ_ADD_PLUGIN_ALIAS(saye_description::PedestrianManagerSystem,
                    "saye::PedestrianManagerSystem")
