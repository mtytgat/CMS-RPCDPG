import FWCore.ParameterSet.Config as cms

# Give the process a name
process = cms.Process("PickEvent")

# Tell the process which files to use as the source
process.source = cms.Source ("PoolSource",
#          fileNames = cms.untracked.vstring ("/store/data/Run2017F/Commissioning/RAW/v1/000/306/460/00000/7AD2D2FE-27C6-E711-920C-02163E01A22E.root")
#           fileNames = cms.untracked.vstring ("/store/data/Run2018C/Cosmics/RAW/v1/000/319/313/00000/F2DA7355-E981-E811-B685-FA163E687348.root")    
#            fileNames = cms.untracked.vstring ("/store/data/Run2018D/SingleMuon/RAW/v1/000/321/051/00000/00B63BBA-C39B-E811-AF54-FA163EF033E8.root")
            fileNames = cms.untracked.vstring("/store/mc/PhaseIITDRSpring19DR/Mu_FlatPt2to100-pythia8-gun/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3-v2/70000/739EB969-CEBF-E546-890F-6C4EC5506D8B.root")
)

# tell the process to only run over 100 events (-1 would mean run over
#  everything
process.maxEvents = cms.untracked.PSet(
            input = cms.untracked.int32 (20)

)

# Tell the process what filename to use to save the output
process.Out = cms.OutputModule("PoolOutputModule",
         fileName = cms.untracked.string ("output/Mu_Flat_Pt2to100-pythia8-gun.root")
)

# make sure everything is hooked up
process.end = cms.EndPath(process.Out)
