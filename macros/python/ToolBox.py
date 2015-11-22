from sys import exit
from optparse import OptionParser
from L1Analysis import L1Ana

def parse_options():
    """
    Adds often used options to the OptionParser...
    """
    parser = OptionParser()
    parser.add_option("--fname", dest="fname", default="", type="string", help="A root file containing L1Ntuples.")
    parser.add_option("--flist", dest="flist", default="", type="string", help="A txt file containing list of L1Ntuple files, one file per line.")
    parser.add_option("--nevents", dest="nevents", default=-1, type="int", help="Number of events to run, -1 for all [default: %default]")

    opts, args = parser.parse_args()
    if opts.fname == "" and opts.flist == "":
        L1Ana.log.fatal("No inputs specified!")
        parser.print_help()
        exit(0)

    return opts, args