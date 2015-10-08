#include "../interface/L1AnalysisUGMT.h"
#include "DataFormats/L1TMuon/interface/RegionalMuonCand.h"
#include "DataFormats/L1TMuon/interface/GMTInternalMuon.h"
#include "L1Trigger/L1TMuon/interface/MicroGMTConfiguration.h"


namespace L1Analysis {
L1AnalysisUGMT::L1AnalysisUGMT() :
  ugmt_(32)
{
}

void
L1AnalysisUGMT::fillTrackFinder(const L1TRegionalMuonColl& coll, tftype mytype, int& ctr, int bx) {
  for (auto mu = coll.begin(bx); mu != coll.end(bx); ++mu) {
      ctr++;
      l1t::tftype regTf = mu->trackFinderType();
      int globPhi = l1t::MicroGMTConfiguration::calcGlobalPhi(mu->hwPhi(), regTf, mu->processor());
      ugmt_.tfInfo[mytype].pt.push_back(mu->hwPt() * 0.5f);
      ugmt_.tfInfo[mytype].eta.push_back(mu->hwEta() * 0.010875);
      ugmt_.tfInfo[mytype].phi.push_back(globPhi * 0.010908);
      ugmt_.tfInfo[mytype].qual.push_back(mu->hwQual());
      ugmt_.tfInfo[mytype].ch.push_back(mu->hwSign());
      ugmt_.tfInfo[mytype].bx.push_back(bx);
      ugmt_.tfInfo[mytype].processor.push_back(mu->processor());
      ugmt_.tfInfo[mytype].trAddress.push_back(mu->hwTrackAddress());

      ugmt_.tfInfo[mytype].packedPt.push_back(mu->hwPt());
      ugmt_.tfInfo[mytype].packedEta.push_back(mu->hwEta());
      ugmt_.tfInfo[mytype].packedPhi.push_back(mu->hwPhi());
    }
}

int
L1AnalysisUGMT::findMuon(const l1t::Muon& mu, const L1TRegionalMuonColl& coll, int bx)
{
  for (unsigned i = 0; i < coll.size(bx); ++i) {
    auto tf = coll.at(bx, i);
    int phi = l1t::MicroGMTConfiguration::calcGlobalPhi(tf.hwPhi(), tf.trackFinderType(), tf.processor());
    if (tf.hwEta() == mu.hwEta() &&
        phi == mu.hwPhi() &&
        tf.hwPt() == mu.hwPt() &&
        tf.hwQual() == mu.hwQual()) {
      return i;
    }
  }
  return -1;
}

TFLink
L1AnalysisUGMT::matchTrackFinder(const l1t::Muon& mu, const L1TRegionalMuonColl& bmtf, const L1TRegionalMuonColl& omtf, const L1TRegionalMuonColl& emtf, const L1TRegionalMuonColl& brlRpc, int bx)
{
  int match = -1;
  match = findMuon(mu, bmtf, bx);
  if (match >= 0) {
    return TFLink(L1Analysis::tftype::bmtf, match);
  }
  match = findMuon(mu, omtf, bx);
  if (match >= 0) {
    return TFLink(L1Analysis::tftype::omtf, match);
  }
  match = findMuon(mu, emtf, bx);
  if (match >= 0) {
    return TFLink(L1Analysis::tftype::emtf, match);
  }
  match = findMuon(mu, brlRpc, bx);
  if (match >= 0) {
    return TFLink(L1Analysis::tftype::brlRpc, match);
  }
  return TFLink();
}


void
L1AnalysisUGMT::Set(const l1t::MuonBxCollection& ugmtrc,
                    const L1TRegionalMuonColl& bmtfColl,
                    const L1TRegionalMuonColl& omtfColl,
                    const L1TRegionalMuonColl& emtfColl,
                    const L1TRegionalMuonColl& brlRpcColl,
                    bool onlyBX0) {
  int ugmtCtr = 0;
  int bmtfCtr = 0;
  int omtfCtr = 0;
  int emtfCtr = 0;
  int brlRpcCtr = 0;

  int lastBXMaxBmtf = 0;
  int lastBXMaxOmtf = 0;
  int lastBXMaxEmtf = 0;
  int lastBXMaxBrlRpc = 0;

  for (int bx = ugmtrc.getFirstBX(); bx <= ugmtrc.getLastBX(); ++bx) {
    if (bx != 0 && onlyBX0) {
      continue;
    }
    for (auto mu = ugmtrc.begin(bx); mu != ugmtrc.end(bx); ++mu) {
      ugmtCtr++;

      ugmt_.pt.push_back(mu->pt());
      ugmt_.eta.push_back(mu->eta());
      ugmt_.phi.push_back(mu->phi());
      ugmt_.qual.push_back(mu->hwQual());
      ugmt_.ch.push_back(mu->hwCharge());
      ugmt_.bx.push_back(bx);

      ugmt_.packedPt.push_back(mu->hwPt());
      ugmt_.packedEta.push_back(mu->hwEta());
      ugmt_.packedPhi.push_back(mu->hwPhi());

      // second bit = rel iso, first bit = abs iso
      ugmt_.relIso.push_back((mu->hwIso() & 2) << 1);
      ugmt_.absIso.push_back(mu->hwIso() & 1);
      ugmt_.isoEnergy.push_back(mu->hwIsoSum());

      ugmt_.rank.push_back(mu->hwRank());
      ugmt_.packedIso.push_back(mu->hwIso());
      // keep the vector index in synch with the collection index:
      // add offset of the max muon index of last BX
      TFLink tfL = matchTrackFinder(*mu, bmtfColl, omtfColl, emtfColl, brlRpcColl, bx);
      if (tfL.tf == L1Analysis::tftype::bmtf) tfL.idx += lastBXMaxBmtf;
      if (tfL.tf == L1Analysis::tftype::omtf) tfL.idx += lastBXMaxOmtf;
      if (tfL.tf == L1Analysis::tftype::emtf) tfL.idx += lastBXMaxEmtf;
      if (tfL.tf == L1Analysis::tftype::brlRpc) tfL.idx += lastBXMaxBrlRpc;

      ugmt_.tfLink.push_back(tfL);
    }

    fillTrackFinder(bmtfColl, tftype::bmtf, bmtfCtr, bx);
    fillTrackFinder(omtfColl, tftype::omtf, omtfCtr, bx);
    fillTrackFinder(emtfColl, tftype::emtf, emtfCtr, bx);
    fillTrackFinder(brlRpcColl, tftype::brlRpc, brlRpcCtr, bx);

    lastBXMaxBmtf = bmtfCtr;
    lastBXMaxOmtf = omtfCtr;
    lastBXMaxEmtf = emtfCtr;
    lastBXMaxBrlRpc = brlRpcCtr;
  }

  ugmt_.n = ugmtCtr;
  ugmt_.nBmtf = bmtfCtr;
  ugmt_.nOmtf = omtfCtr;
  ugmt_.nEmtf = emtfCtr;
  ugmt_.nBrlRpc = brlRpcCtr;
}

} // namespace L1Analysis
