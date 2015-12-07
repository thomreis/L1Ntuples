
#include <memory>
// framework
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <DataFormats/FEDRawData/interface/FEDRawDataCollection.h>
#include <DataFormats/FEDRawData/interface/FEDHeader.h>
#include <DataFormats/FEDRawData/interface/FEDNumbering.h>

#include "L1TriggerDPG/L1Ntuples/interface/L1AnalysisUGMT.h"
#include "L1TriggerDPG/L1Ntuples/interface/L1AnalysisUGMTDataFormat.h"

#include "DataFormats/L1Trigger/interface/Muon.h"
#include "DataFormats/L1TMuon/interface/RegionalMuonCand.h"
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

  int selBx_;

  TH1I *hMuSize_;
  TH2I *h2MuSize_;
  TH1I *hMuPerBx_;
  TH1I *hMuPerBxId_;

  void FillBx(const int bx, const int bxId);
  void FillSize(const int size, const int bx);

protected:
  float ptScale_;
  float etaScale_;
  float phiScale_;
};

MuonHistos::MuonHistos(edm::Service<TFileService> &fs, const std::string& label, const std::string& titlePrefix) : selBx_(0), ptScale_(0.5), etaScale_(0.010875), phiScale_(0.010908)
{
  const std::string selBxStr = std::to_string(selBx_);
  hMuSize_ = fs->make<TH1I>(("hMuSize"+label).c_str(), (titlePrefix+" muons in BX"+selBxStr+";Number of muons").c_str(), 10, 0, 10);
  h2MuSize_ = fs->make<TH2I>(("h2MuSize"+label).c_str(), (titlePrefix+" muons per BX;Number of muons;BX").c_str(), 9, 1, 10, 15, -7, 8);
  hMuPerBx_ = fs->make<TH1I>(("hMuPerBx"+label).c_str(), (titlePrefix+" muons per BX;BX").c_str(), 15, -7, 8);
  hMuPerBxId_ = fs->make<TH1I>(("hMuPerBxId"+label).c_str(), (titlePrefix+" muons per BX ID;BX ID").c_str(), 3700, 0, 3700);
}

void
MuonHistos::FillBx(const int bx, const int bxId)
{
  hMuPerBx_->Fill(bx);
  hMuPerBxId_->Fill(bxId);
}

void
MuonHistos::FillSize(const int size, const int bx)
{
  if (bx == selBx_) {
    hMuSize_->Fill(size);
  }
  h2MuSize_->Fill(size, bx);
}


class TfHistos : public MuonHistos {
public:
  TfHistos(edm::Service<TFileService> &fs, const std::string& label, const std::string& titlePrefix);
  ~TfHistos() {};

  TH2I *h2HwPtVsProc_;
  TH2I *h2HwEtaVsProc_;
  TH2I *h2HwPhiVsProc_;
  TH2I *h2HwPtVsProcNeg_;
  TH2I *h2HwEtaVsProcNeg_;
  TH2I *h2HwPhiVsProcNeg_;
  TH2I *h2HwPtVsProcPos_;
  TH2I *h2HwEtaVsProcPos_;
  TH2I *h2HwPhiVsProcPos_;

  TH2I *h2ProcId_;
  TH2I *h2ProcIdNeg_;
  TH2I *h2ProcIdPos_;
  TH2I *h2LinkId_;
  TH2I *h2HwPt_;
  TH2I *h2HwEta_;
  TH2I *h2HwPhi_;
  TH2I *h2HwGlobalPhi_;
  TH2I *h2HwGlobalPhiNeg_;
  TH2I *h2HwGlobalPhiPos_;
  TH2I *h2HwSign_;
  TH2I *h2HwSignValid_;
  TH2I *h2HwQual_;
  TH2I *h2TfType_;
  TH2F *h2Pt_;
  TH2F *h2Eta_;
  TH2F *h2GlobalPhi_;
  TH2F *h2GlobalPhiNeg_;
  TH2F *h2GlobalPhiPos_;

  TH1I *hHwPt_;
  TH1I *hHwEta_;
  TH1I *hHwPhi_;
  TH1I *hHwGlobalPhi_;
  TH1I *hHwGlobalPhiNeg_;
  TH1I *hHwGlobalPhiPos_;
  TH1I *hHwSign_;
  TH1I *hHwSignValid_;
  TH1I *hHwQual_;
  TH1I *hTfType_;
  TH1I *hMuPerBxNeg_;
  TH1I *hMuPerBxPos_;
  TH1F *hPt_;
  TH1F *hEta_;
  TH1F *hGlobalPhi_;
  TH1F *hGlobalPhiNeg_;
  TH1F *hGlobalPhiPos_;

  TH2F *h2EtaPhi_;

  void FillMuon(const l1t::RegionalMuonCand& muon, const int bx);
};

TfHistos::TfHistos(edm::Service<TFileService> &fs, const std::string& label, const std::string& titlePrefix) : MuonHistos(fs, label, titlePrefix)
{
  h2HwPtVsProc_ = fs->make<TH2I>(("h2HwPtVsProc"+label).c_str(), (titlePrefix+" hwPt per Processor ID;hwPt;Processor ID (EMTF-/OMTF-: ID + 6)").c_str(), 150, 0, 300, 12, 0, 12);
  h2HwEtaVsProc_ = fs->make<TH2I>(("h2HwEtaVsProc"+label).c_str(), (titlePrefix+" hwEta per Processor ID;hwEta;Processor ID (EMTF-/OMTF-: ID + 6)").c_str(), 231, -230, 232, 12, 0, 12);
  h2HwPhiVsProc_ = fs->make<TH2I>(("h2HwPhiVsProc"+label).c_str(), (titlePrefix+" hwPhi per Processor ID;hwPhi;Processor ID (EMTF-/OMTF-: ID + 6)").c_str(), 120, -8, 112, 12, 0, 12);
  h2HwPtVsProcNeg_ = fs->make<TH2I>(("h2HwPtVsProcNeg"+label).c_str(), (titlePrefix+" hwPt per Processor ID negative side;hwPt;Processor ID").c_str(), 150, 0, 300, 6, 0, 6);
  h2HwEtaVsProcNeg_ = fs->make<TH2I>(("h2HwEtaVsProcNeg"+label).c_str(), (titlePrefix+" hwEta per Processor ID negative side;hwEta;Processor ID").c_str(), 231, -230, 232, 6, 0, 6);
  h2HwPhiVsProcNeg_ = fs->make<TH2I>(("h2HwPhiVsProcNeg"+label).c_str(), (titlePrefix+" hwPhi per Processor ID negative side;hwPhi;Processor ID").c_str(), 120, -8, 112, 6, 0, 6);
  h2HwPtVsProcPos_ = fs->make<TH2I>(("h2HwPtVsProcPos"+label).c_str(), (titlePrefix+" hwPt per Processor ID positive side;hwPt;Processor ID").c_str(), 150, 0, 300, 6, 0, 6);
  h2HwEtaVsProcPos_ = fs->make<TH2I>(("h2HwEtaVsProcPos"+label).c_str(), (titlePrefix+" hwEta per Processor ID positive side;hwEta;Processor ID").c_str(), 231, -230, 232, 6, 0, 6);
  h2HwPhiVsProcPos_ = fs->make<TH2I>(("h2HwPhiVsProcPos"+label).c_str(), (titlePrefix+" hwPhi per Processor ID positive side;hwPhi;Processor ID").c_str(), 120, -8, 112, 6, 0, 6);

  h2ProcId_ = fs->make<TH2I>(("h2ProcId"+label).c_str(), (titlePrefix+" Processer ID per BX;Processor ID;BX").c_str(), 12, 0, 12, 15, -7, 8);
  h2ProcIdNeg_ = fs->make<TH2I>(("h2ProcIdNeg"+label).c_str(), (titlePrefix+" Processer ID per BX negative side;Processor ID;BX").c_str(), 6, 0, 6, 15, -7, 8);
  h2ProcIdPos_ = fs->make<TH2I>(("h2ProcIdPos"+label).c_str(), (titlePrefix+" Processer ID per BX positive side;Processor ID;BX").c_str(), 6, 0, 6, 15, -7, 8);
  h2LinkId_ = fs->make<TH2I>(("h2LinkId"+label).c_str(), (titlePrefix+" Link ID per BX;Link ID;BX").c_str(), 36, 36, 72, 15, -7, 8);
  h2HwPt_ = fs->make<TH2I>(("h2HwPt"+label).c_str(), (titlePrefix+" hwPt per BX;hwPt;BX").c_str(), 150, 0, 300, 15, -7, 8);
  h2HwEta_ = fs->make<TH2I>(("h2HwEta"+label).c_str(), (titlePrefix+" hwEta per BX;hwEta;BX").c_str(), 231, -230, 232, 15, -7, 8);
  h2HwPhi_ = fs->make<TH2I>(("h2HwPhi"+label).c_str(), (titlePrefix+" hwPhi per BX;hwPhi;BX").c_str(), 120, -8, 112, 15, -7, 8);
  h2HwGlobalPhi_ = fs->make<TH2I>(("h2HwGlobalPhi"+label).c_str(), (titlePrefix+" hwGlobalPhi per BX;global #phi;BX").c_str(), 145, 0, 580, 15, -7, 8);
  h2HwGlobalPhiNeg_ = fs->make<TH2I>(("h2HwGlobalPhiNeg"+label).c_str(), (titlePrefix+" hwGlobalPhi per BX negative side;global #phi;BX").c_str(), 145, 0, 580, 15, -7, 8);
  h2HwGlobalPhiPos_ = fs->make<TH2I>(("h2HwGlobalPhiPos"+label).c_str(), (titlePrefix+" hwGlobalPhi per BX positive side;global #phi;BX").c_str(), 145, 0, 580, 15, -7, 8);
  h2HwSign_ = fs->make<TH2I>(("h2HwSign"+label).c_str(), (titlePrefix+" hwSign per BX;Sign;BX").c_str(), 3, -1, 2, 15, -7, 8);
  h2HwSignValid_ = fs->make<TH2I>(("h2HwSignValid"+label).c_str(), (titlePrefix+" hwSignValid per BX;Sign valid;BX").c_str(), 2, 0, 2, 15, -7, 8);
  h2HwQual_ = fs->make<TH2I>(("h2HwQual"+label).c_str(), (titlePrefix+" hwQual per BX;Quality;BX").c_str(), 16, 0, 16, 15, -7, 8);
  h2TfType_ = fs->make<TH2I>(("h2TfType"+label).c_str(), (titlePrefix+" tfType per BX;TF type;BX").c_str(), 5, 0, 5, 15, -7, 8);
  h2Pt_ = fs->make<TH2F>(("h2Pt"+label).c_str(), (titlePrefix+" Pt per BX;GeV;BX").c_str(), 150, 0, 150, 15, -7, 8);
  h2Eta_ = fs->make<TH2F>(("h2Eta"+label).c_str(), (titlePrefix+" Eta per BX;#eta;BX").c_str(), 100, -2.5, 2.5, 15, -7, 8);
  h2GlobalPhi_ = fs->make<TH2F>(("h2GlobalPhi"+label).c_str(), (titlePrefix+" GlobalPhi per BX;#phi;BX").c_str(), 128, 0., 6.4, 15, -7, 8);
  h2GlobalPhiNeg_ = fs->make<TH2F>(("h2GlobalPhiNeg"+label).c_str(), (titlePrefix+" GlobalPhi per BX negative side;#phi;BX").c_str(), 128, 0., 6.4, 15, -7, 8);
  h2GlobalPhiPos_ = fs->make<TH2F>(("h2GlobalPhiPos"+label).c_str(), (titlePrefix+" GlobalPhi per BX positive side;#phi;BX").c_str(), 128, 0., 6.4, 15, -7, 8);

  hHwPt_ = fs->make<TH1I>(("hHwPt"+label).c_str(), (titlePrefix+" hwPt").c_str(), 300, 0, 300);
  hHwEta_ = fs->make<TH1I>(("hHwEta"+label).c_str(), (titlePrefix+" hwEta").c_str(), 462, -230, 232);
  hHwPhi_ = fs->make<TH1I>(("hHwPhi"+label).c_str(), (titlePrefix+" hwPhi").c_str(), 120, -8, 112);
  hHwGlobalPhi_ = fs->make<TH1I>(("hHwGlobalPhi"+label).c_str(), (titlePrefix+" hwGlobalPhi").c_str(), 580, 0, 580);
  hHwGlobalPhiNeg_ = fs->make<TH1I>(("hHwGlobalPhiNeg"+label).c_str(), (titlePrefix+" hwGlobalPhiNeg").c_str(), 580, 0, 580);
  hHwGlobalPhiPos_ = fs->make<TH1I>(("hHwGlobalPhiPos"+label).c_str(), (titlePrefix+" hwGlobalPhiPos").c_str(), 580, 0, 580);
  hHwSign_ = fs->make<TH1I>(("hHwSign"+label).c_str(), (titlePrefix+" hwSign").c_str(), 3, -1, 2);
  hHwSignValid_ = fs->make<TH1I>(("hHwSignValid"+label).c_str(), (titlePrefix+" hwSignValid").c_str(), 2, 0, 2);
  hHwQual_ = fs->make<TH1I>(("hHwQual"+label).c_str(), (titlePrefix+" hwQual").c_str(), 16, 0, 16);
  hTfType_ = fs->make<TH1I>(("hTfType"+label).c_str(), (titlePrefix+" hTfType").c_str(), 5, 0, 5);
  hMuPerBxNeg_ = fs->make<TH1I>(("hMuPerBxNeg"+label).c_str(), (titlePrefix+" muons per BX negative side;BX").c_str(), 15, -7, 8);
  hMuPerBxPos_ = fs->make<TH1I>(("hMuPerBxPos"+label).c_str(), (titlePrefix+" muons per BX positive side;BX").c_str(), 15, -7, 8);
  hPt_ = fs->make<TH1F>(("hPt"+label).c_str(), (titlePrefix+" Pt;GeV").c_str(), 150, 0, 150);
  hEta_ = fs->make<TH1F>(("hEta"+label).c_str(), (titlePrefix+" Eta;#eta").c_str(), 100, -2.5, 2.5);
  hGlobalPhi_ = fs->make<TH1F>(("hGlobalPhi"+label).c_str(), (titlePrefix+" GlobalPhi;#phi").c_str(), 128, 0., 6.4);
  hGlobalPhiNeg_ = fs->make<TH1F>(("hGlobalPhiNeg"+label).c_str(), (titlePrefix+" GlobalPhi negative side;#phi").c_str(), 128, 0., 6.4);
  hGlobalPhiPos_ = fs->make<TH1F>(("hGlobalPhiPos"+label).c_str(), (titlePrefix+" GlobalPhi positive side;#phi").c_str(), 128, 0., 6.4);

  h2EtaPhi_ = fs->make<TH2F>(("h2EtaPhi"+label).c_str(), (titlePrefix+" EtaPhi;#eta;#phi").c_str(), 100, -2.5, 2.5, 128, 0., 6.4);
}

void
TfHistos::FillMuon(const l1t::RegionalMuonCand& muon, const int bx)
{
  l1t::tftype tfType = muon.trackFinderType();
  int proc = muon.processor();
  int link = muon.link();
  int globalPhi = l1t::MicroGMTConfiguration::calcGlobalPhi(muon.hwPhi(), tfType, proc);

  h2ProcId_->Fill(proc, bx);
  h2LinkId_->Fill(link, bx);
  h2HwPt_->Fill(muon.hwPt(), bx);
  h2HwEta_->Fill(muon.hwEta(), bx);
  h2HwPhi_->Fill(muon.hwPhi(), bx);
  h2HwGlobalPhi_->Fill(globalPhi, bx);
  h2HwSign_->Fill(muon.hwSign(), bx);
  h2HwSignValid_->Fill(muon.hwSignValid(), bx);
  h2HwQual_->Fill(muon.hwQual(), bx);
  h2TfType_->Fill(tfType, bx);
  h2Pt_->Fill(muon.hwPt() * ptScale_, bx);
  h2Eta_->Fill(muon.hwEta() * etaScale_, bx);
  h2GlobalPhi_->Fill(globalPhi * phiScale_, bx);

  hHwPt_->Fill(muon.hwPt());
  hHwEta_->Fill(muon.hwEta());
  hHwPhi_->Fill(muon.hwPhi());
  hHwGlobalPhi_->Fill(globalPhi);
  hHwSign_->Fill(muon.hwSign());
  hHwSignValid_->Fill(muon.hwSignValid());
  hHwQual_->Fill(muon.hwQual());
  hTfType_->Fill((int)tfType);
  int fillProc = proc;
  if (tfType == l1t::tftype::omtf_neg || tfType == l1t::tftype::emtf_neg) {
    fillProc += 6;
    h2HwPtVsProcNeg_->Fill(muon.hwPt(), proc);
    h2HwEtaVsProcNeg_->Fill(muon.hwEta(), proc);
    h2HwPhiVsProcNeg_->Fill(muon.hwPhi(), proc);
    h2ProcIdNeg_->Fill(proc, bx);
    h2HwGlobalPhiNeg_->Fill(globalPhi, bx);
    h2GlobalPhiNeg_->Fill(globalPhi * phiScale_, bx);
    hMuPerBxNeg_->Fill(bx);
    hHwGlobalPhiNeg_->Fill(globalPhi);
    hGlobalPhiNeg_->Fill(globalPhi * phiScale_);
  } else if (tfType == l1t::tftype::omtf_pos || tfType == l1t::tftype::emtf_pos) {
    h2HwPtVsProcPos_->Fill(muon.hwPt(), proc);
    h2HwEtaVsProcPos_->Fill(muon.hwEta(), proc);
    h2HwPhiVsProcPos_->Fill(muon.hwPhi(), proc);
    h2ProcIdPos_->Fill(proc, bx);
    h2HwGlobalPhiPos_->Fill(globalPhi, bx);
    h2GlobalPhiPos_->Fill(globalPhi * phiScale_, bx);
    hMuPerBxPos_->Fill(bx);
    hHwGlobalPhiPos_->Fill(globalPhi);
    hGlobalPhiPos_->Fill(globalPhi * phiScale_);
  } else {
    if (muon.hwEta() < 0) {
      h2HwPtVsProcNeg_->Fill(muon.hwPt(), proc);
      h2HwEtaVsProcNeg_->Fill(muon.hwEta(), proc);
      h2HwPhiVsProcNeg_->Fill(muon.hwPhi(), proc);
      h2ProcIdNeg_->Fill(proc, bx);
      h2HwGlobalPhiNeg_->Fill(globalPhi, bx);
      h2GlobalPhiNeg_->Fill(globalPhi * phiScale_, bx);
      hMuPerBxNeg_->Fill(bx);
      hHwGlobalPhiNeg_->Fill(globalPhi);
      hGlobalPhiNeg_->Fill(globalPhi * phiScale_);
    } else {
      h2HwPtVsProcPos_->Fill(muon.hwPt(), proc);
      h2HwEtaVsProcPos_->Fill(muon.hwEta(), proc);
      h2HwPhiVsProcPos_->Fill(muon.hwPhi(), proc);
      h2ProcIdPos_->Fill(proc, bx);
      h2HwGlobalPhiPos_->Fill(globalPhi, bx);
      h2GlobalPhiPos_->Fill(globalPhi * phiScale_, bx);
      hMuPerBxPos_->Fill(bx);
      hHwGlobalPhiPos_->Fill(globalPhi);
      hGlobalPhiPos_->Fill(globalPhi * phiScale_);
    }
  }
  hPt_->Fill(muon.hwPt() * ptScale_);
  hEta_->Fill(muon.hwEta() * etaScale_);
  hGlobalPhi_->Fill(globalPhi * phiScale_);

  h2HwPtVsProc_->Fill(muon.hwPt(), fillProc);
  h2HwEtaVsProc_->Fill(muon.hwEta(), fillProc);
  h2HwPhiVsProc_->Fill(muon.hwPhi(), fillProc);


  h2EtaPhi_->Fill(muon.hwEta()*etaScale_, globalPhi*phiScale_);
}


class UGmtHistos : public MuonHistos {
public:
  UGmtHistos(edm::Service<TFileService> &fs, const std::string& label, const std::string& titlePrefix);
  ~UGmtHistos() {};

  TH2I *h2HwPt_;
  TH2I *h2HwEta_;
  TH2I *h2HwPhi_;
  TH2I *h2HwCharge_;
  TH2I *h2HwChargeValid_;
  TH2I *h2HwQual_;
  TH2F *h2Pt_;
  TH2F *h2Eta_;
  TH2F *h2Phi_;

  TH1I *hHwPt_;
  TH1I *hHwEta_;
  TH1I *hHwPhi_;
  TH1I *hHwCharge_;
  TH1I *hHwChargeValid_;
  TH1I *hHwQual_;
  TH1I *hHwIso_;
  TH1F *hPt_;
  TH1F *hEta_;
  TH1F *hPhi_;

  TH2F *h2EtaPhi_;

  void FillMuon(const l1t::Muon& muon, const int bx);
};

UGmtHistos::UGmtHistos(edm::Service<TFileService> &fs, const std::string& label, const std::string& titlePrefix) : MuonHistos(fs, label, titlePrefix)
{
  h2HwPt_ = fs->make<TH2I>(("h2HwPt"+label).c_str(), (titlePrefix+" hwPt per BX;hwPt;BX").c_str(), 150, 0, 300, 15, -7, 8);
  h2HwEta_ = fs->make<TH2I>(("h2HwEta"+label).c_str(), (titlePrefix+" hwEta per BX;hwEta;BX").c_str(), 231, -230, 232, 15, -7, 8);
  h2HwPhi_ = fs->make<TH2I>(("h2HwPhi"+label).c_str(), (titlePrefix+" hwPhi per BX;hwPhi;BX").c_str(), 145, 0, 580, 15, -7, 8);
  h2HwCharge_ = fs->make<TH2I>(("h2HwCharge"+label).c_str(), (titlePrefix+" hwCharge per BX;charge;BX").c_str(), 3, -1, 2, 15, -7, 8);
  h2HwChargeValid_ = fs->make<TH2I>(("h2HwChargeValid"+label).c_str(), (titlePrefix+" hwChargeValid per BX;charge valid;BX").c_str(), 2, 0, 2, 15, -7, 8);
  h2HwQual_ = fs->make<TH2I>(("h2HwQual"+label).c_str(), (titlePrefix+" hwQual per BX;Quality;BX").c_str(), 16, 0, 16, 15, -7, 8);
  h2Pt_ = fs->make<TH2F>(("h2Pt"+label).c_str(), (titlePrefix+" Pt per BX;GeV;BX").c_str(), 150, 0, 150, 15, -7, 8);
  h2Eta_ = fs->make<TH2F>(("h2Eta"+label).c_str(), (titlePrefix+" Eta per BX;#eta;BX").c_str(), 100, -2.5, 2.5, 15, -7, 8);
  h2Phi_ = fs->make<TH2F>(("h2Phi"+label).c_str(), (titlePrefix+" Phi per BX;#phi;BX").c_str(), 128, 0., 6.4, 15, -7, 8);

  hHwPt_ = fs->make<TH1I>(("hHwPt"+label).c_str(), (titlePrefix+" hwPt").c_str(), 300, 0, 300);
  hHwEta_ = fs->make<TH1I>(("hHwEta"+label).c_str(), (titlePrefix+" hwEta").c_str(), 462, -230, 232);
  hHwPhi_ = fs->make<TH1I>(("hHwPhi"+label).c_str(), (titlePrefix+" hwPhi").c_str(), 580, 0, 580);
  hHwCharge_ = fs->make<TH1I>(("hHwCharge"+label).c_str(), (titlePrefix+" hwCharge").c_str(), 3, -1, 2);
  hHwChargeValid_ = fs->make<TH1I>(("hHwChargeValid"+label).c_str(), (titlePrefix+" hwChargeValid").c_str(), 2, 0, 2);
  hHwQual_ = fs->make<TH1I>(("hHwQual"+label).c_str(), (titlePrefix+" hwQual").c_str(), 16, 0, 16);
  hHwIso_ = fs->make<TH1I>(("hHwIso"+label).c_str(), (titlePrefix+" hwIso").c_str(), 4, 0, 4);
  hPt_ = fs->make<TH1F>(("hPt"+label).c_str(), (titlePrefix+" Pt;GeV").c_str(), 150, 0, 150);
  hEta_ = fs->make<TH1F>(("hEta"+label).c_str(), (titlePrefix+" Eta;#eta").c_str(), 100, -2.5, 2.5);
  hPhi_ = fs->make<TH1F>(("hPhi"+label).c_str(), (titlePrefix+" Phi;#phi").c_str(), 128, 0., 6.4);

  h2EtaPhi_ = fs->make<TH2F>(("h2EtaPhi"+label).c_str(), (titlePrefix+" EtaPhi;#eta;#phi").c_str(), 100, -2.5, 2.5, 128, 0., 6.4);
}

void
UGmtHistos::FillMuon(const l1t::Muon& muon, const int bx)
{
  h2HwPt_->Fill(muon.hwPt(), bx);
  h2HwEta_->Fill(muon.hwEta(), bx);
  h2HwPhi_->Fill(muon.hwPhi(), bx);
  h2HwCharge_->Fill(muon.hwCharge(), bx);
  h2HwChargeValid_->Fill(muon.hwChargeValid(), bx);
  h2HwQual_->Fill(muon.hwQual(), bx);
  h2Pt_->Fill(muon.hwPt() * ptScale_, bx);
  h2Eta_->Fill(muon.hwEta() * etaScale_, bx);
  h2Phi_->Fill(muon.hwPhi() * phiScale_, bx);

  hHwPt_->Fill(muon.hwPt());
  hHwEta_->Fill(muon.hwEta());
  hHwPhi_->Fill(muon.hwPhi());
  hHwCharge_->Fill(muon.hwCharge());
  hHwChargeValid_->Fill(muon.hwChargeValid());
  hHwQual_->Fill(muon.hwQual());
  hHwIso_->Fill(muon.hwIso());
  hPt_->Fill(muon.hwPt() * ptScale_);
  hEta_->Fill(muon.hwEta() * etaScale_);
  hPhi_->Fill(muon.hwPhi() * phiScale_);

  h2EtaPhi_->Fill(muon.hwEta()*etaScale_, muon.hwPhi()*phiScale_);
}


class L1MuonAnalyzer : public edm::EDAnalyzer {
public:
  explicit L1MuonAnalyzer(const edm::ParameterSet&);
  ~L1MuonAnalyzer();

protected:
  float ptScale_;
  float etaScale_;
  float phiScale_;

private:
  virtual void beginJob(void) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();

  void analyzeRegionalMuonCand(const l1t::RegionalMuonCandBxCollection& muons, TfHistos& histos, const std::string& label);
  void analyzeMuon(const l1t::MuonBxCollection& muons, UGmtHistos& histos, const std::string& label);

  int bxId_;

  int fedId_;

  // EDM input tags
  edm::InputTag fedRawDataTag_;
  edm::InputTag bmtfTag_;
  edm::InputTag omtfTag_;
  edm::InputTag emtfTag_;
  edm::InputTag ugmtEmulatorTag_;
  edm::InputTag ugmtUnpackerTag_;
  edm::InputTag gmtTag_;

  bool debug_;

  bool suppressCommas_;

  std::vector<int> bmtfBxShifts_;
  std::vector<int> omtfBxShifts_;
  std::vector<int> emtfBxShifts_;

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

  TH1I *hBxId_;

  TH1I *hMuSizeDiff_;
  TH1I *hMuPtDiff_;
  TH1I *hMuEtaDiff_;
  TH1I *hMuPhiDiff_;
  TH1I *hMuChargeDiff_;
  TH1I *hMuChargeValidDiff_;
  TH1I *hMuQualDiff_;
  TH1I *hMuIsoDiff_;

  TH1F *h2TfVsGmtPhi_;

  TH1I *hGmtBxId_;
  TH2F *h2GmtEtaPhi_;
};


L1MuonAnalyzer::L1MuonAnalyzer(const edm::ParameterSet& iConfig) :
  ptScale_(0.5),
  etaScale_(0.010875),
  phiScale_(0.010908),
  bxId_(-1),
  fedId_(iConfig.getUntrackedParameter<int>("fedId", 1402)),
  fedRawDataTag_(iConfig.getParameter<edm::InputTag>("fedRawDataTag")),
  bmtfTag_(iConfig.getParameter<edm::InputTag>("bmtfTag")),
  omtfTag_(iConfig.getParameter<edm::InputTag>("omtfTag")),
  emtfTag_(iConfig.getParameter<edm::InputTag>("emtfTag")),
  ugmtEmulatorTag_(iConfig.getParameter<edm::InputTag>("ugmtEmulatorTag")),
  ugmtUnpackerTag_(iConfig.getParameter<edm::InputTag>("ugmtUnpackerTag")),
  gmtTag_(iConfig.getParameter<edm::InputTag>("gmtTag")),
  debug_(iConfig.getUntrackedParameter<bool>("debug", false)),
  suppressCommas_(iConfig.getUntrackedParameter<bool>("commaSuppression", false)),
  bmtfBxShifts_(iConfig.getUntrackedParameter<std::vector<int> >("bmtfBxShifts")),
  omtfBxShifts_(iConfig.getUntrackedParameter<std::vector<int> >("omtfBxShifts")),
  emtfBxShifts_(iConfig.getUntrackedParameter<std::vector<int> >("emtfBxShifts")),
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
  consumes<FEDRawDataCollection>(fedRawDataTag_);

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

  hBxId_ = fs_->make<TH1I>("hBxId", "BX id", 370, 0, 3700);

  hMuSizeDiff_ = fs_->make<TH1I>("hMuSizeDiff", "Emulator muon size - unpacker muon size for BX 0", 17, -8, 9);
  hMuPtDiff_ = fs_->make<TH1I>("hMuPtDiff", "Emulator muon pT - unpacker muon pt for BX 0", 521, -260, 261);
  hMuEtaDiff_ = fs_->make<TH1I>("hMuEtaDiff", "Emulator muon #eta - unpacker muon #eta for BX 0", 461, -230, 231);
  hMuPhiDiff_ = fs_->make<TH1I>("hMuPhiDiff", "Emulator muon #phi - unpacker muon #phi for BX 0", 581, -290, 291);
  hMuChargeDiff_ = fs_->make<TH1I>("hMuChargeDiff", "Emulator muon charge - unpacker muon charge for BX 0", 5, -2, 3);
  hMuChargeValidDiff_ = fs_->make<TH1I>("hMuChargeValidDiff", "Emulator muon chargeValid - unpacker muon chargeValid for BX 0", 3, -1, 2);
  hMuQualDiff_ = fs_->make<TH1I>("hMuQualDiff", "Emulator muon qual - unpacker muon qual for BX 0", 31, -15, 16);
  hMuIsoDiff_ = fs_->make<TH1I>("hMuIsoDiff", "Emulator muon iso - unpacker muon iso for BX 0", 7, -3, 4);

  hGmtBxId_ = fs_->make<TH1I>("hGmtBxId", "GMT event BX", 370, 0, 3700);
  h2GmtEtaPhi_ = fs_->make<TH2F>("h2GmtEtaPhi", "GMT eta phi map;#eta;#phi", 100, -2.5, 2.5, 128, 0., 6.4);
}


L1MuonAnalyzer::~L1MuonAnalyzer()
{
}


void
L1MuonAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  edm::Handle<FEDRawDataCollection> rawdata;
  iEvent.getByLabel(fedRawDataTag_, rawdata);
  const FEDRawData& data = rawdata->FEDData(fedId_);
  size_t size = data.size();
  if (size > 0) {
    FEDHeader header(data.data());

    bxId_ = header.bxID();
    hBxId_->Fill(bxId_);
  }

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
          hGmtBxId_->Fill(igmtrr->getBxNr());
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

  //if (analyzeGmt_ && analyzeEmtf_) {
  //  edm::Handle<L1MuGMTReadoutCollection> gmtReadoutColl;
  //  edm::Handle<l1t::RegionalMuonCandBxCollection> tfMuons;
  //  iEvent.getByLabel(gmtTag_, gmtReadoutColl);
  //  iEvent.getByLabel(emtfTag_, tfMuons);

  //  if (gmtReadoutColl.isValid() && tfMuons.isValid()) {
  //    L1MuGMTReadoutCollection const* gmtrc = gmtReadoutColl.product();

  //    std::vector<L1MuGMTReadoutRecord> gmt_records = gmtrc->getRecords();
  //    std::vector<L1MuGMTReadoutRecord>::const_iterator igmtrr;
  //    for(igmtrr=gmt_records.begin(); igmtrr!=gmt_records.end(); igmtrr++) {

  //      std::vector<L1MuGMTExtendedCand>::const_iterator gmt_iter;
  //      std::vector<L1MuGMTExtendedCand> exc = igmtrr->getGMTCands();
  //      unsigned int n = 0;
  //      for(gmt_iter=exc.begin(); gmt_iter!=exc.end(); gmt_iter++) {
  //        if (n < 12 && !(*gmt_iter).empty()) {
  //          if ((*gmt_iter).bx() != 0) {
  //            continue;
  //          }
  //          for (int bx = tfMuons->getFirstBX(); bx <= tfMuons->getLastBX(); ++bx) {
  //            unsigned int ctr = 0;
  //            for (auto mu = tfMuons->begin(bx); mu != tfMuons->end(bx) && ctr < tfMuons->size(bx); ++mu, ++ctr) {
  //              int bxShiftIdx = mu->processor();
  //              l1t::tftype tfType = mu->trackFinderType();
  //              if (tfType == l1t::tftype::omtf_neg || tfType == l1t::tftype::emtf_neg) {
  //                bxShiftIdx += 6;
  //              }
  //              if (bx != emtfBxShifts_[bxShiftIdx]) {
  //                continue;
  //              }
  //              int globalPhi = l1t::MicroGMTConfiguration::calcGlobalPhi(mu->hwPhi(), tfType, mu->processor());
  //              h2TfVsGmtPhi_->Fill((*gmt_iter).phiValue(), globalPhi * phiScale_);
  //            }
  //          }
  //          ++n;
  //        }
  //      }
  //    }
  //  } else {
  //    edm::LogError("MissingProduct") << "Input collection not found" << std::endl;
  //    return;
  //  }
  //}

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
    histos.FillSize(muons.size(bx), bx);
    unsigned int ctr = 0;
    for (auto mu = muons.begin(bx); mu != muons.end(bx) && ctr < muons.size(bx); ++mu, ++ctr) {
      // suppress comma events
      if (suppressCommas_ && ((mu->dataword() & 0xFFFFFFFF) == 0x505050bc || (mu->dataword() >> 32) == 0x505050bc)) {
        if (debug_) {
          std::cout << "       COMMA   ";
        }
      } else {
        histos.FillBx(bx, bxId_);
        histos.FillMuon(*mu, bx);
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
                  << "   TF type: " << mu->trackFinderType()
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
    histos.FillSize(muons.size(bx), bx);
    unsigned int ctr = 0;
    for (auto mu = muons.begin(bx); mu != muons.end(bx) && ctr < muons.size(bx); ++mu, ++ctr) {
      histos.FillBx(bx, bxId_);
      histos.FillMuon(*mu, bx);

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
