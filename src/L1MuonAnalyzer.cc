
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
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
// output
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TTree.h"
#include "TH2F.h"
#include "L1Trigger/L1TMuon/interface/MuonRawDigiTranslator.h"
#include "L1Trigger/L1TMuon/interface/RegionalMuonRawDigiTranslator.h"
#include "L1Trigger/L1TMuon/interface/MicroGMTConfiguration.h"

class MuonHistos {
public:
  MuonHistos(edm::Service<TFileService> &fs, const std::string& label, const std::string& titlePrefix);
  ~MuonHistos() {};

  TH1I *hMuPerBx_;

  void FillBx(const int bx);
};

MuonHistos::MuonHistos(edm::Service<TFileService> &fs, const std::string& label, const std::string& titlePrefix)
{
  hMuPerBx_ = fs->make<TH1I>(("hMuPerBx"+label).c_str(), (titlePrefix+" muons per BX;BX").c_str(), 15, -7, 8);
}

void
MuonHistos::FillBx(const int bx)
{
  hMuPerBx_->Fill(bx);
}


class TfHistos : public MuonHistos {
public:
  TfHistos(edm::Service<TFileService> &fs, const std::string& label, const std::string& titlePrefix);
  ~TfHistos() {};

  TH1I *hHwPt_;
  TH1I *hHwEta_;
  TH1I *hHwPhi_;
  TH1I *hHwGlobalPhi_;
  TH1I *hHwSign_;
  TH1I *hHwSignValid_;
  TH1I *hHwQual_;
  TH2F *h2EtaPhi_;

  void FillMuon(const l1t::RegionalMuonCand& muon);
};

TfHistos::TfHistos(edm::Service<TFileService> &fs, const std::string& label, const std::string& titlePrefix) : MuonHistos(fs, label, titlePrefix)
{
  hHwPt_ = fs->make<TH1I>(("hHwPt"+label).c_str(), (titlePrefix+" hwPt").c_str(), 300, 0, 300);
  hHwEta_ = fs->make<TH1I>(("hHwEta"+label).c_str(), (titlePrefix+" hwEta").c_str(), 461, -230, 231);
  hHwPhi_ = fs->make<TH1I>(("hHwPhi"+label).c_str(), (titlePrefix+" hwPhi").c_str(), 120, -8, 112);
  hHwGlobalPhi_ = fs->make<TH1I>(("hHwGlobalPhi"+label).c_str(), (titlePrefix+" hwGlobalPhi").c_str(), 580, 0, 580);
  hHwSign_ = fs->make<TH1I>(("hHwSign"+label).c_str(), (titlePrefix+" hwSign").c_str(), 3, -1, 2);
  hHwSignValid_ = fs->make<TH1I>(("hHwSignValid"+label).c_str(), (titlePrefix+" hwSignValid").c_str(), 2, 0, 2);
  hHwQual_ = fs->make<TH1I>(("hHwQual"+label).c_str(), (titlePrefix+" hwQual").c_str(), 16, 0, 16);
  h2EtaPhi_ = fs->make<TH2F>(("h2EtaPhi"+label).c_str(), (titlePrefix+" EtaPhi;#eta;#phi").c_str(), 100, -2.5, 2.5, 128, 0., 6.4);
}

void
TfHistos::FillMuon(const l1t::RegionalMuonCand& muon)
{
  hHwPt_->Fill(muon.hwPt());
  hHwEta_->Fill(muon.hwEta());
  hHwPhi_->Fill(muon.hwPhi());
  int globalPhi = l1t::MicroGMTConfiguration::calcGlobalPhi(muon.hwPhi(), muon.trackFinderType(), muon.processor());
  hHwGlobalPhi_->Fill(globalPhi);
  hHwSign_->Fill(muon.hwSign());
  hHwSignValid_->Fill(muon.hwSignValid());
  hHwQual_->Fill(muon.hwQual());
  h2EtaPhi_->Fill(muon.hwEta()*0.010875, globalPhi*0.010908);
}


class UGmtHistos : public MuonHistos {
public:
  UGmtHistos(edm::Service<TFileService> &fs, const std::string& label, const std::string& titlePrefix);
  ~UGmtHistos() {};

  TH1I *hHwPt_;
  TH1I *hHwEta_;
  TH1I *hHwPhi_;
  TH1I *hHwCharge_;
  TH1I *hHwChargeValid_;
  TH1I *hHwQual_;
  TH1I *hHwIso_;
  TH2F *h2EtaPhi_;

  void FillMuon(const l1t::Muon& muon);
};

UGmtHistos::UGmtHistos(edm::Service<TFileService> &fs, const std::string& label, const std::string& titlePrefix) : MuonHistos(fs, label, titlePrefix)
{
  hHwPt_ = fs->make<TH1I>(("hHwPt"+label).c_str(), (titlePrefix+" hwPt").c_str(), 300, 0, 300);
  hHwEta_ = fs->make<TH1I>(("hHwEta"+label).c_str(), (titlePrefix+" hwEta").c_str(), 461, -230, 231);
  hHwPhi_ = fs->make<TH1I>(("hHwPhi"+label).c_str(), (titlePrefix+" hwPhi").c_str(), 580, 0, 580);
  hHwCharge_ = fs->make<TH1I>(("hHwCharge"+label).c_str(), (titlePrefix+" hwCharge").c_str(), 3, -1, 2);
  hHwChargeValid_ = fs->make<TH1I>(("hHwChargeValid"+label).c_str(), (titlePrefix+" hwChargeValid").c_str(), 2, 0, 2);
  hHwQual_ = fs->make<TH1I>(("hHwQual"+label).c_str(), (titlePrefix+" hwQual").c_str(), 16, 0, 16);
  hHwIso_ = fs->make<TH1I>(("hHwIso"+label).c_str(), (titlePrefix+" hwIso").c_str(), 4, 0, 4);
  h2EtaPhi_ = fs->make<TH2F>(("h2EtaPhi"+label).c_str(), (titlePrefix+" EtaPhi;#eta;#phi").c_str(), 100, -2.5, 2.5, 128, 0., 6.4);
}

void
UGmtHistos::FillMuon(const l1t::Muon& muon)
{
  hHwPt_->Fill(muon.hwPt());
  hHwEta_->Fill(muon.hwEta());
  hHwPhi_->Fill(muon.hwPhi());
  hHwCharge_->Fill(muon.hwCharge());
  hHwChargeValid_->Fill(muon.hwChargeValid());
  hHwQual_->Fill(muon.hwQual());
  hHwIso_->Fill(muon.hwIso());
  h2EtaPhi_->Fill(muon.hwEta()*0.010875, muon.hwPhi()*0.010908);
}


class L1MuonAnalyzer : public edm::EDAnalyzer {
public:
  explicit L1MuonAnalyzer(const edm::ParameterSet&);
  ~L1MuonAnalyzer();


private:
  virtual void beginJob(void) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();

private:
  void analyzeRegionalMuonCand(const l1t::RegionalMuonCandBxCollection& muons, TfHistos& histos, const std::string& label);
  void analyzeMuon(const l1t::MuonBxCollection& muons, UGmtHistos& histos, const std::string& label);

  // EDM input tags
  edm::InputTag bmtfTag_;
  edm::InputTag omtfTag_;
  edm::InputTag emtfTag_;
  edm::InputTag ugmtEmulatorTag_;
  edm::InputTag ugmtUnpackerTag_;
  edm::InputTag gmtTag_;

  bool debug_;

  bool suppressCommas_;

  bool analyzeBmtf_;
  bool analyzeOmtf_;
  bool analyzeEmtf_;
  bool analyzeUgmtEmulator_;
  bool analyzeUgmtUnpacker_;
  bool analyzeGmt_;

  // output file
  edm::Service<TFileService> fs_;

  TfHistos bmtfHistos_;
  TfHistos omtfHistos_;
  TfHistos emtfHistos_;
  UGmtHistos emulatorHistos_;
  UGmtHistos unpackerHistos_;

  TH1I *hMuSizeDiff_;
  TH1I *hMuPtDiff_;
  TH1I *hMuEtaDiff_;
  TH1I *hMuPhiDiff_;
  TH1I *hMuChargeDiff_;
  TH1I *hMuChargeValidDiff_;
  TH1I *hMuQualDiff_;
  TH1I *hMuIsoDiff_;

  TH1I *hGmtEvtBx_;
  TH2F *h2GmtEtaPhi_;
};


L1MuonAnalyzer::L1MuonAnalyzer(const edm::ParameterSet& iConfig) :
  bmtfTag_(iConfig.getParameter<edm::InputTag>("bmtfTag")),
  omtfTag_(iConfig.getParameter<edm::InputTag>("omtfTag")),
  emtfTag_(iConfig.getParameter<edm::InputTag>("emtfTag")),
  ugmtEmulatorTag_(iConfig.getParameter<edm::InputTag>("ugmtEmulatorTag")),
  ugmtUnpackerTag_(iConfig.getParameter<edm::InputTag>("ugmtUnpackerTag")),
  gmtTag_(iConfig.getParameter<edm::InputTag>("gmtTag")),
  debug_(iConfig.getUntrackedParameter<bool>("debug", false)),
  suppressCommas_(iConfig.getUntrackedParameter<bool>("commaSuppression", false)),
  analyzeBmtf_(false),
  analyzeOmtf_(false),
  analyzeEmtf_(false),
  analyzeUgmtEmulator_(false),
  analyzeUgmtUnpacker_(false),
  analyzeGmt_(false),
  bmtfHistos_(fs_, "Bmtf", "BMTF"),
  omtfHistos_(fs_, "Omtf", "OMTF"),
  emtfHistos_(fs_, "Emtf", "EMTF"),
  emulatorHistos_(fs_, "Emulator", "Emulator uGMT"),
  unpackerHistos_(fs_, "Unpacker", "Unpacked uGMT")
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
  if (gmtTag_.label() != "none") {
    analyzeGmt_ = true;
    consumes<L1MuGMTReadoutCollection>(gmtTag_);
  }

  hMuSizeDiff_ = fs_->make<TH1I>("hMuSizeDiff", "Emulator muon size - unpacker muon size for BX 0", 17, -8, 9);
  hMuPtDiff_ = fs_->make<TH1I>("hMuPtDiff", "Emulator muon pT - unpacker muon pt for BX 0", 521, -260, 261);
  hMuEtaDiff_ = fs_->make<TH1I>("hMuEtaDiff", "Emulator muon #eta - unpacker muon #eta for BX 0", 461, -230, 231);
  hMuPhiDiff_ = fs_->make<TH1I>("hMuPhiDiff", "Emulator muon #phi - unpacker muon #phi for BX 0", 581, -290, 291);
  hMuChargeDiff_ = fs_->make<TH1I>("hMuChargeDiff", "Emulator muon charge - unpacker muon charge for BX 0", 5, -2, 3);
  hMuChargeValidDiff_ = fs_->make<TH1I>("hMuChargeValidDiff", "Emulator muon chargeValid - unpacker muon chargeValid for BX 0", 3, -1, 2);
  hMuQualDiff_ = fs_->make<TH1I>("hMuQualDiff", "Emulator muon qual - unpacker muon qual for BX 0", 31, -15, 16);
  hMuIsoDiff_ = fs_->make<TH1I>("hMuIsoDiff", "Emulator muon iso - unpacker muon iso for BX 0", 7, -3, 4);

  hGmtEvtBx_ = fs_->make<TH1I>("hGmtEvtBx", "GMT event BX", 370, 0, 3700);
  h2GmtEtaPhi_ = fs_->make<TH2F>("h2GmtEtaPhi", "GMT eta phi map;#eta;#phi", 100, -2.5, 2.5, 128, 0., 6.4);
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
      analyzeRegionalMuonCand(*bmtfMuons, bmtfHistos_, "BMTF");
    } else {
      edm::LogError("MissingProduct") << "L1Upgrade GMT BMTF input collection not found" << std::endl;
      return;
    }
  }
  if (analyzeOmtf_) {
    edm::Handle<l1t::RegionalMuonCandBxCollection> omtfMuons;
    iEvent.getByLabel(omtfTag_, omtfMuons);
    if (omtfMuons.isValid()) {
      analyzeRegionalMuonCand(*omtfMuons, omtfHistos_, "OMTF");
    } else {
      edm::LogError("MissingProduct") << "L1Upgrade GMT OMTF input collection not found" << std::endl;
      return;
    }
  }
  if (analyzeEmtf_) {
    edm::Handle<l1t::RegionalMuonCandBxCollection> emtfMuons;
    iEvent.getByLabel(emtfTag_, emtfMuons);
    if (emtfMuons.isValid()) {
      analyzeRegionalMuonCand(*emtfMuons, emtfHistos_, "EMTF");
    } else {
      edm::LogError("MissingProduct") << "L1Upgrade GMT EMTF input collection not found" << std::endl;
      return;
    }
  }
  if (analyzeUgmtEmulator_) {
    edm::Handle<l1t::MuonBxCollection> ugmtEmulatorMuons;
    iEvent.getByLabel(ugmtEmulatorTag_, ugmtEmulatorMuons);
    if (ugmtEmulatorMuons.isValid()) {
      analyzeMuon(*ugmtEmulatorMuons, emulatorHistos_, "Emulator");
    } else {
      edm::LogError("MissingProduct") << "L1Upgrade GMT emulator collection not found" << std::endl;
      return;
    }
  }
  if (analyzeUgmtUnpacker_) {
    edm::Handle<l1t::MuonBxCollection> ugmtUnpackerMuons;
    iEvent.getByLabel(ugmtUnpackerTag_, ugmtUnpackerMuons);
    if (ugmtUnpackerMuons.isValid()) {
      analyzeMuon(*ugmtUnpackerMuons, unpackerHistos_, "Unpacker");
    } else {
      edm::LogError("MissingProduct") << "L1Upgrade GMT unpacker collection not found" << std::endl;
      return;
    }
  }
  if (analyzeGmt_) {
    edm::Handle<L1MuGMTReadoutCollection> gmtReadoutColl;
    iEvent.getByLabel(gmtTag_, gmtReadoutColl);
    if (gmtReadoutColl.isValid()) {
      L1MuGMTReadoutCollection const* gmtrc = gmtReadoutColl.product();
      std::vector<L1MuGMTReadoutRecord> gmt_records = gmtrc->getRecords();
      std::vector<L1MuGMTReadoutRecord>::const_iterator igmtrr;
      for(igmtrr=gmt_records.begin(); igmtrr!=gmt_records.end(); igmtrr++) {

        if(igmtrr->getBxInEvent()==0) {
          hGmtEvtBx_->Fill(igmtrr->getBxNr());
        }

        std::vector<L1MuGMTExtendedCand>::const_iterator gmt_iter;
        std::vector<L1MuGMTExtendedCand> exc = igmtrr->getGMTCands();
        unsigned int n = 0;
        for(gmt_iter=exc.begin(); gmt_iter!=exc.end(); gmt_iter++) {
          if (n < 12 && !(*gmt_iter).empty()) {
            h2GmtEtaPhi_->Fill((*gmt_iter).etaValue(), (*gmt_iter).phiValue());
            ++n;
          }
        }
      }
    } else {
      edm::LogError("MissingProduct") << "GMT GMT readout collection not found" << std::endl;
      return;
    }
  }

  // emulator - unpacker comparisons
  if (analyzeUgmtEmulator_ && analyzeUgmtUnpacker_) {
    edm::Handle<l1t::MuonBxCollection> ugmtEmulatorMuons;
    edm::Handle<l1t::MuonBxCollection> ugmtUnpackerMuons;
    iEvent.getByLabel(ugmtEmulatorTag_, ugmtEmulatorMuons);
    iEvent.getByLabel(ugmtUnpackerTag_, ugmtUnpackerMuons);
    if (ugmtEmulatorMuons.isValid() && ugmtUnpackerMuons.isValid()) {
      hMuSizeDiff_->Fill(ugmtEmulatorMuons->size(0) - ugmtUnpackerMuons->size(0));
      auto muEmu = ugmtEmulatorMuons->begin(0);
      auto muUnp = ugmtUnpackerMuons->begin(0);
      unsigned int ctr = 0;
      for (; muEmu != ugmtEmulatorMuons->end(0) && muUnp != ugmtUnpackerMuons->end(0) && ctr < ugmtEmulatorMuons->size(0) && ctr < ugmtUnpackerMuons->size(0); ++muEmu, ++muUnp, ++ctr) {
        hMuPtDiff_->Fill(muEmu->hwPt() - muUnp->hwPt());
        hMuEtaDiff_->Fill(muEmu->hwEta() - muUnp->hwEta());
        hMuPhiDiff_->Fill(muEmu->hwPhi() - muUnp->hwPhi());
        hMuChargeDiff_->Fill(muEmu->hwCharge() - muUnp->hwCharge());
        hMuChargeValidDiff_->Fill(muEmu->hwChargeValid() - muUnp->hwChargeValid());
        hMuQualDiff_->Fill(muEmu->hwQual() - muUnp->hwQual());
        hMuIsoDiff_->Fill(muEmu->hwIso() - muUnp->hwIso());
      }
    }
  }
}


void
L1MuonAnalyzer::analyzeRegionalMuonCand(const l1t::RegionalMuonCandBxCollection& muons, TfHistos& histos, const std::string& label)
{
  if (debug_) {
    std::cout << "*** " << label << " muon bx range: " << muons.getFirstBX() << " " << muons.getLastBX() << std::endl;
  }
  for (int bx = muons.getFirstBX(); bx <= muons.getLastBX(); ++bx) {
    if (debug_ && muons.size(bx) > 0) std::cout << "    BX " << bx << ": " << muons.size(bx) << " muons" << std::endl;
    unsigned int ctr = 0;
    for (auto mu = muons.begin(bx); mu != muons.end(bx) && ctr < muons.size(bx); ++mu, ++ctr) {
      // suppress comma events
      if (suppressCommas_ && ((mu->dataword() & 0xFFFFFFFF) == 0x505050bc || (mu->dataword() >> 32) == 0x505050bc)) {
        if (debug_) {
          std::cout << "       COMMA   ";
        }
      } else {
        histos.FillBx(bx);
        histos.FillMuon(*mu);
        if (debug_) {
          std::cout << "       ";
        }
      }

      if (debug_) {
        std::cout << "hwPt: " << mu->hwPt()
                  << "   hwEta: " << mu->hwEta()
                  << "   hwPhi: " << mu->hwPhi()
                  << "   hwGlobalPhi: " << l1t::MicroGMTConfiguration::calcGlobalPhi(mu->hwPhi(), mu->trackFinderType(), mu->processor())
                  << "   hwSign: " << mu->hwSign()
                  << "   hwSignValid: " << mu->hwSignValid()
                  << "   hwQual: " << mu->hwQual()
                  << "   stored dataword: 0x" << hex << setw(16) << setfill('0') << mu->dataword() << dec
                  << "   dataword: 0x" << hex << setw(16) << setfill('0') << l1t::RegionalMuonRawDigiTranslator::generate64bitDataWord(*mu) << dec << std::endl;
      }
    }
  }
}

void
L1MuonAnalyzer::analyzeMuon(const l1t::MuonBxCollection& muons, UGmtHistos& histos, const std::string& label)
{
  if (debug_) {
    std::cout << "*** " << label << " muon bx range: " << muons.getFirstBX() << " " << muons.getLastBX() << std::endl;
  }
  for (int bx = muons.getFirstBX(); bx <= muons.getLastBX(); ++bx) {
    if (debug_ && muons.size(bx) > 0) std::cout << "    BX " << bx << ": " << muons.size(bx) << " muons" << std::endl;
    unsigned int ctr = 0;
    for (auto mu = muons.begin(bx); mu != muons.end(bx) && ctr < muons.size(bx); ++mu, ++ctr) {
      histos.FillBx(bx);
      histos.FillMuon(*mu);

      if (debug_) {
        std::cout << "       hwPt: " << mu->hwPt()
                  << "   hwEta: " << mu->hwEta()
                  << "   hwPhi: " << mu->hwPhi()
                  << "   hwCharge: " << mu->hwCharge()
                  << "   hwChargeValid: " << mu->hwChargeValid()
                  << "   hwQual: " << mu->hwQual()
                  << "   hwIso: " << mu->hwIso()
                  << "   dataword: 0x" << hex << setw(16) << setfill('0') << l1t::MuonRawDigiTranslator::generate64bitDataWord(*mu) << dec << std::endl;
      }
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
