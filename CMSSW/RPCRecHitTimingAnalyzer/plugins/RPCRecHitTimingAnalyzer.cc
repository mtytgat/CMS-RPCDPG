// -*- C++ -*-
//
// Package:    RPCDPG/RPCRecHitTimingAnalyzer
// Class:      RPCRecHitTimingAnalyzer
//
/**\class RPCRecHitTimingAnalyzer RPCRecHitTimingAnalyzer.cc RPCDPG/RPCRecHitTimingAnalyzer/plugins/RPCRecHitTimingAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Michael Tytgat
//         Created:  Fri, 24 Apr 2020 15:25:15 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"
#include "DataFormats/TrackReco/interface/TrackExtraFwd.h"

#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHitFwd.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

#include "DataFormats/RPCRecHit/interface/RPCRecHitCollection.h"

#include <TFile.h>
#include <TTree.h>

#include "RPCDPG/RPCRecHitTimingAnalyzer/interface/myRPCRecHitTimingEvent.h"

//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<>
// This will improve performance in multithreaded jobs.

using reco::TrackCollection;
using reco::MuonCollection;
using namespace std;

class RPCRecHitTimingAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
public:
  explicit RPCRecHitTimingAnalyzer(const edm::ParameterSet&);
  ~RPCRecHitTimingAnalyzer();
  
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  
private:
  virtual void beginJob() override;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;
  
  // ----------member data ---------------------------
  edm::InputTag muonlabel_;
  edm::InputTag muontracklabel_;
  edm::InputTag rpcrechitlabel_;
  
  edm::EDGetTokenT<reco::MuonCollection> muonToken_;  
  edm::EDGetTokenT<reco::TrackCollection> muontrackToken_;  
  edm::EDGetTokenT<RPCRecHitCollection> rpcrechitToken_;

  std::string OutputFileName;
  TFile* OutputFile;
  TTree* myTree;
  myRPCRecHitTimingEvent_t myRPCRecHitTimingEvent;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
RPCRecHitTimingAnalyzer::RPCRecHitTimingAnalyzer(const edm::ParameterSet& iConfig)
  : OutputFileName(iConfig.getParameter<edm::ParameterSet>("RPCRecHitTimingParameters").getUntrackedParameter<std::string>("outputfile", std::string("output/RPCRecHitTimingAnalyzer.root")))
{
  muonlabel_ = iConfig.getParameter<edm::ParameterSet>("RPCRecHitTimingParameters").getUntrackedParameter<edm::InputTag>("muons");
  muontracklabel_ = iConfig.getParameter<edm::ParameterSet>("RPCRecHitTimingParameters").getUntrackedParameter<edm::InputTag>("muontracks");
  //rpcrechitlabel_ = iConfig.getParameter<edm::ParameterSet>("RPCRecHitTimingParameters").getUntrackedParameter<edm::InputTag>("rpcrechits");

  muonToken_ = consumes<reco::MuonCollection>(muonlabel_);
  muontrackToken_ = consumes<reco::TrackCollection>(muontracklabel_);
  rpcrechitToken_ = consumes<RPCRecHitCollection>(iConfig.getParameter<edm::ParameterSet>("RPCRecHitTimingParameters").getUntrackedParameter<edm::InputTag>("rpcrechits"));

}

RPCRecHitTimingAnalyzer::~RPCRecHitTimingAnalyzer()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

//
// member functions
//

// ------------ method called for each event  ------------
void
RPCRecHitTimingAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   myRPCRecHitTimingEvent.NRPCRecHit = 0;
   myRPCRecHitTimingEvent.NMuon = 0;
   myRPCRecHitTimingEvent.Event = (Int_t) iEvent.id().event();
   myRPCRecHitTimingEvent.Run = (Int_t) iEvent.id().run();

   cout << "Working on event number: " << iEvent.id().event() << endl;

   Handle<reco::MuonCollection> muons;
   iEvent.getByToken(muonToken_, muons);
   for (MuonCollection::const_iterator itMuon = muons->begin();
       itMuon != muons->end();
       ++itMuon) {

     // muon track info
     if (itMuon->combinedMuon().isNull()) continue;
     myRPCRecHitTimingEvent.MuonType[myRPCRecHitTimingEvent.NMuon] = (Int_t) itMuon->type();

     reco::TrackRef muonTrack = itMuon->combinedMuon();
     cout << "number of muon track hits: " << muonTrack->recHitsSize() << endl;  
     /*for (trackingRecHit_iterator itTrackRecHit = muonTrack->recHitsBegin();
	  itTrackRecHit != muonTrack->recHitsEnd();
	  ++itTrackRecHit) {
       cout << "rechit: " << endl;
       itTrackRecHit->
       }*/
     for (const auto& TrackRecHit : muonTrack->recHits()) {
       cout << "TrackRecHit Id: " << TrackRecHit->rawId() << endl;
     }
     myRPCRecHitTimingEvent.MuonCharge[myRPCRecHitTimingEvent.NMuon] = (Int_t) itMuon->charge();

     // Muon timing info: DT/CSC and RPC 
     myRPCRecHitTimingEvent.MuonTimeValid[myRPCRecHitTimingEvent.NMuon] = (Bool_t) itMuon->isTimeValid();

     reco::MuonTime muontime = itMuon->time();
     myRPCRecHitTimingEvent.MuonTimeAtIpInOut[myRPCRecHitTimingEvent.NMuon] = (Float_t) muontime.timeAtIpInOut;

     reco::MuonTime muonrpctime = itMuon->rpcTime();
     myRPCRecHitTimingEvent.MuonRPCTimeAtIpInOut[myRPCRecHitTimingEvent.NMuon] = (Float_t) muonrpctime.timeAtIpInOut;
     
     // done
     myRPCRecHitTimingEvent.NMuon++;
   }

   /*
#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif

#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
   */

   // RPC RecHit collection
   edm::Handle<RPCRecHitCollection> rpcrechits;
   if (!iEvent.getByToken(rpcrechitToken_, rpcrechits)) {
     edm::LogWarning("RPCRecHitTimingAnalyzer") << "Problemos !"; 
     return;
   }

   if (rpcrechits.isValid()) {
     cout << "we are now rpcrechit-ing ;-)" << endl;

     for (RPCRecHitCollection::const_iterator itRPCRecHit = rpcrechits->begin();
	  itRPCRecHit != rpcrechits->end();
	  ++itRPCRecHit) {

       myRPCRecHitTimingEvent.RPCRecHitBunchX[myRPCRecHitTimingEvent.NRPCRecHit] = (Int_t) itRPCRecHit->BunchX();
       myRPCRecHitTimingEvent.RPCRecHitTime[myRPCRecHitTimingEvent.NRPCRecHit] = (Float_t) itRPCRecHit->time();
       myRPCRecHitTimingEvent.RPCRecHitTimeError[myRPCRecHitTimingEvent.NRPCRecHit] = (Float_t) itRPCRecHit->timeError();
       myRPCRecHitTimingEvent.NRPCRecHit++;

       // TrackingRecHits
       // this is a "ConstRecHitContainer"
       std::vector<const TrackingRecHit*> trackingRecHits = itRPCRecHit->recHits();

     }
   
   /*
     for (const auto& rechits: *rpcrechits) {
       DetId detId(rechits.id());
       const GeomDetUnit* geomDetUnit = tGeom->IdToDetUnit(detId);

       for (const auto& rechit: rechits) {
	 if (!rechit.isValid()) continue;

       }

       }*/
   }

   myTree->Fill();
}


// ------------ method called once each job just before starting event loop  ------------
void
RPCRecHitTimingAnalyzer::beginJob()
{
  std::cout << "Using " << OutputFileName.c_str() << std::endl;
  OutputFile = new TFile(OutputFileName.c_str(), "RECREATE");

  myTree = new TTree("RPCRecHitTiming", "RPCRecHitTimingAnalyzer");
  myTree->Branch("Event", &myRPCRecHitTimingEvent.Event, "Event/I");
  myTree->Branch("Run", &myRPCRecHitTimingEvent.Run, "Run/I");

  myTree->Branch("NRPCRecHit", &myRPCRecHitTimingEvent.NRPCRecHit, "NRPCRecHit/I");
  myTree->Branch("RPCRecHitBunchX", &myRPCRecHitTimingEvent.RPCRecHitBunchX, "RPCRecHitBunchX[NRPCRecHit]/I");
  myTree->Branch("RPCRecHitTime", &myRPCRecHitTimingEvent.RPCRecHitTime, "RPCRecHitTime[NRPCRecHit]/F");
  myTree->Branch("RPCRecHitTimeError", &myRPCRecHitTimingEvent.RPCRecHitTimeError, "RPCRecHitTimeError[NRPCRecHit]/F");
  
  myTree->Branch("NMuon", &myRPCRecHitTimingEvent.NMuon, "NMuon/I");
  myTree->Branch("MuonType", &myRPCRecHitTimingEvent.MuonType, "MuonType[NMuon]/I");
  myTree->Branch("MuonCharge", &myRPCRecHitTimingEvent.MuonCharge, "MuonCharge[NMuon]/I");
  myTree->Branch("MuonTimeValid", &myRPCRecHitTimingEvent.MuonTimeValid, "MuonTimeValid[NMuon]/B");
  myTree->Branch("MuonTimeAtIpInOut", &myRPCRecHitTimingEvent.MuonTimeAtIpInOut, "MuonTimeAtIpInOut[NMuon]/F");
  myTree->Branch("MuonRPCTimeAtIpInOut", &myRPCRecHitTimingEvent.MuonRPCTimeAtIpInOut, "MuonRPCTimeAtIpInOut[NMuon]/F");
}

// ------------ method called once each job just after ending the event loop  ------------
void
RPCRecHitTimingAnalyzer::endJob()
{
  OutputFile->Write();
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
RPCRecHitTimingAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);

  //Specify that only 'tracks' is allowed
  //To use, remove the default given above and uncomment below
  //ParameterSetDescription desc;
  //desc.addUntracked<edm::InputTag>("tracks","ctfWithMaterialTracks");
  //descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(RPCRecHitTimingAnalyzer);
