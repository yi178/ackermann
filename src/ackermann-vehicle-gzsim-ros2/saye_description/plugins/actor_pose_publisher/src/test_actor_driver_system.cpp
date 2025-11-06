#include "saye_description/test_actor_driver_system.hpp"

#include <gz/math/Quaternion.hh>
#include <gz/plugin/Register.hh>
#include <gz/sim/EntityComponentManager.hh>
#include <gz/sim/components/Actor.hh>
#include <gz/sim/components/Name.hh>
#include <gz/sim/components/Pose.hh>

#include <algorithm>
#include <chrono>
#include <cmath>

namespace saye_description
{

namespace
{
constexpr double kEpsilon = 1e-6;
}

void TestActorDriverSystem::Configure(const gz::sim::Entity &entity,
                                      const std::shared_ptr<const sdf::Element> &sdf,
                                      gz::sim::EntityComponentManager &ecm,
                                      gz::sim::EventManager &)
{
  this->world_entity_ = entity;

  if (sdf)
  {
    if (sdf->HasElement("actor_name"))
    {
      this->actor_name_ = sdf->Get<std::string>("actor_name");
    }
    if (sdf->HasElement("radius"))
    {
      this->radius_ = sdf->Get<double>("radius");
    }
    if (sdf->HasElement("speed"))
    {
      this->speed_ = sdf->Get<double>("speed");
    }
    if (sdf->HasElement("height"))
    {
      this->height_ = sdf->Get<double>("height");
    }

    if (sdf->HasElement("loop"))
    {
      this->loop_path_ = sdf->Get<bool>("loop");
    }

    if (sdf->HasElement("waypoints"))
    {
      auto waypointsClone = sdf->Clone();
      auto waypointsElem = waypointsClone->GetElement("waypoints");
      auto wpElem = waypointsElem->GetElement("waypoint");
      double previous_time = -1.0;
      while (wpElem)
      {
        double time = wpElem->Get<double>("time");
        if (time <= previous_time)
        {
          gzerr << "[" << this->actor_name_ << "] waypoint time "
                << time << " must be greater than " << previous_time << std::endl;
          return;
        }
        previous_time = time;
        Waypoint wp;
        wp.time = time;
        wp.pose = wpElem->Get<gz::math::Pose3d>("pose");
        this->waypoints_.push_back(wp);
        wpElem = wpElem->GetNextElement("waypoint");
      }

      if (this->waypoints_.size() >= 2)
      {
        this->use_waypoints_ = true;
        this->path_duration_ = this->waypoints_.back().time;
      }
      else if (!this->waypoints_.empty())
      {
        gzerr << "[" << this->actor_name_
              << "] requires at least two waypoints, reverting to circular mode."
              << std::endl;
        this->waypoints_.clear();
      }
    }
  }

  // 查找目标 Actor
  ecm.Each<gz::sim::components::Actor,
           gz::sim::components::Name>(
    [&](const gz::sim::Entity &entity,
        const gz::sim::components::Actor *,
        const gz::sim::components::Name *name) -> bool
    {
      if (name && name->Data() == this->actor_name_)
      {
        this->actor_entity_ = entity;
        this->has_traj_pose_ =
          (ecm.Component<gz::sim::components::TrajectoryPose>(entity) != nullptr);
        return false;
      }
      return true;
    });
}

void TestActorDriverSystem::PreUpdate(const gz::sim::UpdateInfo &info,
                                      gz::sim::EntityComponentManager &ecm)
{
  if (info.paused || this->actor_entity_ == gz::sim::kNullEntity)
  {
    return;
  }

  if (!this->initial_pose_)
  {
    if (auto poseComp = ecm.Component<gz::sim::components::Pose>(this->actor_entity_))
    {
      this->initial_pose_ = poseComp->Data();
    }
  }

  if (!this->has_traj_pose_)
  {
    ecm.CreateComponent(this->actor_entity_, gz::sim::components::TrajectoryPose());
    this->has_traj_pose_ = true;
  }

  auto poseComp = ecm.Component<gz::sim::components::Pose>(this->actor_entity_);
  auto trajPoseComp =
      ecm.Component<gz::sim::components::TrajectoryPose>(this->actor_entity_);

  if (!poseComp && !trajPoseComp)
  {
    return;
  }

  const double sim_time =
      std::chrono::duration<double>(info.simTime).count();

  if (this->use_waypoints_)
  {
    this->UpdateWaypoints(sim_time, ecm);
  }
  else
  {
    this->UpdateCircular(sim_time, ecm);
  }
}

void TestActorDriverSystem::UpdateCircular(
    double sim_time, gz::sim::EntityComponentManager &ecm)
{
  if (!this->initial_pose_)
  {
    return;
  }

  auto poseComp = ecm.Component<gz::sim::components::Pose>(this->actor_entity_);
  auto trajPoseComp =
      ecm.Component<gz::sim::components::TrajectoryPose>(this->actor_entity_);

  if (!poseComp && !trajPoseComp)
  {
    return;
  }

  const double angle = this->speed_ * sim_time;

  const double x = this->radius_ * std::cos(angle);
  const double y = this->radius_ * std::sin(angle);

  gz::math::Pose3d local_pose;
  local_pose.Pos().Set(x, y, this->height_);
  local_pose.Rot().SetFromEuler(0.0, 0.0, angle + M_PI_2);

  if (trajPoseComp)
  {
    *trajPoseComp = gz::sim::components::TrajectoryPose(local_pose);
    ecm.SetChanged(this->actor_entity_,
                   gz::sim::components::TrajectoryPose::typeId,
                   gz::sim::ComponentState::OneTimeChange);
  }

  if (poseComp)
  {
    gz::math::Pose3d world_pose = (*this->initial_pose_) * local_pose;
    *poseComp = gz::sim::components::Pose(world_pose);
    ecm.SetChanged(
        this->actor_entity_, gz::sim::components::Pose::typeId,
        gz::sim::ComponentState::OneTimeChange);
  }
}

void TestActorDriverSystem::UpdateWaypoints(
    double sim_time, gz::sim::EntityComponentManager &ecm)
{
  if (!this->initial_pose_ || this->waypoints_.size() < 2)
  {
    return;
  }

  double t = sim_time;
  if (this->loop_path_ && this->path_duration_ > 0.0)
  {
    t = std::fmod(sim_time, this->path_duration_);
  }
  else
  {
    t = std::clamp(sim_time, 0.0, this->path_duration_);
  }

  const Waypoint *prev = &this->waypoints_.front();
  const Waypoint *next = nullptr;

  if (t <= prev->time + kEpsilon)
  {
    next = prev;
  }
  else if (t >= this->path_duration_ - kEpsilon)
  {
    prev = &this->waypoints_.back();
    next = prev;
  }
  else
  {
    for (size_t i = 1; i < this->waypoints_.size(); ++i)
    {
      if (t <= this->waypoints_[i].time + kEpsilon)
      {
        prev = &this->waypoints_[i - 1];
        next = &this->waypoints_[i];
        break;
      }
    }
  }

  gz::math::Pose3d local_pose = prev->pose;

  if (next && next != prev && next->time > prev->time)
  {
    const double ratio =
        (t - prev->time) / (next->time - prev->time);

    const auto &p0 = prev->pose;
    const auto &p1 = next->pose;

    gz::math::Vector3d pos =
        p0.Pos() + (p1.Pos() - p0.Pos()) * ratio;
    gz::math::Quaterniond rot =
        gz::math::Quaterniond::Slerp(ratio, p0.Rot(), p1.Rot());

    local_pose.Set(pos, rot);
  }

  local_pose.Pos().Z(local_pose.Pos().Z() + this->height_);

  auto poseComp = ecm.Component<gz::sim::components::Pose>(this->actor_entity_);
  auto trajPoseComp =
      ecm.Component<gz::sim::components::TrajectoryPose>(this->actor_entity_);

  if (trajPoseComp)
  {
    *trajPoseComp = gz::sim::components::TrajectoryPose(local_pose);
    ecm.SetChanged(this->actor_entity_,
                   gz::sim::components::TrajectoryPose::typeId,
                   gz::sim::ComponentState::OneTimeChange);
  }

  if (poseComp)
  {
    gz::math::Pose3d world_pose =
        (*this->initial_pose_) * local_pose;

    *poseComp = gz::sim::components::Pose(world_pose);
    ecm.SetChanged(
        this->actor_entity_, gz::sim::components::Pose::typeId,
        gz::sim::ComponentState::OneTimeChange);
  }
}

}  // namespace saye_description

GZ_ADD_PLUGIN(saye_description::TestActorDriverSystem,
              gz::sim::System,
              saye_description::TestActorDriverSystem::ISystemConfigure,
              saye_description::TestActorDriverSystem::ISystemPreUpdate)

GZ_ADD_PLUGIN_ALIAS(saye_description::TestActorDriverSystem,
                    "saye::TestActorDriverSystem")
