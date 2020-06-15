import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Era_Phase2C4_timing_layer_bar_cff import Phase2C4_timing_layer_bar

process = cms.Process("RPCTiming", Phase2C4_timing_layer_bar)

process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.Geometry.GeometryExtended2023D41Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '106X_upgrade2023_realistic_v3', '')

process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('*'),
    destinations = cms.untracked.vstring('RPCRecHitTimingAnalyzer_Mu_FlatPt2to100_pythia8_gun'),
    RPCRecHitTimingAnalyzer_Mu_FlatPt2to100_pythia8_gun = cms.untracked.PSet(
        threshold = cms.untracked.string('DEBUG')
    )
)

process.maxEvents = cms.untracked.PSet( 
    input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:output/Mu_FlatPt2to100-pythia8-gun_RECO.root'
    )
)

process.RPCRecHitTiming = cms.EDAnalyzer('RPCRecHitTimingAnalyzer',
    RPCRecHitTimingParameters = cms.PSet(
        outputfile = cms.untracked.string("output/RPCRecHitTimingAnalyzer_Mu_Flat_Pt2to100-pythia8-gun.root"), 
        muons = cms.untracked.InputTag("muons"),
        muontracks = cms.untracked.InputTag("standAloneMuons"),
        rpcrechits = cms.untracked.InputTag("rpcRecHits"),
        genparticles = cms.untracked.InputTag("genParticles","")
    )
)

process.p = cms.Path(process.RPCRecHitTiming)
