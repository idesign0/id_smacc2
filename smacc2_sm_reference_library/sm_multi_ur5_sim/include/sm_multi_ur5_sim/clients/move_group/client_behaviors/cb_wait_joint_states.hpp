
// Copyright 2021 RobosoftAI Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*****************************************************************************************************************
 *
 * 	 Authors: Pablo Inigo Blasco, Brett Aldrich
 *
 ******************************************************************************************************************/
#pragma once

#include <sensor_msgs/msg/joint_state.hpp>
#include <move_group_interface_client/common.hpp>
#include <move_group_interface_client/cl_movegroup.hpp>

#include <smacc2/smacc_asynchronous_client_behavior.hpp>
#include <condition_variable>

namespace cl_move_group_interface
{
// ci-movegroup-ns: MoveGroupInterface::Options spells its namespace field WITHOUT a
// trailing underscore from moveit2 2.9 onwards (it was move_group_namespace_ before).
// Pick whichever the installed moveit actually declares, so this header builds against
// both. The int/long parameter only orders the overloads. Same shim as the one in
// move_group_interface_client/src/.../cl_movegroup.cpp, but that one lives in an
// anonymous namespace inside a .cpp and so cannot be reached from here; this copy is in
// a NAMED namespace, which keeps it ODR-safe in a header.
namespace ci_detail
{
template <typename Options>
auto moveGroupNamespaceOf(const Options & o, int) -> decltype(o.move_group_namespace_)
{
  return o.move_group_namespace_;
}

template <typename Options>
auto moveGroupNamespaceOf(const Options & o, long) -> decltype(o.move_group_namespace)
{
  return o.move_group_namespace;
}
}  // namespace ci_detail
class CbWaitJointState : public smacc2::SmaccAsyncClientBehavior
{
public:
  CbWaitJointState()
  {

  }

  void onEntry() override
  {
    ClMoveGroup * movegroupClient_;
    requiresClient(movegroupClient_);

    //auto group_name = movegroupClient_->getOptions().group_name_;
    std::string ns = ci_detail::moveGroupNamespaceOf(movegroupClient_->getOptions(), 0);

    //auto topicname = "/joint_state_broadcaster_"UR5PREFIX"/joint_states";

    std::string topicname = "/joint_states";
    if (!ns.empty())
    {
        topicname = ns + "/" + topicname;
    }

    //rclcpp::SensorDataQoS qos;
    rclcpp::SubscriptionOptions sub_option;

    sub_ = getNode()->create_subscription<sensor_msgs::msg::JointState>(
        topicname, 20, std::bind(&CbWaitJointState::onMessageReceived, this, std::placeholders::_1),
        sub_option);

        mutex.lock();
    }

private:
  void onMessageReceived(const sensor_msgs::msg::JointState & msg)
  {
      postSuccessEvent();
      mutex.unlock();
      sub_ = nullptr; // unsuscribe
  }

  bool triggered = false;
  std::mutex mutex;
  rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr sub_;
};
}
