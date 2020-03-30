/******************************************************************************\
* Copyright (C) 2012-2017 Leap Motion, Inc. All rights reserved.               *
* Leap Motion proprietary and confidential. Not for distribution.              *
* Use subject to the terms of the Leap Motion SDK Agreement available at       *
* https://developer.leapmotion.com/sdk_agreement, or another agreement         *
* between Leap Motion and you, your company or other organization.             *
\******************************************************************************/
#ifndef ExampleConnection_h
#define ExampleConnection_h

#include "LeapC.h"

/* Client functions */
LEAP_CONNECTION* OpenConnection();
LEAP_CONNECTION* OpenConnectionWithConfig(const LEAP_CONNECTION_CONFIG *connectionConfig);
void CloseConnection();
void DestroyConnection();
LEAP_TRACKING_EVENT* GetFrame(); //Used in polling example
LEAP_DEVICE_INFO* GetDeviceProperties(); //Used in polling example
const char* ResultString(eLeapRS r);

/* State */
extern LEAP_CONNECTION ConnectionHandle;
extern bool IsConnected;
extern uint32_t DeviceId;

/* Callback function pointers */
typedef void (*connection_callback)           ();
typedef void (*device_callback)               (const LEAP_DEVICE_INFO *device_info, const LEAP_DEVICE device);
typedef void (*device_lost_callback)          ();
typedef void (*device_failure_callback)       (eLeapDeviceStatus failure_code, const LEAP_DEVICE failed_device);
typedef void (*policy_callback)               (uint32_t current_policies);
typedef void (*tracking_callback)             (const LEAP_TRACKING_EVENT *tracking_event);
typedef void (*log_callback)                  (eLeapLogSeverity severity, int64_t timestamp, const char* message);
typedef void (*config_change_callback)        (uint32_t requestID, bool success);
typedef void (*config_response_callback)      (uint32_t requestID, const LEAP_VARIANT value);
typedef void (*device_status_change_callback) (const LEAP_DEVICE_REF device, uint32_t last_status, uint32_t status);
typedef void (*dropped_frame_callback)        (int64_t frame_id, eLeapDroppedFrameType type);
typedef void (*image_callback)                (const LEAP_IMAGE_EVENT *image_event);
typedef void (*point_mapping_change_callback) (const LEAP_POINT_MAPPING_CHANGE_EVENT *point_mapping_change_event);
typedef void (*head_pose_callback)            (const LEAP_HEAD_POSE_EVENT *head_pose_event);

struct Callbacks{
  connection_callback           on_connection;
  connection_callback           on_connection_lost;
  device_callback               on_device_found;
  device_lost_callback          on_device_lost;
  device_failure_callback       on_device_failure;
  policy_callback               on_policy;
  tracking_callback             on_frame;
  log_callback                  on_log_message;
  config_change_callback        on_config_change;
  config_response_callback      on_config_response;
  device_status_change_callback on_device_status_change;
  dropped_frame_callback        on_dropped_frame;
  image_callback                on_image;
  point_mapping_change_callback on_point_mapping_change;
  head_pose_callback            on_head_pose;
};
extern struct Callbacks ConnectionCallbacks;
extern void millisleep(int milliseconds);
#endif /* ExampleConnection_h */
