/**************************************************************************/
/*!
    @file     BLEEventLogger.cpp
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

#include "BLEEventLogger.h"

// Initialize static members
StaticQueue_t BLEEventLogger::event_queue_buffer_;
uint8_t BLEEventLogger::event_queue_storage_[EVENT_QUEUE_SIZE * sizeof(BLEEvent)];
QueueHandle_t BLEEventLogger::event_queue_handle_ = nullptr;

void BLEEventLogger::begin() {
  if (!event_queue_handle_) {
    event_queue_handle_ = xQueueCreateStatic(EVENT_QUEUE_SIZE,
                                           sizeof(BLEEvent),
                                           event_queue_storage_,
                                           &event_queue_buffer_);
  }
}

void BLEEventLogger::end() {
  // Static queue doesn't need deletion
  event_queue_handle_ = nullptr;
}

void BLEEventLogger::pushEvent(const BLEEvent& evt) {
  if (!event_queue_handle_) return;
  
  // Try to queue the event, if queue is full just drop it
  xQueueSend(event_queue_handle_, &evt, 0);
}

bool BLEEventLogger::popEvent(BLEEvent& evt) {
  if (!event_queue_handle_) return false;
  
  return xQueueReceive(event_queue_handle_, &evt, 0) == pdTRUE;
}

void BLEEventLogger::printEvent(const BLEEvent& evt) {
  // Common fields for all events
  Serial.printf("[%lu] Conn: %d, ", evt.timestamp, evt.conn_handle);

  switch (evt.type) {
    case BLEEvent::HVN_PACKET_TAKE:
      Serial.printf("HVN Take - Success: %s, Queue: %d/%d (Used: %d)\n",
                   evt.success ? "true" : "false",
                   evt.hvn.available_packets,
                   evt.hvn.hvn_queue_size,
                   evt.hvn.packets_in_use);
      break;
      
    case BLEEvent::HVN_PACKET_RELEASE:
      Serial.printf("HVN Release - Success: %s, Queue: %d/%d (Used: %d)\n",
                   evt.success ? "true" : "false",
                   evt.hvn.available_packets,
                   evt.hvn.hvn_queue_size,
                   evt.hvn.packets_in_use);
      break;
      
    case BLEEvent::HVN_TX_COMPLETE:
      Serial.printf("HVN TX %s - Count: %d, Queue: %d/%d (Used: %d)\n",
                   evt.success ? "Complete" : "Failed",
                   evt.hvn.count,
                   evt.hvn.available_packets,
                   evt.hvn.hvn_queue_size,
                   evt.hvn.packets_in_use);
      break;

    case BLEEvent::HVN_NOTIFY_START:
      Serial.printf("Notify Start - Total Packets: %d, HVN Queue Size: %d\n",
                   evt.hvn.count,
                   evt.hvn.hvn_queue_size);
      break;

    case BLEEvent::HVN_NOTIFY_CHUNK:
      Serial.printf("Notify Chunk %d/%d\n",
                   evt.hvn.count,
                   evt.hvn.hvn_queue_size);
      break;

    case BLEEvent::HVN_NOTIFY_COMPLETE:
      Serial.printf("Notify Complete - Sent %d/%d packets\n",
                   evt.hvn.count,
                   evt.hvn.hvn_queue_size);
      break;

    case BLEEvent::HVN_NOTIFY_FAILED:
      Serial.printf("Notify Failed - Sent %d/%d packets\n",
                   evt.hvn.count,
                   evt.hvn.hvn_queue_size);
      break;
      
    case BLEEvent::HID_REPORT_QUEUED:
      Serial.printf("HID Report Queued - ID: %d, Type: %d, Retries: %d\n",
                   evt.hid.report_id,
                   evt.hid.report_type,
                   evt.hid.retries_left);
      break;
      
    case BLEEvent::HID_REPORT_SENT:
      Serial.printf("HID Report Sent - ID: %d, Type: %d\n",
                   evt.hid.report_id,
                   evt.hid.report_type);
      break;
      
    case BLEEvent::HID_REPORT_FAILED:
      Serial.printf("HID Report Failed - ID: %d, Type: %d, Retries Left: %d\n",
                   evt.hid.report_id,
                   evt.hid.report_type,
                   evt.hid.retries_left);
      break;

    default:
      Serial.printf("Unknown Event Type: %d\n", evt.type);
      break;
  }
}

void BLEEventLogger::processEvents() {
  BLEEvent evt;
    if (!event_queue_handle_) 
    { 
      Serial.printf("No event queue handle\n");
    }
  while (popEvent(evt)) {
    printEvent(evt);
  }
}

BLEEventLogger bleEventLogger; 