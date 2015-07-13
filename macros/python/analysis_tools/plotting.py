import ROOT as root


class HistManager(object):
    """Class that manages and holds histograms"""
    def __init__(self, varnames, binning_dict, prefix=""):
        super(HistManager, self).__init__()
        self.varnames = varnames
        self.binnings = binning_dict
        self.prefix = prefix
        root.TGaxis().SetMaxDigits(3)

        self.hists = {}

        self._stackcache = {}

        for vname in varnames:
            self.hists[vname] = root.TH1D(prefix+vname, "", self.binnings[vname][0], self.binnings[vname][1], self.binnings[vname][2])
            self.hists[vname].Sumw2()
            xtitle = self.binnings[vname][3]
            if len(self.binnings[vname]) > 4:
                xtitle += " ({unit})".format(unit=self.binnings[vname][4])

            self.hists[vname].GetXaxis().SetTitle(xtitle)
            self.hists[vname].GetYaxis().SetTitle("# Muons")

    def fill(self, varname, val):
        self.hists[varname].Fill(val)

    def get(self, varname):
        return self.hists[varname]

    def get_stack(self, varnames):
        keyname = "_".join(varnames)
        if keyname in self._stackcache.keys():
            return self._stackcache[keyname]
        stack = root.THStack()
        hs = []
        for vname in varnames:
            h = self.get(vname)
            stack.Add(h)
            hs.append(h)
        self._stackcache[keyname] = [stack, hs]
        return [stack, hs]
