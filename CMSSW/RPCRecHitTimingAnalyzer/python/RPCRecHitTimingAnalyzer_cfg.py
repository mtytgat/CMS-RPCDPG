import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process("RPCTiming", eras.Run2_2018)

process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_Data_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '102X_dataRun2_Prompt_v15', '')

process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('*'),
    destinations = cms.untracked.vstring('RPCRecHitTimingAnalyzer'),
    RPCRecHitTimingAnalyzer = cms.untracked.PSet(
        threshold = cms.untracked.string('DEBUG')
    )
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        'file:output/MyEvents_2018_RECO.root'
    )
)

process.RPCRecHitTiming = cms.EDAnalyzer('RPCRecHitTimingAnalyzer',
    RPCRecHitTimingParameters = cms.PSet(
        muons = cms.untracked.InputTag("muons"),
        muontracks = cms.untracked.InputTag("standAloneMuons"),
        rpcrechits = cms.untracked.InputTag("rpcRecHits")
    )
)

process.p = cms.Path(process.RPCRecHitTiming)
