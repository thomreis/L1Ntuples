from sys import exit
import argparse
import logging


def parse_options_and_init_log(loglevel=logging.INFO):
    """
    Adds often used options to the OptionParser...
    """
    parser = argparse.ArgumentParser(description="L1 Analysis Framework macro", formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("-f", "--fname", dest="fname", default="", type=str, help="A root file containing L1Ntuples.")
    parser.add_argument("-l", "--flist", dest="flist", default="", type=str, help="A txt file containing list of L1Ntuple files, one file per line.")
    parser.add_argument("-n", "--nevents", dest="nevents", default=-1, type=int, help="Number of events to run, -1 for all [default: %default]")

    opts = parser.parse_args()
    if opts.fname == "" and opts.flist == "":
        from L1Analysis import L1Ana
        L1Ana.init_logging("L1Analysis", loglevel)
        L1Ana.log.fatal("No inputs specified!")
        parser.print_help()
        exit(0)
    else:
        from L1Analysis import L1Ana
        L1Ana.init_logging("L1Analysis", loglevel)
    return opts
