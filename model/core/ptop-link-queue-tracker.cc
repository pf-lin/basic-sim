/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2020 ETH Zurich
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Simon
 */

#include "ptop-link-queue-tracker.h"

namespace ns3 {

    NS_OBJECT_ENSURE_REGISTERED (PtopLinkQueueTracker);
    TypeId PtopLinkQueueTracker::GetTypeId (void)
    {
        static TypeId tid = TypeId ("ns3::PtopLinkQueueTracker")
                .SetParent<Object> ()
                .SetGroupName("BasicSim")
        ;
        return tid;
    }

    // 新增：預設構造函數
    PtopLinkQueueTracker::PtopLinkQueueTracker() {
        m_queue = nullptr;
        m_log_update_helper_queue_pkt = LogUpdateHelper();
        m_log_update_helper_queue_byte = LogUpdateHelper();
    }

    // 新增：SetQueue 方法
    void PtopLinkQueueTracker::SetQueue(Ptr<Queue<Packet>> queue) {
        if (queue == nullptr) {
            NS_FATAL_ERROR("PtopLinkQueueTracker::SetQueue: Queue is null");
        }
        
        m_queue = queue;
        
        // Logging number of packets in the queue
        m_log_update_helper_queue_pkt.Update(0, 0);
        m_queue->TraceConnectWithoutContext(
            "PacketsInQueue", 
            MakeCallback(&PtopLinkQueueTracker::NetDevicePacketsInQueueCallback, this)
        );
        
        // Logging bytes in the queue
        m_log_update_helper_queue_byte.Update(0, 0);
        m_queue->TraceConnectWithoutContext(
            "BytesInQueue", 
            MakeCallback(&PtopLinkQueueTracker::NetDeviceBytesInQueueCallback, this)
        );
    }

    void PtopLinkQueueTracker::Reset() {
        // 重置兩個 log helper
        m_log_update_helper_queue_pkt = LogUpdateHelper();
        m_log_update_helper_queue_byte = LogUpdateHelper();
        
        // 使用當前時間和當前 queue 狀態初始化
        int64_t current_time_ns = Simulator::Now().GetNanoSeconds();
        
        if (m_queue != nullptr) {
            uint32_t current_packets = m_queue->GetNPackets();
            uint32_t current_bytes = m_queue->GetNBytes();
            
            m_log_update_helper_queue_pkt.Update(current_time_ns, current_packets);
            m_log_update_helper_queue_byte.Update(current_time_ns, current_bytes);
        } else {
            m_log_update_helper_queue_pkt.Update(current_time_ns, 0);
            m_log_update_helper_queue_byte.Update(current_time_ns, 0);
        }
    }

    PtopLinkQueueTracker::PtopLinkQueueTracker(Ptr<PointToPointNetDevice> netDevice) {

        // Register this tracker into the tracing callbacks of the network device
        m_queue = netDevice->GetQueue();

        // 檢查 queue 是否有效
        if (m_queue == nullptr) {
            NS_FATAL_ERROR("PtopLinkQueueTracker: NetDevice queue is null");
        }

        // Logging number of packets in the queue
        m_log_update_helper_queue_pkt = LogUpdateHelper();
        m_log_update_helper_queue_pkt.Update(0, 0);
        m_queue->TraceConnectWithoutContext("PacketsInQueue", MakeCallback(&PtopLinkQueueTracker::NetDevicePacketsInQueueCallback, this));
        
        // Logging bytes in the queue
        m_log_update_helper_queue_byte = LogUpdateHelper();
        m_log_update_helper_queue_byte.Update(0, 0);
        m_queue->TraceConnectWithoutContext("BytesInQueue", MakeCallback(&PtopLinkQueueTracker::NetDeviceBytesInQueueCallback, this));

    }

    void PtopLinkQueueTracker::NetDevicePacketsInQueueCallback(uint32_t, uint32_t num_packets) {
        m_log_update_helper_queue_pkt.Update(
                (int64_t) Simulator::Now().GetNanoSeconds(),
                num_packets
        );
    }

    void PtopLinkQueueTracker::NetDeviceBytesInQueueCallback(uint32_t, uint32_t num_bytes) {
        m_log_update_helper_queue_byte.Update(
                (int64_t) Simulator::Now().GetNanoSeconds(),
                num_bytes
        );
    }

    const std::vector<std::tuple<int64_t, int64_t, int64_t>>& PtopLinkQueueTracker::GetIntervalsNumPackets() {
        return m_log_update_helper_queue_pkt.Finalize((int64_t) Simulator::Now().GetNanoSeconds());
    }

    const std::vector<std::tuple<int64_t, int64_t, int64_t>>& PtopLinkQueueTracker::GetIntervalsNumBytes() {
        return m_log_update_helper_queue_byte.Finalize((int64_t) Simulator::Now().GetNanoSeconds());
    }

}
