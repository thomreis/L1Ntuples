
#include <memory>
// framework
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "L1TriggerDPG/L1Ntuples/interface/L1AnalysisUGMT.h"
#include "L1TriggerDPG/L1Ntuples/interface/L1AnalysisUGMTDataFormat.h"

#include "DataFormats/L1Trigger/interface/Muon.h"
#include "DataFormats/L1TMuon/interface/L1TRegionalMuonCandidateFwd.h"

// output
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TTree.h"


class L1MuonUpgradeTreeProducer : public edm::EDAnalyzer {
public:
  explicit L1MuonUpgradeTreeProducer(const edm::ParameterSet&);
  ~L1MuonUpgradeTreeProducer();


private:
  virtual void beginJob(void) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();

public:
  L1Analysis::L1AnalysisUGMT ugmt;
  L1Analysis::L1AnalysisUGMTDataFormat* ugmtData;

private:
  // output file
  edm::Service<TFileService> fs_;

  // tree
  TTree * tree_;

  // EDM input tags
  edm::InputTag bmtfTag_;
  edm::InputTag omtfTag_;
  edm::InputTag emtfTag_;
  edm::InputTag ugmtTag_;
};


L1MuonUpgradeTreeProducer::L1MuonUpgradeTreeProducer(const edm::ParameterSet& iConfig) :
  ugmt(),
  bmtfTag_(iConfig.getParameter<edm::InputTag>("bmtfTag")),
  omtfTag_(iConfig.getParameter<edm::InputTag>("omtfTag")),
  emtfTag_(iConfig.getParameter<edm::InputTag>("emtfTag")),
  ugmtTag_(iConfig.getParameter<edm::InputTag>("ugmtTag"))
{
  ugmtData = ugmt.getData();
  tree_ = fs_->make<TTree>("L1MuonUpgradeTree", "L1MuonUpgradeTree");
  tree_->Branch("L1TMuon", "L1Analysis::L1AnalysisUGMTDataFormat", &ugmtData, 32000, 3);
}


L1MuonUpgradeTreeProducer::~L1MuonUpgradeTreeProducer()
{
}


void
L1MuonUpgradeTreeProducer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  ugmt.Reset();

  edm::Handle<l1t::L1TRegionalMuonCandidateCollection> bmtfMuons;
  edm::Handle<l1t::L1TRegionalMuonCandidateCollection> emtfMuons;
  edm::Handle<l1t::L1TRegionalMuonCandidateCollection> omtfMuons;
  edm::Handle<l1t::MuonBxCollection> ugmtMuons;


  iEvent.getByLabel(bmtfTag_, bmtfMuons);
  iEvent.getByLabel(emtfTag_, emtfMuons);
  iEvent.getByLabel(omtfTag_, omtfMuons);
  iEvent.getByLabel(ugmtTag_, ugmtMuons);
  // iEvent.getByLabel(m_trigTowerTag, trigTowers);
  if (bmtfMuons.isValid() && emtfMuons.isValid() && omtfMuons.isValid() && ugmtMuons.isValid()) {
    ugmt.Set(*ugmtMuons, *bmtfMuons, *omtfMuons, *emtfMuons, true);
  } else {
    edm::LogWarning("MissingProduct") << "L1Upgrade GMT inputs and output not found" << std::endl;
    return;
  }
  tree_->Fill();
}


// ------------ method called once each job just before starting event loop  ------------
void
L1MuonUpgradeTreeProducer::beginJob(void)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
L1MuonUpgradeTreeProducer::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(L1MuonUpgradeTreeProducer);
