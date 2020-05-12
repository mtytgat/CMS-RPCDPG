#ifndef myRPCRecHitTimingEvent_H
#define myRPCRecHitTimingEvent_H

static const int MaxNMuon = 100000;
static const int MaxNRPCRecHit = 100000;

struct myRPCRecHitTimingEvent_t {
  Int_t Event;
  Int_t Run;

  Int_t NRPCRecHit;
  Int_t RPCRecHitBunchX[MaxNRPCRecHit];
  Float_t RPCRecHitTime[MaxNRPCRecHit];
  Float_t RPCRecHitTimeError[MaxNRPCRecHit];

  Int_t NMuon;
  Int_t MuonCharge[MaxNMuon];
  Float_t MuonTimeAtIpInOut[MaxNMuon];
};

#endif




