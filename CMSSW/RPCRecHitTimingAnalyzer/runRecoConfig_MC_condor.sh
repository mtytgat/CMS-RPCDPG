#!/bin/bash

cd /afs/cern.ch/user/m/mtytgat
source .bash_profile
echo "I am here:" 
pwd
cd cms/CMSSW/CMSSW_10_6_0_patch2
#eval `scramv1 runtime -sh`
cd src/RPCDPG/RPCRecHitTimingAnalyzer
export X509_USER_PROXY=$1
voms-proxy-info -all
voms-proxy-info -all -file $1
#cmsRun step1_DIGI_L1_DIGI2RAW.py
cmsRun reco_RAW2DIGI_L1Reco_RECO.py
