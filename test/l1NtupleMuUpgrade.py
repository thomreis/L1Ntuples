import FWCore.ParameterSet.Config as cms

# make L1 ntuples from RAW+RECO

process = cms.Process("L1NTUPLE")

# import of standard configurations
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('Configuration/Geometry/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
process.load('Configuration/StandardSequences/SimL1Emulator_cff')
process.load("Configuration.StandardSequences.RawToDigi_cff")
process.load('Configuration/StandardSequences/EndOfProcess_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration/EventContent/EventContent_cff')
process.load('Configuration.StandardSequences.ReconstructionCosmics_cff')


process.mergedSuperClusters = cms.EDFilter("EgammaSuperClusterMerger",
#src = cms.VInputTag(cms.InputTag("correctedHybridSuperClusters"),cms.InputTag("correctedMulti5x5SuperClustersWithPreshower"))
src = cms.VInputTag(cms.InputTag("hybridSuperClusters"),cms.InputTag("multi5x5SuperClustersWithPreshower"))
)

# global tag
process.GlobalTag.globaltag = 'GR_P_V43::All'

# output file
process.TFileService = cms.Service("TFileService",
    fileName = cms.string('L1Tree_2.root')
)

# analysis
process.load("L1Trigger.Configuration.L1Extra_cff")
process.load("L1TriggerDPG.L1Ntuples.l1NtupleProducer_cfi")
process.load("L1TriggerDPG.L1Ntuples.l1RecoTreeProducer_cfi")
process.load("L1TriggerDPG.L1Ntuples.l1ExtraTreeProducer_cfi")
process.load("L1TriggerDPG.L1Ntuples.l1MuonRecoTreeProducer_cfi")
process.load("L1TriggerDPG.L1Ntuples.l1MuonUpgradeTreeProducer_cfi")

process.load("L1TriggerDPG.L1Ntuples.l1MenuTreeProducer_cfi")
process.load("EventFilter.L1GlobalTriggerRawToDigi.l1GtTriggerMenuLite_cfi")


process.l1NtupleProducer.hltSource            = cms.InputTag("none")
process.l1NtupleProducer.gtSource             = cms.InputTag("none")
process.l1NtupleProducer.gctCentralJetsSource = cms.InputTag("none")
process.l1NtupleProducer.gctNonIsoEmSource    = cms.InputTag("none")
process.l1NtupleProducer.gctForwardJetsSource = cms.InputTag("none")
process.l1NtupleProducer.gctIsoEmSource       = cms.InputTag("none")
process.l1NtupleProducer.gctEnergySumsSource  = cms.InputTag("none")
process.l1NtupleProducer.gctTauJetsSource     = cms.InputTag("none")
process.l1NtupleProducer.gctIsoTauJetsSource  = cms.InputTag("none")
process.l1NtupleProducer.rctSource            = cms.InputTag("none")
process.l1NtupleProducer.dttfSource           = cms.InputTag("none")
process.l1NtupleProducer.ecalSource           = cms.InputTag("none")
process.l1NtupleProducer.hcalSource           = cms.InputTag("none")
process.l1NtupleProducer.csctfTrkSource       = cms.InputTag("none")
process.l1NtupleProducer.csctfLCTSource       = cms.InputTag("none")
process.l1NtupleProducer.csctfStatusSource    = cms.InputTag("none")
csctfDTStubsSource   = cms.InputTag("none")

process.p = cms.Path(
    process.gtDigis
    +process.gtEvmDigis
    # +process.gctDigis
    # +process.dttfDigis
    # +process.csctfDigis
    +process.l1NtupleProducer
    # +process.l1extraParticles
    # +process.l1ExtraTreeProducer
    # +process.l1GtTriggerMenuLite
    # +process.l1MenuTreeProducer
    # +process.l1RecoTreeProducer
    # +process.l1MuonRecoTreeProducer
    +process.l1MuonUpgradeTreeProducer
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(250) )

process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(5000)

readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()
process.source = cms.Source ("PoolSource",
                             fileNames = readFiles,
                             # secondaryFileNames = secFiles
                             )

readFiles.extend( [
    'file:///afs/cern.ch/work/j/jlingema/private/scratch0/L1TDev/CMSSW_7_5_0_pre1/src/L1Trigger/L1TMuon/test/l1tmuon_test.root'
] )

# secFiles.extend( [
#     '/store/data/Run2012D/SingleMu25ns/RAW/v1/000/209/151/4607C9E6-BD47-E211-B249-003048D37694.root',
#     '/store/data/Run2012D/SingleMu25ns/RAW/v1/000/209/151/688ADEDC-D547-E211-BFF1-BCAEC518FF80.root',
#     '/store/data/Run2012D/SingleMu25ns/RAW/v1/000/209/151/9C02920D-B447-E211-A14E-003048F024FE.root',
#     '/store/data/Run2012D/SingleMu25ns/RAW/v1/000/209/151/DA62B044-AC47-E211-8298-003048F118C2.root'
# ] )
