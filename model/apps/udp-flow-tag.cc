/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "udp-flow-tag.h"

#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("UdpFlowTag");
NS_OBJECT_ENSURE_REGISTERED (UdpFlowTag);

TypeId
UdpFlowTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::UdpFlowTag")
    .SetParent<Tag> ()
    .SetGroupName ("BasicSim")
    .AddConstructor<UdpFlowTag> ();
  return tid;
}

UdpFlowTag::UdpFlowTag ()
  : m_flowId (0),
    m_packetSequence (0)
{
}

UdpFlowTag::UdpFlowTag (uint64_t flowId, uint64_t packetSequence)
  : m_flowId (flowId),
    m_packetSequence (packetSequence)
{
}

TypeId
UdpFlowTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t
UdpFlowTag::GetSerializedSize (void) const
{
  return 16;
}

void
UdpFlowTag::Serialize (TagBuffer buffer) const
{
  buffer.WriteU64 (m_flowId);
  buffer.WriteU64 (m_packetSequence);
}

void
UdpFlowTag::Deserialize (TagBuffer buffer)
{
  m_flowId = buffer.ReadU64 ();
  m_packetSequence = buffer.ReadU64 ();
}

void
UdpFlowTag::Print (std::ostream &os) const
{
  os << "flow_id=" << m_flowId << ",packet_sequence=" << m_packetSequence;
}

uint64_t
UdpFlowTag::GetFlowId (void) const
{
  return m_flowId;
}

uint64_t
UdpFlowTag::GetPacketSequence (void) const
{
  return m_packetSequence;
}

} // namespace ns3
