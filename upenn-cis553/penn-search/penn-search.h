/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 University of Pennsylvania
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
 */

#ifndef PENN_SEARCH_H
#define PENN_SEARCH_H

#include "ns3/penn-application.h"
#include "ns3/penn-chord.h"
#include "ns3/penn-search-message.h"
#include "ns3/ping-request.h"

#include "ns3/ipv4-address.h"
#include <map>
#include <set>
#include <vector>
#include <string>
#include <fstream>
#include "ns3/socket.h"
#include "ns3/nstime.h"
#include "ns3/timer.h"
#include "ns3/uinteger.h"
#include "ns3/boolean.h"

using namespace ns3;

class PennSearch : public PennApplication
{
  public:
    static TypeId GetTypeId (void);
    PennSearch ();
    virtual ~PennSearch ();

    void SendPing (std::string nodeId, std::string pingMessage);
    void SendPennSearchPing (Ipv4Address destAddress, std::string pingMessage);
    void RecvMessage (Ptr<Socket> socket);
    void ProcessPingReq (PennSearchMessage message, Ipv4Address sourceAddress, uint16_t sourcePort);
    void ProcessPingRsp (PennSearchMessage message, Ipv4Address sourceAddress, uint16_t sourcePort);
    void AuditPings ();
    uint32_t GetNextTransactionId ();
    void Publish ();
    void SendInvertList(std::string key, Ipv4Address addressResponsible, uint32_t transactionId);
    void ProcessStoreList (PennSearchMessage message, Ipv4Address sourceAddress, uint16_t sourcePort);
    void ProcessSearchInitial (PennSearchMessage message, Ipv4Address sourceAddress, uint16_t sourcePort);
    void SendSearchBegin (std::string key,Ipv4Address addressResponsible, uint32_t transactionId);
    void ProcessSearchBegin (PennSearchMessage message, Ipv4Address sourceAddress, uint16_t sourcePort);
    void SendSearch (std::string key,Ipv4Address addressResponsible, uint32_t transactionId);
    void ProcessSearch (PennSearchMessage message, Ipv4Address sourceAddress, uint16_t sourcePort);
    void SendSearchComplete (Ipv4Address initiatorAddress, std::vector<std::string> keyList, std::vector<std::string> docList, uint32_t transactionId);
    void ProcessSearchComplete (PennSearchMessage message, Ipv4Address sourceAddress, uint16_t sourcePort);
    void PassKeysJoin (Ipv4Address predecessorAddress, uint32_t transactionId);
    void PassKeysLeave (Ipv4Address successorAddress, uint32_t transactionId);
    void ProcessPassKeys (PennSearchMessage message, Ipv4Address sourceAddress, uint16_t sourcePort);

    
    
    // Chord Callbacks
    void HandleChordPingSuccess (Ipv4Address destAddress, std::string message);
    void HandleChordPingFailure (Ipv4Address destAddress, std::string message);
    void HandleChordPingRecv (Ipv4Address destAddress, std::string message);
    
    void HandleChordLookupPublishSuccess (std::string message, Ipv4Address destAddress, uint32_t transactionId);
    void HandleChordSearchInitialSuccess (std::string message, Ipv4Address destAddress, uint32_t transactionId);
    void HandleChordSearchBeginSuccess (std::string message, Ipv4Address destAddress, uint32_t transactionId);
    void HandleChordJoinNotify ( Ipv4Address predecessorAddress, uint32_t transactionId);
    void HandleChordLeaveNotify ( Ipv4Address successorAddress, uint32_t transactionId);

    // From PennApplication
    virtual void ProcessCommand (std::vector<std::string> tokens);
    // From PennLog
    virtual void SetTrafficVerbose (bool on);
    virtual void SetErrorVerbose (bool on);
    virtual void SetDebugVerbose (bool on);
    virtual void SetStatusVerbose (bool on);
    virtual void SetChordVerbose (bool on);
    virtual void SetSearchVerbose (bool on);
    void Tokenizer (const std::string& str,std::vector<std::string>& tokens,const std::string& delimiters);
    std::vector<std::string> FindIntersection (std::vector<std::string> v1, std::vector<std::string> v2);
    
  protected:
    virtual void DoDispose ();
    
  private:
    virtual void StartApplication (void);
    virtual void StopApplication (void);
     
    std::map<std::string,std::vector<std::string> > m_invertListMap;
    std::map<std::string,std::vector<std::string> > m_dataMap;
    
    struct SearchData
    {
      Ipv4Address initiatorAddress;
      std::vector<std::string> keyList;
      std::vector<std::string> docList;
    };
    
    void SHA_1 (Ipv4Address ipv4Addr, unsigned char *digest);
    void SHA_1 (std::string s, unsigned char *digest);
    uint8_t compareSHA1 (unsigned char *digest1, unsigned char *digest2);
       
    
    Ptr<PennChord> m_chord;
    uint32_t m_currentTransactionId;
    Ptr<Socket> m_socket;
    Time m_pingTimeout;
    uint16_t m_appPort, m_chordPort;
    // Timers
    Timer m_auditPingsTimer;
    // Ping tracker
    std::map<uint32_t, Ptr<PingRequest> > m_pingTracker;
    std::map<uint32_t, SearchData> m_searchTracker;
    
};


#endif


