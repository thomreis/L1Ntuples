#!/usr/bin/env python
from ToolBox import parse_options_and_init_log
# have to do this first or ROOT masks the -h messages
opts, args = parse_options_and_init_log()

from L1Analysis import L1Ana, L1Ntuple


def analyse(evt):
    # USER HOOK
    # do what you want to do with the ntuples here
    # example:
    print "GMT:", evt.gmt.N
    print "UGMT:", evt.ugmt.n


def main():
    L1Ana.init_l1_analysis()
    print ""

    ntuple = L1Ntuple(opts.nevents)

    if opts.flist:
        ntuple.open_with_file_list(opts.flist)
    if opts.fname:
        ntuple.open_with_file(opts.fname)

    for i, event in enumerate(ntuple):
        if (i+1) % 1000 == 0:
            L1Ana.log.info("Processing event: {n}".format(n=i))
        analyse(event)

if __name__ == "__main__":
    main()
