import FWCore.ParameterSet.Config as cms

l1MuonUpgradeTreeProducer = cms.EDAnalyzer("L1MuonUpgradeTreeProducer",
  ugmtTag = cms.InputTag("microGMTEmulator"),
  bmtfTag = cms.InputTag("uGMTInputProducer", "BarrelTFMuons"),
  omtfTag = cms.InputTag("omtfEmulator", "OMTF"),
  emtfTag = cms.InputTag("uGMTInputProducer", "ForwardTFMuons")
)
