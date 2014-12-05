from optparse import OptionParser

def parse_options():
    """
    Adds often used options to the OptionParser...
    """
    parser = OptionParser()
    parser.add_option("--flist", dest="flist", default="", type="string", help="A txt file containing list of files, one file per line.")
    parser.add_option("--fname", dest="fname", default="", type="string", help="The root file to run with")

    opts, args = parser.parse_args()
    if len(args) > 0:
        print args
    return opts, args