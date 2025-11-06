#include "saye_description/actor_pose_publisher_system.hpp"

#include <gz/math/Pose3.hh>
#include <gz/msgs/Utility.hh>
#include <gz/plugin/Register.hh>
#include <gz/sim/EntityComponentManager.hh>
#include <gz/sim/components/Actor.hh>
#include <gz/sim/components/Name.hh>
#include <gz/sim/components/Pose.hh>

#include <algorithm>
#include <iostream>

namespace saye_description
{

namespace
{
constexpr const char *kAggregateSuffix = "/poses";
}

//////////////////////////////////////////////////
void ActorPosePublisherSystem::Configure(const gz::sim::Entity &entity,
                                         const std::shared_ptr<const sdf::Element> &sdf,
                                         gz::sim::EntityComponentManager &,
                                         gz::sim::EventManager &)
{
  this->world_entity_ = entity;

  if (sdf)
  {
    if (sdf->HasElement("topic_prefix"))
    {
      this->topic_prefix_ = sdf->Get<std::string>("topic_prefix");
      if (this->topic_prefix_.empty())
      {
        this->topic_prefix_ = "/actor_pose";
      }
    }

    if (sdf->HasElement("publish_rate"))
    {
      const double rate = sdf->Get<double>("publish_rate");
      if (rate >= 0.0)
      {
        this->publish_rate_hz_ = rate;
      }
    }
  }

  if (this->publish_rate_hz_ > 0.0)
  {
    const double period = 1.0 / this->publish_rate_hz_;
    this->publish_period_ =
      std::chrono::duration_cast<std::chrono::steady_clock::duration>(
        std::chrono::duration<double>(period));
  }
  else
  {
    this->publish_period_ = std::chrono::steady_clock::duration::zero();
  }

  this->aggregate_topic_ = this->topic_prefix_ + kAggregateSuffix;
}

//////////////////////////////////////////////////
void ActorPosePublisherSystem::PostUpdate(const gz::sim::UpdateInfo &info,
                                          const gz::sim::EntityComponentManager &ecm)
{
  if (info.paused && !this->first_publish_)
  {
    return;
  }

  if (!this->first_publish_ && this->publish_period_.count() > 0)
  {
    const auto elapsed = info.simTime - this->last_publish_time_;
    if (elapsed < this->publish_period_)
    {
      return;
    }
  }

  this->EnsureAggregatePublisher();
  gz::msgs::Pose_V aggregate_msg;

  // 遍历所有 Actor，发布单独位姿并填充聚合消息
  ecm.Each<gz::sim::components::Actor,
           gz::sim::components::Name,
           gz::sim::components::Pose>(
    [&](const gz::sim::Entity &,
        const gz::sim::components::Actor *,
        const gz::sim::components::Name *name,
        const gz::sim::components::Pose *pose) -> bool
    {
      if (name == nullptr || pose == nullptr)
      {
        return true;
      }

      const std::string actor_name = name->Data();

      auto &pub = this->GetOrCreatePublisher(actor_name);

      gz::msgs::Pose pose_msg;
      pose_msg.set_name(actor_name);
      gz::msgs::Set(pose_msg.mutable_position(), pose->Data().Pos());
      gz::msgs::Set(pose_msg.mutable_orientation(), pose->Data().Rot());

      pub.Publish(pose_msg);

      auto *agg_pose = aggregate_msg.add_pose();
      agg_pose->CopyFrom(pose_msg);

      return true;
    });

  if (aggregate_msg.pose_size() > 0 && this->aggregate_pub_)
  {
    aggregate_msg.mutable_header()->mutable_stamp()->set_sec(
      std::chrono::duration_cast<std::chrono::seconds>(info.simTime).count());
    aggregate_msg.mutable_header()->mutable_stamp()->set_nsec(
      std::chrono::duration_cast<std::chrono::nanoseconds>(info.simTime).count() % 1000000000L);
    this->aggregate_pub_.Publish(aggregate_msg);
  }

  this->last_publish_time_ = info.simTime;
  this->first_publish_ = false;
}

//////////////////////////////////////////////////
gz::transport::Node::Publisher &
ActorPosePublisherSystem::GetOrCreatePublisher(const std::string &actor_name)
{
  auto it = this->actor_publishers_.find(actor_name);
  if (it != this->actor_publishers_.end())
  {
    return it->second;
  }

  std::string topic = this->topic_prefix_;
  if (!topic.empty() && topic.back() != '/')
  {
    topic += '/';
  }
  topic += actor_name;

  auto result = this->actor_publishers_.emplace(
    actor_name, this->node_.Advertise<gz::msgs::Pose>(topic));

  if (!result.second)
  {
    // 插入失败时返回已有元素（理论上不会发生）
    return it->second;
  }

  return result.first->second;
}

//////////////////////////////////////////////////
void ActorPosePublisherSystem::EnsureAggregatePublisher()
{
  if (this->aggregate_pub_)
  {
    return;
  }

  this->aggregate_pub_ = this->node_.Advertise<gz::msgs::Pose_V>(this->aggregate_topic_);
}

}  // namespace saye_description

GZ_ADD_PLUGIN(saye_description::ActorPosePublisherSystem,
              gz::sim::System,
              saye_description::ActorPosePublisherSystem::ISystemConfigure,
              saye_description::ActorPosePublisherSystem::ISystemPostUpdate)

GZ_ADD_PLUGIN_ALIAS(saye_description::ActorPosePublisherSystem,
                    "saye::ActorPosePublisherSystem")
