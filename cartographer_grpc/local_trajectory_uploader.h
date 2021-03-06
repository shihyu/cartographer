/*
 * Copyright 2018 The Cartographer Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CARTOGRAPHER_GRPC_LOCAL_TRAJECTORY_UPLOADER_H
#define CARTOGRAPHER_GRPC_LOCAL_TRAJECTORY_UPLOADER_H

#include <map>
#include <string>
#include <unordered_set>

#include "cartographer/common/blocking_queue.h"
#include "cartographer/mapping/proto/trajectory_builder_options.pb.h"
#include "cartographer_grpc/proto/map_builder_service.grpc.pb.h"
#include "grpc++/grpc++.h"

namespace cartographer_grpc {

class LocalTrajectoryUploader {
 public:
  LocalTrajectoryUploader(const std::string& server_address);
  void AddTrajectory(
      int local_trajectory_id,
      const std::unordered_set<std::string>& expected_sensor_ids,
      const cartographer::mapping::proto::TrajectoryBuilderOptions&
          trajectory_options);
  void FinishTrajectory(int local_trajectory_id);
  void EnqueueDataRequest(
      std::unique_ptr<google::protobuf::Message> data_request);

 private:
  std::shared_ptr<grpc::Channel> client_channel_;
  std::unique_ptr<proto::MapBuilderService::Stub> service_stub_;
  std::map<int, int> local_to_cloud_trajectory_id_map_;
  cartographer::common::BlockingQueue<
      std::unique_ptr<google::protobuf::Message>>
      send_queue_;
};

}  // namespace cartographer_grpc

#endif  // CARTOGRAPHER_GRPC_LOCAL_TRAJECTORY_UPLOADER_H
