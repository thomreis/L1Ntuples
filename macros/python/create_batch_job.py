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
    parser.add_argument("-q" ,"--queue", dest="queue", default="1nh", help="Queue to submit the jobs to.")
    parser.add_argument("-j", "--njobs", dest="njobs", default=1, type=int, help="Number of jobs to submit")
    parser.add_argument("-n", "--nevents", dest="nevents", default=-1, type=int, help="Number of events to run, -1 for all [default: %default]")
    parser.add_argument("-w"  ,"--workdir", dest="workdir", default='job', type=str, help="Work directory to create scripts and store output in [default: %default]")
    parser.add_argument("-o", "--outname", dest="outname", default="output", type=str, help="File name for output files. .root added automatically [default: %default]")
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
    if not os.path.exists(opts.workdir+"/out"):
        os.makedirs(opts.workdir+"/out")
    if not os.path.exists(opts.workdir+"/scripts"):
        os.makedirs(opts.workdir+"/scripts")

    start_up = "cd {cmssw_dir}/src\n".format(cmssw_dir=os.environ["CMSSW_BASE"])
    start_up += "eval `scram runtime -sh`\n"
    start_up += "cd {pwd}\n".format(pwd=os.getcwd())

    submission_string = ""
    job_dir = os.path.abspath(opts.workdir+"/scripts/")
    out_dir = os.path.abspath(opts.workdir+"/out/")
    for i in range(opts.njobs):
        with open(opts.workdir+"/scripts/job_{i}.sh".format(i=i), "w") as job_script:
            job_script.write(start_up)
            py_string = "python {script} -f {fname} -n {n} -s {start} {subparser} -o {out}\n"
            py_string = py_string.format(script=opts.scriptname, fname=opts.fname, n=n_per_job, start=i*n_per_job, subparser=opts.scriptname[:-3], out=opts.workdir+"/out/{name}_{n}.root".format(name=opts.outname, n=i))
            job_script.write(py_string)
            sub_string = "bsub -q {queue} -cwd {cwd} -J job_{i} {dir}/job_{i}.sh\n".format(queue=opts.queue, cwd=out_dir, dir=job_dir, i=i)
            submission_string += sub_string
        os.system('chmod 744 {dir}/job_{i}.sh'.format(dir=job_dir, i=i))

    with open(opts.workdir+"/submit.sh", "w") as submitfile:
        submitfile.write(submission_string)


if __name__ == "__main__":
    main()
