/**************************************************************************/
/*!
    @file     BLEEventLogger.h
    @author   Keyboard.io

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2024, Keyboard.io
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/

#pragma once

#include <Arduino.h>
#include "bluefruit_common.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "semphr.h"

// Event structure for BLE events
struct BLEEvent {
  enum Type {
    HVN_PACKET_TAKE,
    HVN_PACKET_RELEASE,
    HVN_TX_COMPLETE,
    HID_REPORT_QUEUED,
    HID_REPORT_SENT,
    HID_REPORT_FAILED,
    HVN_NOTIFY_START,       // Starting a notification process
    HVN_NOTIFY_CHUNK,       // Individual chunk notification
    HVN_NOTIFY_COMPLETE,    // All chunks sent successfully
    HVN_NOTIFY_FAILED      // Notification failed
  };

  uint32_t timestamp;
  Type type;
  uint16_t conn_handle;
  bool success;

  union {
    struct {
      uint8_t hvn_queue_size;
      uint8_t available_packets;
      uint8_t packets_in_use;
      uint8_t count;  // For TX_COMPLETE events
    } hvn;

    struct {
      uint8_t report_id;
      uint8_t report_type;
      uint8_t retries_left;
    } hid;
  };
};

class BLEEventLogger {
 private:
  static constexpr size_t EVENT_QUEUE_SIZE = 1024;
  static StaticQueue_t event_queue_buffer_;
  static uint8_t event_queue_storage_[EVENT_QUEUE_SIZE * sizeof(BLEEvent)];
  static QueueHandle_t event_queue_handle_;

 public:
  static void begin();
  static void end();
  static void pushEvent(const BLEEvent& evt);
  static bool popEvent(BLEEvent& evt);
  static void printEvent(const BLEEvent& evt);
  static void processEvents();
};

extern BLEEventLogger bleEventLogger; 