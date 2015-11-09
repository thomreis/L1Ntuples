
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
#include "DataFormats/L1TMuon/interface/RegionalMuonCandFwd.h"
// output
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TTree.h"
#include "L1Trigger/L1TMuon/interface/MuonRawDigiTranslator.h"
#include "L1Trigger/L1TMuon/interface/RegionalMuonRawDigiTranslator.h"
#include "L1Trigger/L1TMuon/interface/MicroGMTConfiguration.h"


class L1MuonAnalyzer : public edm::EDAnalyzer {
public:
  explicit L1MuonAnalyzer(const edm::ParameterSet&);
  ~L1MuonAnalyzer();


private:
  virtual void beginJob(void) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();

private:
  void analyzeRegionalMuonCand(const l1t::RegionalMuonCandBxCollection& muons, const std::string& label);
  void analyzeMuon(const l1t::MuonBxCollection& muons, const std::string& label);

  // EDM input tags
  edm::InputTag bmtfTag_;
  edm::InputTag omtfTag_;
  edm::InputTag emtfTag_;
  edm::InputTag ugmtEmulatorTag_;
  edm::InputTag ugmtUnpackerTag_;

  bool analyzeBmtf_;
  bool analyzeOmtf_;
  bool analyzeEmtf_;
  bool analyzeUgmtEmulator_;
  bool analyzeUgmtUnpacker_;
};


L1MuonAnalyzer::L1MuonAnalyzer(const edm::ParameterSet& iConfig) :
  bmtfTag_(iConfig.getParameter<edm::InputTag>("bmtfTag")),
  omtfTag_(iConfig.getParameter<edm::InputTag>("omtfTag")),
  emtfTag_(iConfig.getParameter<edm::InputTag>("emtfTag")),
  ugmtEmulatorTag_(iConfig.getParameter<edm::InputTag>("ugmtEmulatorTag")),
  ugmtUnpackerTag_(iConfig.getParameter<edm::InputTag>("ugmtUnpackerTag")),
  analyzeBmtf_(false),
  analyzeOmtf_(false),
  analyzeEmtf_(false),
  analyzeUgmtEmulator_(false),
  analyzeUgmtUnpacker_(false)
{
  if (bmtfTag_.label() != "none") {
    analyzeBmtf_ = true;
    consumes<l1t::RegionalMuonCandBxCollection>(bmtfTag_);
  }
  if (omtfTag_.label() != "none") {
    analyzeOmtf_ = true;
    consumes<l1t::RegionalMuonCandBxCollection>(omtfTag_);
  }
  if (emtfTag_.label() != "none") {
    analyzeEmtf_ = true;
    consumes<l1t::RegionalMuonCandBxCollection>(emtfTag_);
  }
  if (ugmtEmulatorTag_.label() != "none") {
    analyzeUgmtEmulator_ = true;
    consumes<l1t::MuonBxCollection>(ugmtEmulatorTag_);
  }
  if (ugmtUnpackerTag_.label() != "none") {
    analyzeUgmtUnpacker_ = true;
    consumes<l1t::MuonBxCollection>(ugmtUnpackerTag_);
  }
}


L1MuonAnalyzer::~L1MuonAnalyzer()
{
}


void
L1MuonAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  if (analyzeBmtf_) {
    edm::Handle<l1t::RegionalMuonCandBxCollection> bmtfMuons;
    iEvent.getByLabel(bmtfTag_, bmtfMuons);
    if (bmtfMuons.isValid()) {
      analyzeRegionalMuonCand(*bmtfMuons, "BMTF");
    } else {
      edm::LogError("MissingProduct") << "L1Upgrade GMT BMTF input collection not found" << std::endl;
      return;
    }
  }
  if (analyzeOmtf_) {
    edm::Handle<l1t::RegionalMuonCandBxCollection> omtfMuons;
    iEvent.getByLabel(omtfTag_, omtfMuons);
    if (omtfMuons.isValid()) {
      analyzeRegionalMuonCand(*omtfMuons, "OMTF");
    } else {
      edm::LogError("MissingProduct") << "L1Upgrade GMT OMTF input collection not found" << std::endl;
      return;
    }
  }
  if (analyzeEmtf_) {
    edm::Handle<l1t::RegionalMuonCandBxCollection> emtfMuons;
    iEvent.getByLabel(emtfTag_, emtfMuons);
    if (emtfMuons.isValid()) {
      analyzeRegionalMuonCand(*emtfMuons, "EMTF");
    } else {
      edm::LogError("MissingProduct") << "L1Upgrade GMT EMTF input collection not found" << std::endl;
      return;
    }
  }
  if (analyzeUgmtEmulator_) {
    edm::Handle<l1t::MuonBxCollection> ugmtEmulatorMuons;
    iEvent.getByLabel(ugmtEmulatorTag_, ugmtEmulatorMuons);
    if (ugmtEmulatorMuons.isValid()) {
      analyzeMuon(*ugmtEmulatorMuons, "Emulator");
    } else {
      edm::LogError("MissingProduct") << "L1Upgrade GMT emulator collection not found" << std::endl;
      return;
    }
  }
  if (analyzeUgmtUnpacker_) {
    edm::Handle<l1t::MuonBxCollection> ugmtUnpackerMuons;
    iEvent.getByLabel(ugmtUnpackerTag_, ugmtUnpackerMuons);
    if (ugmtUnpackerMuons.isValid()) {
      analyzeMuon(*ugmtUnpackerMuons, "Unpacker");
    } else {
      edm::LogError("MissingProduct") << "L1Upgrade GMT unpacker collection not found" << std::endl;
      return;
    }
  }
}


void
L1MuonAnalyzer::analyzeRegionalMuonCand(const l1t::RegionalMuonCandBxCollection& muons, const std::string& label)
{
  std::cout << label << ": " << std::endl;
  std::cout << label << " muon bx range: " << muons.getFirstBX() << " " << muons.getLastBX() << std::endl;
  for (int bx = muons.getFirstBX(); bx <= muons.getLastBX(); ++bx) {
    std::cout << "BX " << bx << ": " << muons.size(bx) << " muons" << std::endl;
    unsigned int crt = 0;
    for (auto mu = muons.begin(bx); mu != muons.end(bx) && crt < muons.size(bx); ++mu, ++crt) {
      std::cout << "   hwPt: " << mu->hwPt()
                << "   hwEta: " << mu->hwEta()
                << "   hwPhi: " << mu->hwPhi()
                << "   hwGlobalPhi: " << l1t::MicroGMTConfiguration::calcGlobalPhi(mu->hwPhi(), mu->trackFinderType(), mu->processor())
                << "   hwSign: " << mu->hwSign()
                << "   hwSignValid: " << mu->hwSignValid()
                << "   hwQual: " << mu->hwQual()
                << "   stored dataword: " << hex << mu->dataword() << dec
                << "   dataword: " << hex << l1t::RegionalMuonRawDigiTranslator::generate64bitDataWord(*mu) << dec << std::endl;
    }
  }
}

void
L1MuonAnalyzer::analyzeMuon(const l1t::MuonBxCollection& muons, const std::string& label)
{
  std::cout << label << ": " << std::endl;
  std::cout << label << " muon bx range: " << muons.getFirstBX() << " " << muons.getLastBX() << std::endl;
  for (int bx = muons.getFirstBX(); bx <= muons.getLastBX(); ++bx) {
    std::cout << "BX " << bx << ": " << muons.size(bx) << " muons" << std::endl;
    unsigned int crt = 0;
    for (auto mu = muons.begin(bx); mu != muons.end(bx) && crt < muons.size(bx); ++mu, ++crt) {
      std::cout << "   hwPt: " << mu->hwPt()
                << "   hwEta: " << mu->hwEta()
                << "   hwPhi: " << mu->hwPhi()
                << "   hwCharge: " << mu->hwCharge()
                << "   hwChargeValid: " << mu->hwChargeValid()
                << "   hwQual: " << mu->hwQual()
                << "   hwIso: " << mu->hwIso()
                << "   dataword: " << hex << l1t::MuonRawDigiTranslator::generate64bitDataWord(*mu) << dec << std::endl;
    }
  }
}

// ------------ method called once each job just before starting event loop  ------------
void
L1MuonAnalyzer::beginJob(void)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
L1MuonAnalyzer::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(L1MuonAnalyzer);
