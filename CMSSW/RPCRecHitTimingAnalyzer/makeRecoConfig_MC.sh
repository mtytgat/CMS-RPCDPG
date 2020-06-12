##cmsDriver.py reco -s DIGI --mc --filein /store/mc/PhaseIITDRSpring19DR/Mu_FlatPt2to100-pythia8-gun/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3-v2/70000/739EB969-CEBF-E546-890F-6C4EC5506D8B.root --fileout file:output/MyEvents_Mu_FlatT2to100-pythia8-gun_RECO.root --conditions auto:phase2_realistic --geometry Extended2023D41 --era Phase2C4_timing_layer_bar --no_exec --eventcontent FEVT -n 10 

#cmsDriver.py reco -s DIGI2RAW,RAW2DIGI,L1Reco,RECO --mc --filein /store/mc/PhaseIITDRSpring19DR/Mu_FlatPt2to100-pythia8-gun/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3-v2/70000/739EB969-CEBF-E546-890F-6C4EC5506D8B.root --fileout file:output/Mu_FlatPt2to100-pythia8-gun_RECO.root --conditions 106X_upgrade2023_realistic_v3 --geometry Extended2023D41 --era Phase2C4_timing_layer_bar --no_exec --eventcontent FEVT -n 10 

cmsDriver.py step1 -s DIGI,L1,DIGI2RAW --mc --scenario pp --filein /store/mc/PhaseIITDRSpring19DR/Mu_FlatPt2to100-pythia8-gun/GEN-SIM-DIGI-RAW/PU200_106X_upgrade2023_realistic_v3-v2/70000/739EB969-CEBF-E546-890F-6C4EC5506D8B.root --fileout file:output/Mu_FlatPt2to100-pythia8-gun_step1.root --conditions 106X_upgrade2023_realistic_v3 --geometry Extended2023D41 --era Phase2C4_timing_layer_bar --no_exec --eventcontent FEVTDEBUGHLT -n -1 

cmsDriver.py reco -s RAW2DIGI,L1Reco,RECO --mc --scenario pp --filein file:output/Mu_FlatPt2to100-pythia8-gun_step1.root --fileout file:output/Mu_FlatPt2to100-pythia8-gun_RECO.root --conditions auto:phase2_realistic --geometry Extended2023D41 --era Phase2C4_timing_layer_bar --no_exec --eventcontent FEVTSIM --customise Configuration/DataProcessing/Utils.addMonitoring --runUnscheduled -n -1



