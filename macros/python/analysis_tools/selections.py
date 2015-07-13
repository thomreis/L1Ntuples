import math


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
    def match_dr(eta_coll1, phi_coll1, eta_coll2, phi_coll2, cut=0.5, phi_normalize=True, idcs1=None, idcs2=None):
        """
        Matching based on delta R (dR = sqrt(dEta^2 + dPhi^2)):
        Returns a list of index pairs (first idx for collection1 second for collection2)
        Parameters eta_coll1/2 & phi_coll1/2 are the eta/phi vectors from the two collections
        Parameter cut specifies the maximum allowed dR
        Parameter phi_normalize specifies whether the phi scales are different (-pi, pi) vs (0, 2pi)
        Parameters idcs1/2 can specify if only a subset of the collections should be considered
        """
        index_pairs = []
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
                if math.sqrt(deta*deta + dphi*dphi) < cut:
                    index_pairs.append([i, j])

        return index_pairs


class MuonSelections(object):
    """Class containing functions for commonly used muon selections"""

    @staticmethod
    def select_ugmt_muons(ugmt, pt_min=0.5, qual_min=0, abs_eta_min=0, abs_eta_max=4, tftype=None):
        type_acc = tftype
        if isinstance(tftype, int):
            type_acc.append(tftype)
        elif tftype is None:
            type_acc = [0, 1, 2]

        indices = []
        for i in range(ugmt.n):
            select = True
            if ugmt.pt[i] < pt_min:
                select = False
            if math.fabs(ugmt.eta[i]) < abs_eta_min or math.fabs(ugmt.eta[i]) > abs_eta_max:
                select = False
            if ugmt.qual < qual_min:
                select = False
            if not (ugmt.tfLink[i].tf in type_acc):
                select = False

            if select:
                indices.append(i)
        return indices

    @staticmethod
    def select_gmt_muons(gmt, pt_min=0.5, qual_min=0, abs_eta_min=0, abs_eta_max=4):
        indices = []
        for i in range(gmt.N):
            select = True
            if gmt.Pt[i] < pt_min:
                select = False
            if math.fabs(gmt.Eta[i]) < abs_eta_min or math.fabs(gmt.Eta[i]) > abs_eta_max:
                select = False
            if gmt.Qual < qual_min:
                select = False

            if select:
                indices.append(i)
        return indices

    @staticmethod
    def select_reco_muons(reco, pt_min=0.5, abs_eta_min=0, abs_eta_max=4, muon_id=1):
        indices = []
        for i in range(reco.n):
            select = True
            if reco.pt[i] < pt_min:
                select = False
            if math.fabs(reco.eta[i]) < abs_eta_min or math.fabs(reco.eta[i]) > abs_eta_max:
                select = False
            if reco.qual < muon_id:  # FIXME currently not correct!
                select = False

            if select:
                indices.append(i)
        return indices

    @staticmethod
    def select_gen_muons(gen, pt_min=0.5, abs_eta_min=0, abs_eta_max=4):
        indices = []
        for i in range(gen.px.size()):
            select = True
            pt = math.sqrt(gen.px[i]*gen.px[i] + gen.py[i]*gen.py[i])
            if pt < pt_min:
                select = False
            if math.fabs(gen.eta[i]) < abs_eta_min or math.fabs(gen.eta[i]) > abs_eta_max:
                select = False
            if abs(gen.id[i]) != 13:  # Select muons only!
                select = False

            if select:
                indices.append(i)
        return indices
