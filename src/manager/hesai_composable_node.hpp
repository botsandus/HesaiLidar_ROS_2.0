#pragma once

#include <rclcpp/rclcpp.hpp>
#include <rclcpp_components/register_node_macro.hpp>
#include "node_manager.h"

namespace hesai_ros_driver {

class HesaiComposableNode : public rclcpp::Node {
public:
  explicit HesaiComposableNode(const rclcpp::NodeOptions& options)
  : Node("hesai_ros_driver_node", options) {
    declare_parameter<std::string>("config_path", "");
    std::string config_path = get_parameter("config_path").as_string();
    if (config_path.empty()) {
      RCLCPP_ERROR(get_logger(), "config_path parameter is empty");
      return;
    }
    YAML::Node config = YAML::LoadFile(config_path);
    node_manager_ = std::make_shared<NodeManager>();
    // Use non-owning shared_ptr since the container owns this node's lifetime
    auto non_owning = std::shared_ptr<rclcpp::Node>(this, [](rclcpp::Node*){});
    node_manager_->Init(config, non_owning);
    node_manager_->Start();
  }

  ~HesaiComposableNode() {
    if (node_manager_) {
      node_manager_->Stop();
    }
  }

private:
  std::shared_ptr<NodeManager> node_manager_;
};

}  // namespace hesai_ros_driver

RCLCPP_COMPONENTS_REGISTER_NODE(hesai_ros_driver::HesaiComposableNode)
