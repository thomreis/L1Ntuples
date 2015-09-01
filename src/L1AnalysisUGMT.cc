#include "../interface/L1AnalysisUGMT.h"
#include "DataFormats/L1TMuon/interface/L1TRegionalMuonCandidate.h"


namespace L1Analysis {
L1AnalysisUGMT::L1AnalysisUGMT() :
  ugmt_(32)
{
}

void
L1AnalysisUGMT::fillTrackFinder(const L1TRegionalMuonColl& coll, tftype mytype, int& ctr, int bx) {
  for (auto mu = coll.begin(); mu != coll.end(); ++mu) {
      ctr++;
      ugmt_.tfInfo[mytype].pt.push_back(mu->hwPt() * 0.5f);
      ugmt_.tfInfo[mytype].eta.push_back(mu->hwEta() * 0.010875);
      ugmt_.tfInfo[mytype].phi.push_back(mu->hwPhi() * 0.010908);
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

void
L1AnalysisUGMT::Set(const l1t::MuonBxCollection& ugmtrc,
                    const L1TRegionalMuonColl& bmtfColl,
                    const L1TRegionalMuonColl& omtfColl,
                    const L1TRegionalMuonColl& emtfColl,
                    bool onlyBX0) {
	int ugmtCtr = 0;
	int bmtfCtr = 0;
	int omtfCtr = 0;
	int emtfCtr = 0;

  int lastBXMaxBmtf = 0;
  int lastBXMaxOmtf = 0;
  int lastBXMaxEmtf = 0;

	for (int bx = ugmtrc.getFirstBX(); bx <= ugmtrc.getLastBX(); ++bx) {
		if (bx != 0 && onlyBX0) {
			continue;
		}
		for (auto mu = ugmtrc.begin(bx); mu != ugmtrc.end(bx); ++mu) {
			ugmtCtr++;
			auto regional = mu->origin();
			L1Analysis::tftype l1anaTf = tftype::bmtf;

      l1t::tftype regTf = regional->trackFinderType();
      int lastBXMax = 0;
      if (regTf == l1t::tftype::bmtf) {
        l1anaTf = L1Analysis::tftype::bmtf;
        lastBXMax = lastBXMaxBmtf;
      } else if (regTf == l1t::tftype::omtf_pos || regTf == l1t::tftype::omtf_neg) {
        l1anaTf = L1Analysis::tftype::omtf;
        lastBXMax = lastBXMaxOmtf;
      } else if (regTf == l1t::tftype::emtf_pos || regTf == l1t::tftype::emtf_neg) {
        l1anaTf = L1Analysis::tftype::emtf;
        lastBXMax = lastBXMaxEmtf;
      }

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
      ugmt_.tfLink.emplace_back(l1anaTf, lastBXMax+regional.key());
		}

    fillTrackFinder(bmtfColl, tftype::bmtf, bmtfCtr, bx);
    fillTrackFinder(omtfColl, tftype::omtf, omtfCtr, bx);
    fillTrackFinder(emtfColl, tftype::emtf, emtfCtr, bx);

    lastBXMaxBmtf = bmtfCtr;
    lastBXMaxOmtf = omtfCtr;
    lastBXMaxEmtf = emtfCtr;
	}

  ugmt_.n = ugmtCtr;
  ugmt_.nBmtf = bmtfCtr;
  ugmt_.nOmtf = omtfCtr;
  ugmt_.nEmtf = emtfCtr;
}

} // namespace L1Analysis
