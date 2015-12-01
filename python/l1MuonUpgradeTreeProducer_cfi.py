import FWCore.ParameterSet.Config as cms

l1MuonUpgradeTreeProducer = cms.EDAnalyzer("L1MuonUpgradeTreeProducer",
  ugmtTag = cms.InputTag("simGmtDigis"),
  bmtfTag = cms.InputTag("simBmtfDigis", "BMTF"),
  omtfTag = cms.InputTag("simOmtfDigis", "OMTF"),
  emtfTag = cms.InputTag("simEmtfDigis", "EMTF")
)
