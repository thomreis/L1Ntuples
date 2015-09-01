import math
import ROOT as root


class Matcher(object):
    """Class containing static functions for matching L1Analysis collections"""
    twopi = math.pi*2

    @staticmethod
    def norm_phi(phi):
        """ Makes sure that phi is in -pi, pi, implementation stolen from TVector2"""
        nphi = phi
        while (nphi >= math.pi):
            nphi -= Matcher.twopi
        while (nphi < -math.pi):
            nphi += Matcher.twopi
        return nphi

    @staticmethod
    def delta_phi(phi1, phi2):
        return Matcher.norm_phi(phi1 - phi2)

    @staticmethod
    def delta_r(phi1, eta1, phi2, eta2, phi_normalize=True):
        deta = math.fabs(eta1 - eta2)
        if phi_normalize:
            phi1 = Matcher.norm_phi(phi1)
            phi2 = Matcher.norm_phi(phi2)
        dphi = Matcher.delta_phi(phi1, phi2)
        return math.sqrt(deta*deta + dphi*dphi)

    @staticmethod
    def converted_phi(tf_coll, tftype):
        conv_phi = []
        for i in range(tf_coll.phi.size()):
            conv_phi.append(root.l1t.L1TGMTInternalMuon.calcGlobalPhi(tf_coll.packedPhi[i], tftype, tf_coll.processor[i]) * 0.010908)
        return conv_phi

    @staticmethod
    def match_dr(eta_coll1, phi_coll1, eta_coll2, phi_coll2, cut=0.5, phi_normalize=True, idcs1=None, idcs2=None):
        """
        Matching based on delta R (dR = sqrt(dEta^2 + dPhi^2)):
        Returns a sorted list of index pairs and corr. dR (first in tuple idx for collection1; second for collection2; third is dR)
        List is sorted by increasing dR
        Parameters eta_coll1/2 & phi_coll1/2 are the eta/phi vectors from the two collections
        Parameter cut specifies the maximum allowed dR
        Parameter phi_normalize specifies whether the phi scales are different (-pi, pi) vs (0, 2pi)
        Parameters idcs1/2 can specify if only a subset of the collections should be considered
        """
        index_tuples = []
        if idcs1 is None:
            idcs1 = range(eta_coll1.size())
        if idcs2 is None:
            idcs2 = range(eta_coll2.size())

        for i in idcs1:
            for j in idcs2:
                deta = math.fabs(eta_coll1[i] - eta_coll2[j])
                if deta > cut:  # don't bother with the rest
                    continue
                phi1 = phi_coll1[i]
                phi2 = phi_coll2[j]
                if phi_normalize:
                    phi1 = Matcher.norm_phi(phi1)
                    phi2 = Matcher.norm_phi(phi2)
                dphi = Matcher.delta_phi(phi1, phi2)
                dr = math.sqrt(deta*deta + dphi*dphi)
                if dr < cut:
                    index_tuples.append([i, j, dr, deta, dphi])

        return sorted(index_tuples, key=lambda idx_dr: idx_dr[2])


class MuonSelections(object):
    """Class containing functions for commonly used muon selections"""

    @staticmethod
    def select_ugmt_muons(ugmt, pt_min=0.5, qual_min=0, abs_eta_min=0, abs_eta_max=4, only_pos_eta=False, tftype=None):
        type_acc = tftype
        if isinstance(tftype, int):
            type_acc.append(tftype)
        elif tftype is None:
            type_acc = [0, 1, 2]

        indices = []
        for i in range(ugmt.n):
            if ugmt.pt[i] < pt_min:
                continue
            if only_pos_eta and ugmt.eta[i] < 0:
                continue
            if math.fabs(ugmt.eta[i]) < abs_eta_min or math.fabs(ugmt.eta[i]) > abs_eta_max:
                continue
            if ugmt.qual[i] < qual_min:
                continue
            if not (ugmt.tfLink[i].tf in type_acc):
                continue
            if ugmt.tfLink[i].tf == 1:
                tf = ugmt.tfInfo[ugmt.tfLink[i].tf]
                k = ugmt.tfLink[i].idx
                if tf.trAddress[k] == 99840 or tf.trAddress[k] == 34304 or tf.trAddress[k] == 3075 or tf.trAddress[k] == 36928 or tf.trAddress[k] == 12300 or tf.trAddress[k] == 98816 or tf.trAddress[k] == 98944 or tf.trAddress[k] == 33408 or tf.trAddress[k] == 66688 or tf.trAddress[k] == 66176:
                    continue
            indices.append(i)
        return indices

    @staticmethod
    def select_tf_muons(tf, pt_min=0.5, qual_min=0, abs_eta_min=0, abs_eta_max=4, only_pos_eta=False, tftype=None, idcs=None):
        indices = []
        if idcs is None:
            idcs = range(tf.eta.size())
        for i in idcs:
            if tf.pt[i] < pt_min:
                continue
            if only_pos_eta and tf.eta[i] < 0:
                continue
            if math.fabs(tf.eta[i]) < abs_eta_min or math.fabs(tf.eta[i]) > abs_eta_max:
                continue
            if tf.qual[i] < qual_min:
                continue
            if tftype == 1:
                if tf.trAddress[i] == 99840 or tf.trAddress[i] == 34304 or tf.trAddress[i] == 3075 or tf.trAddress[i] == 36928 or tf.trAddress[i] == 12300 or tf.trAddress[i] == 98816 or tf.trAddress[i] == 98944 or tf.trAddress[i] == 33408 or tf.trAddress[i] == 66688 or tf.trAddress[i] == 66176:
                    continue
            indices.append(i)
        return indices

    @staticmethod
    def select_gmt_muons(gmt, pt_min=0.5, qual_min=0, abs_eta_min=0, abs_eta_max=4, only_pos_eta=False, idcs=None):
        # qual_min = 8 is interpreted as 2012 running conditions, i.e.:
        # take qualities 6, 7 and 5 if BX == 0
        indices = []

        if idcs is None:
            idcs = range(gmt.N)
        for i in idcs:
            if gmt.Pt[i] < pt_min:
                continue
            if only_pos_eta and gmt.Eta[i] < 0:
                continue
            if math.fabs(gmt.Eta[i]) < abs_eta_min or math.fabs(gmt.Eta[i]) > abs_eta_max:
                continue
            if gmt.Qual[i] < qual_min and qual_min < 8:
                continue
            elif qual_min == 8:
                if (gmt.Qual[i] < 5) or (gmt.Qual[i] == 5 and gmt.CandBx[i] != 0):
                    continue
            indices.append(i)
        return indices

    @staticmethod
    def select_reco_muons(reco, pt_min=0.5, abs_eta_min=0, abs_eta_max=4, muon_id=1, only_pos_eta=False):
        indices = []
        for i in range(reco.n):
            if reco.pt[i] < pt_min:
                continue
            if math.fabs(reco.eta[i]) < abs_eta_min or math.fabs(reco.eta[i]) > abs_eta_max:
                continue
            if only_pos_eta and reco.eta[i] < 0:
                continue
            if reco.qual[i] < muon_id:  # FIXME currently not correct!
                continue

            indices.append(i)
        return indices

    @staticmethod
    def select_gen_muons(gen, pt_min=0.5, abs_eta_min=0, abs_eta_max=4, only_pos_eta=False, idcs=None):
        if idcs is None:
            idcs = range(gen.pt.size())
        indices = []
        for i in idcs:
            if abs(gen.id[i]) != 13:  # Select muons only!
                continue
            if gen.pt[i] < pt_min:
                continue
            if math.fabs(gen.eta[i]) < abs_eta_min or math.fabs(gen.eta[i]) > abs_eta_max:
                continue
            if only_pos_eta and gen.eta[i] < 0:
                continue
            indices.append(i)
        return indices
