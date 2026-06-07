/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef UDP_FLOW_TAG_H
#define UDP_FLOW_TAG_H

#include "ns3/tag.h"

namespace ns3 {

class UdpFlowTag : public Tag
{
public:
  static TypeId GetTypeId (void);

  UdpFlowTag ();
  UdpFlowTag (uint64_t flowId, uint64_t packetSequence);

  virtual TypeId GetInstanceTypeId (void) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer buffer) const;
  virtual void Deserialize (TagBuffer buffer);
  virtual void Print (std::ostream &os) const;

  uint64_t GetFlowId (void) const;
  uint64_t GetPacketSequence (void) const;

private:
  uint64_t m_flowId;
  uint64_t m_packetSequence;
};

} // namespace ns3

#endif /* UDP_FLOW_TAG_H */
