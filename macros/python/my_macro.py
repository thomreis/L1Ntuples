#!/usr/bin/env python
from ToolBox import parse_options_and_init_log
# have to do this first or ROOT masks the -h messages
opts, parser = parse_options_and_init_log()

from L1Analysis import L1Ana, L1Ntuple
import ROOT as root

def analyse(evt):
    # USER HOOK
    # do what you want to do with the ntuples here
    # example:
    print evt.gmt.N
    print evt.recoMuon.nMuons


def main():
    root.gROOT.SetBatch()
    L1Ana.init_l1_analysis()
    print ""

    ntuple = L1Ntuple(opts.nevents)

    if opts.flist:
        ntuple.open_with_file_list(opts.flist)
    if opts.fname:
        ntuple.open_with_file(opts.fname)

    start_evt = opts.start_event
    end_evt = opts.start_event+ntuple.nevents
    for i in range(start_evt, end_evt):
        event = ntuple[i]
        if (i+1) % 1000 == 0: L1Ana.log.info("Processing event: {n}".format(n=i))
        analyse(event)

if __name__ == "__main__":
    main()
