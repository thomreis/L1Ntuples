from sys import exit
import argparse
import logging
import os

from L1Analysis import L1Ana, L1Ntuple


def parse_options_and_init_log(loglevel=logging.INFO):
    """
    Adds often used options to the OptionParser...
    """
    parser = argparse.ArgumentParser(description="L1 Analysis Framework macro", formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("-f", "--fname", dest="fname", default="", type=str, help="A root file containing L1Ntuples.")
    parser.add_argument("-l", "--flist", dest="flist", default="", type=str, help="A txt file containing list of L1Ntuple files, one file per line.")
    parser.add_argument("-j", "--njobs", dest="njobs", default=1, type=int, help="Number of jobs to submit")
    parser.add_argument("-n", "--nevents", dest="nevents", default=-1, type=int, help="Number of events to run, -1 for all [default: %default]")
    parser.add_argument("-o", "--outname", dest="outname", default="job", type=str, help="Directory to create scripts in [default: %default]")
    parser.add_argument("-s", "--scriptname", dest="scriptname", default="ntuple.py", type=str, help="Script to create ntuple [default: %default]")

    opts, unknown = parser.parse_known_args()
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


def main():
    L1Ana.init_l1_analysis()
    print ""

    opts = parse_options_and_init_log()

    ntuple = L1Ntuple(opts.nevents)
    if opts.flist:
        ntuple.open_with_file_list(opts.flist)
    if opts.fname:
        ntuple.open_with_file(opts.fname)

    n_per_job = ntuple.nevents / opts.njobs

    # first make sure the directories are created
    if not os.path.exists(opts.outname+"/out"):
        os.makedirs(opts.outname+"/out")
    if not os.path.exists(opts.outname+"/scripts"):
        os.makedirs(opts.outname+"/scripts")

    start_up = "cd {cmssw_dir}/src\n".format(cmssw_dir=os.environ["CMSSW_BASE"])
    start_up += "eval `scram runtime -sh`\n"
    start_up += "cd {pwd}\n".format(pwd=os.getcwd())

    submission_string = ""
    hadd_string = "hadd " + os.path.abspath(opts.outname+"/out/ntuple_comb.root")+" "
    job_dir = os.path.abspath(opts.outname+"/scripts/")
    for i in range(opts.njobs):
        with open(opts.outname+"/scripts/job_{i}.sh".format(i=i), "w") as job_script:
            job_script.write(start_up)
            py_string = "python {script} -f {fname} -n {n} -s {start} ntuple -o {out}\n"
            py_string = py_string.format(script=opts.scriptname, fname=opts.fname, n=n_per_job, start=i*n_per_job, out=opts.outname+"/out/ntuple_{n}.root".format(n=i))
            job_script.write(py_string)
            sub_string = "bsub -q 1nh {dir}/job_{i}.sh\n".format(dir=job_dir, i=i)
            submission_string += sub_string
            hadd_string +=  os.path.abspath(opts.outname+"/out/ntuple_{n}.root".format(n=i))+" "
    with open(opts.outname+"/submit.sh", "w") as submitfile:
        submitfile.write(submission_string)
    with open(opts.outname+"/combine.sh", "w") as combfile:
        combfile.write(hadd_string)

    print "Will process", n_per_job, "events per job"
    print "execute", opts.outname+"/submit.sh", "to submit"
    print "after completion run", opts.outname+"/combine.sh", "to combine the ntuples."

if __name__ == "__main__":
    main()
