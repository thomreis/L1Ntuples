from L1Analysis import L1Ana, L1Ntuple
from ToolBox import parse_options
def main():
    L1Ana.init_l1_analysis()
    print ""
    opts, args = parse_options()
    
    ntuple = L1Ntuple()
    ntuple.open_with_file_list("../fnames_zmumu_v7.txt")

    # for i, event in enumerate(ntuple):
    #     pass
    
if __name__ == "__main__":
    main()