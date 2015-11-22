import ROOT as root

from sys import exit
import logging

class L1Ana(object):
    """
    Static class that manages all the pyROOT / pyFWLite and initialization part
    It also manages logging for those who like colourful prompts
    """
    error_pre = '\x1b[31;01m'
    info_pre = '\x1b[32;01m'
    warning_pre = '\x1b[33;01m'
    fatal_pre = '\x1b[31;31m'
    debug_pre = '\x1b[36;01m'
    reset = '\x1b[39;49;00m'
    log = None

    loginit = False
    l1init = False

    @staticmethod
    def init_l1_analysis():
        """
        Initialize all the tools that are already present for analysing L1Ntuples
        Through ROOT we can just use whatever is already implemented
        """
        if not L1Ana.loginit:
            L1Ana.init_logging()

        # Import FWLite
        L1Ana.log.info("FWCoreFWLite library being loaded.")
        root.gSystem.Load("libFWCoreFWLite")
        root.gROOT.ProcessLine('AutoLibraryLoader::enable();')
        root.gSystem.Load("libCintex")
        root.gROOT.ProcessLine('ROOT::Cintex::Cintex::Enable();')

        # append the paths to the macro classes
        curr_path = root.gEnv.GetValue("Unix.*.Root.MacroPath", "")
        macro_path = ":$CMSSW_BASE/src/L1TriggerDPG/L1Ntuples/macros"
        root.gEnv.SetValue("Unix.*.Root.MacroPath", curr_path+macro_path)
        work_path = root.gROOT.GetMacroPath()
        l1rates_path = ":$CMSSW_BASE/src/L1TriggerDPG/L1Ntuples/macros/L1Rates"
        l1rates_path += ":$CMSSW_BASE/src/L1TriggerDPG/L1Ntuples/macros/L1Rates/toolbox"
        root.gROOT.SetMacroPath(work_path+macro_path+l1rates_path)

        # append the include directories to get access to L1Analysis-DataFormats:
        L1Ana.log.info("Adding L1Ntuple include directories")
        root.gSystem.AddIncludePath(" -I$CMSSW_BASE/src/L1TriggerDPG/L1Ntuples/interface")
        root.gSystem.AddIncludePath(" -I$CMSSW_BASE/src/L1TriggerDPG/L1Ntuples/macros")
        root.gROOT.ProcessLine(".include $CMSSW_BASE/src/L1TriggerDPG/L1Ntuples/interface")
        root.gROOT.ProcessLine(".include $CMSSW_BASE/src/L1TriggerDPG/L1Ntuples/macros")

        root.gSystem.AddIncludePath(" -I$CMSSW_BASE/src/L1TriggerDPG/L1Ntuples/macros/L1Rates")
        root.gSystem.AddIncludePath(" -I$CMSSW_BASE/src/L1TriggerDPG/L1Ntuples/macros/L1Rates/toolbox")
        root.gROOT.ProcessLine(".include $CMSSW_BASE/src/L1TriggerDPG/L1Ntuples/macros/L1Rates")
        root.gROOT.ProcessLine(".include $CMSSW_BASE/src/L1TriggerDPG/L1Ntuples/macros/L1Rates/toolbox")

        L1Ana.log.info("--- Initialization done ---")
        L1Ana.l1init = True
    
    @staticmethod
    def init_logging(name = "L1Ana", level = None):
        """
        Initialize a logger with different colors for importance-levels
        """
        if not L1Ana.loginit or level != None:
            L1Ana.log = logging.getLogger(name)
            if level != None:
                L1Ana.log.setLevel(level)
            else:
                L1Ana.log.setLevel(logging.DEBUG)
            logging.addLevelName( logging.FATAL,   L1Ana.fatal_pre + logging.getLevelName(logging.FATAL)   + L1Ana.reset)
            logging.addLevelName( logging.ERROR,   L1Ana.error_pre + logging.getLevelName(logging.ERROR)   + L1Ana.reset)
            logging.addLevelName( logging.WARNING, L1Ana.warning_pre  + logging.getLevelName(logging.WARNING) + L1Ana.reset)
            logging.addLevelName( logging.INFO,    L1Ana.info_pre  + logging.getLevelName(logging.INFO)    + L1Ana.reset)
            logging.addLevelName( logging.DEBUG,   L1Ana.debug_pre + logging.getLevelName(logging.DEBUG)   + L1Ana.reset)

            logging.basicConfig(level=level,format='%(asctime)s (%(name)s) [%(levelname)s]: %(message)s', datefmt='%H:%M:%S')
            L1Ana.loginit = True

class L1Data(object):
    """
    This is the container that is returned by the iterator:
    The user will basically work on this container only and have access
    to all L1Ntuple-DataFormats through this.
    """
    def __init__(self):
        super(L1Data, self).__init__()
        self.event = None
        self.simulation = None
        self.gct = None
        self.gmt = None
        self.gt = None
        self.rct = None
        self.dttf = None
        self.csctf = None
        self.recoMet = None
        self.recoMuon = None
        self.recoRpcHit = None
        self.recoJet = None
        self.recoBasicCluster = None
        self.recoSuperCluster = None
        self.l1extra = None
        self.l1emuextra = None
        self.recoVertex = None
        self.recoTrack = None
        self.l1menu = None
        self.gen = None


class L1Ntuple(object):
    """
    The interface to the user, it is based on the L1NTuple c++ class
    """
    def __init__(self, nevents = -1):
        super(L1Ntuple, self).__init__()
        self.data = L1Data()
        self.do_reco = False
        self.do_muonreco = False
        self.do_l1extra = False
        self.do_l1emuextra = False
        self.do_l1menu = False

        self.tree_main = None
        self.tree_muon = None
        self.tree_reco = None
        self.tree_extra = None
        self.tree_menu = None
        self.tree_emu_extra = None

        self.file_list = []
        self.nentries = -1
        self.nevents = nevents
        self.current = 0
        self.curr_file = None
        self.init = False

    def open_with_file_list(self, fname_list):
        """
        Initilize with a text file containing all root-files with
        L1Ntuples (one per line)
        TAKES: The file name pointing to the txt-file 
        """
        if not L1Ana.l1init: L1Ana.init_l1_analysis()
        self.open_file_list(fname_list)
        self.check_first_file()
        self.open_no_init()
        self.init_branches()

        L1Ana.log.info("Ready to analyse.")
        self.init = True

    def open_with_file(self, fname):
        """
        Initialize with only one root-file containing a L1Ntuple
        TAKES: The file name pointing to the root-file
        """
        if not L1Ana.l1init: L1Ana.init_l1_analysis()
        self.file_list = [fname]
        self.check_first_file()
        self.open_no_init()
        self.init_branches()
        L1Ana.log.info("Ready to analyse.")
        self.init = True

    def open_no_init(self):
        """
        Initializes the TChains and adds present Trees as friends to the main tree
        this is needed so the tree's GetEntry stays in synch.
        """
        self.tree_main = root.TChain("l1NtupleProducer/L1Tree")
        self.tree_muon = root.TChain("l1MuonRecoTreeProducer/MuonRecoTree")
        self.tree_reco =  root.TChain("l1RecoTreeProducer/RecoTree")
        self.tree_extra = root.TChain("l1ExtraTreeProducer/L1ExtraTree")
        self.tree_emu_extra = root.TChain("l1EmulatorExtraTree/L1ExtraTree")
        self.tree_menu =  root.TChain("l1MenuTreeProducer/L1MenuTree")

        for fname in self.file_list:
            self.tree_main.Add(fname)
            if self.do_reco:        self.tree_reco.Add(fname)
            if self.do_muon:        self.tree_muon.Add(fname)
            if self.do_l1emuextra:  self.tree_emu_extra.Add(fname)
            if self.do_l1extra:     self.tree_extra.Add(fname)
            if self.do_l1menu:      self.tree_menu.Add(fname)

        if self.do_reco:        self.tree_main.AddFriend(self.tree_reco)
        if self.do_muon:        self.tree_main.AddFriend(self.tree_muon)
        if self.do_l1emuextra:  self.tree_main.AddFriend(self.tree_emu_extra)
        if self.do_l1extra:     self.tree_main.AddFriend(self.tree_extra)
        if self.do_l1menu:      self.tree_main.AddFriend(self.tree_menu)


    def open_file_list(self, fname_list):
        """
        Open the txt-file and add all file-names to the list of files
        TAKES: file-name pointing to txt-file with one L1Ntuple-file per line
        """
        self.file_list = []
        L1Ana.log.info("Reading txt file with root-file list.")
        cntr = 0
        try:
            with open(fname_list) as fobj:
                for line in fobj:
                    fname = line.strip()
                    if fname == "": continue
                    self.file_list.append(fname)
                    cntr += 1
        except EnvironmentError:
            L1Ana.log.fatal("While reading file (probably it does not exist): {fname}".format(fname=fname_list))
            exit(0)

        L1Ana.log.info("Found list of {n} files:".format(n = cntr))
        for name in self.file_list:
            L1Ana.log.info("-- {fname}".format(fname=name))

    def check_first_file(self):
        """
        Checks which branches and trees are present in the first root-file.
        Sets flags which ones to add accordingly.
        """
        if not self.file_list:
            L1Ana.log.fatal("No root-files specified")
            exit(0)

        self.curr_file = root.TFile.Open(self.file_list[0])

        if not self.curr_file:
            L1Ana.log.fatal("Could not open file: {fname}".format(fname=self.file_list[0]))
            exit(0)
        if self.curr_file.IsOpen() == 0:
            L1Ana.log.fatal("Could not open file: {fname}".format(fname=self.file_list[0]))
            exit(0)

        my_chain = self.curr_file.Get("l1NtupleProducer/L1Tree")
        muon  = self.curr_file.Get("l1MuonRecoTreeProducer/MuonRecoTree")
        jets  = self.curr_file.Get("l1RecoTreeProducer/RecoTree")
        extra = self.curr_file.Get("l1ExtraTreeProducer/L1ExtraTree")
        emuextra = self.curr_file.Get("l1EmulatorExtraTree/L1ExtraTree")
        menu  = self.curr_file.Get("l1MenuTreeProducer/L1MenuTree")

        if my_chain:
            L1Ana.log.info("Found L1Tree...")
        else:
            L1Ana.log.fatal("Could not find the main L1Tree.")
            exit(0)

        if muon:
            L1Ana.log.info("Found MuonRecoTree... Will add access to it.")
            self.do_muon = True
        else:
            L1Ana.log.warning("Could not find MuonRecoTree... It will be skipped.")


        if jets:
            L1Ana.log.info("Found RecoTree... Will add access to it.")
            self.do_reco = True
        else:
            L1Ana.log.warning("Could not find RecoTree... It will be skipped.")


        if extra:
            L1Ana.log.info("Found L1Extra... Will add access to it.")
            self.do_l1extra = True
        else:
            L1Ana.log.warning("Could not find L1ExtraTree... It will be skipped.")


        if emuextra:
            L1Ana.log.info("Found L1EmulatorExtra... Will add access to it.")
            self.do_l1emuextra = True
        else:
            L1Ana.log.warning("Could not find MuonRecoTree... It will be skipped.")


        if menu:
            L1Ana.log.info("Found L1MenuTree... Will add access to it.")
            self.do_l1menu = True
        else:
            L1Ana.log.warning("Could not find L1MenuTree... It will be skipped.")


    def init_branches(self):
        """
        Connect the branches of the Trees with the corresponding members
        in the L1Data container.
        """
        if not self.tree_main:
            L1Ana.log.fatal("There is no main L1Tree -- aborting initialization of branches")
            exit(0)

        self.nentries = self.tree_main.GetEntries()
        if self.nevents < 0 or self.nevents > self.nentries:
            self.nevents = self.nentries

        L1Ana.log.info("Approximate number of entries: {n}, running over: {n2}".format(n=self.nentries, n2=self.nevents))

        self.data.event =   root.L1Analysis.L1AnalysisEventDataFormat()
        self.data.simulation = root.L1Analysis.L1AnalysisSimulationDataFormat()
        self.data.gct = root.L1Analysis.L1AnalysisGCTDataFormat()
        self.data.gmt = root.L1Analysis.L1AnalysisGMTDataFormat()
        self.data.gt = root.L1Analysis.L1AnalysisGTDataFormat()
        self.data.rct = root.L1Analysis.L1AnalysisRCTDataFormat()
        self.data.dttf = root.L1Analysis.L1AnalysisDTTFDataFormat()
        self.data.csctf = root.L1Analysis.L1AnalysisCSCTFDataFormat()

        L1Ana.log.info("Setting branch addresses for main L1Tree.")
        self.tree_main.SetBranchAddress("Event", root.AddressOf(self.data.event))
        self.tree_main.SetBranchAddress("GCT", root.AddressOf(self.data.gct))
        self.tree_main.SetBranchAddress("GMT", root.AddressOf(self.data.gmt))
        self.tree_main.SetBranchAddress("GT", root.AddressOf(self.data.gt))
        self.tree_main.SetBranchAddress("RCT", root.AddressOf(self.data.rct))
        self.tree_main.SetBranchAddress("CSCTF", root.AddressOf(self.data.csctf))
        self.tree_main.SetBranchAddress("DTTF", root.AddressOf(self.data.dttf))

        if self.tree_main.GetBranch("Simulation"):
            self.tree_main.SetBranchAddress("Simulation", root.AddressOf(self.data.simulation))
        else:
            L1Ana.log.warning("Simulation branch not present...")

        if self.tree_main.GetBranch("Generator"):
            self.tree_main.SetBranchAddress("Generator", root.AddressOf(self.data.gen))
        else:
            L1Ana.log.warning("Generator branch not present...")

        if self.do_reco:
            L1Ana.log.info("Setting branch addresses for RecoTree.")
            self.data.recoMet = root.L1Analysis.L1AnalysisRecoMetDataFormat()
            self.data.recoJet = root.L1Analysis.L1AnalysisRecoJetDataFormat()
            self.data.recoBasicCluster = root.L1Analysis.L1AnalysisRecoClusterDataFormat()
            self.data.recoSuperCluster = root.L1Analysis.L1AnalysisRecoClusterDataFormat()
            self.data.recoVertex = root.L1Analysis.L1AnalysisRecoVertexDataFormat()
            self.data.recoTrack = root.L1Analysis.L1AnalysisRecoTrackDataFormat()

            self.tree_reco.SetBranchAddress("Jet", root.AddressOf(self.data.recoJet))
            self.tree_reco.SetBranchAddress("BasicClusters", root.AddressOf(self.data.recoBasicCluster))
            self.tree_reco.SetBranchAddress("SuperClusters", root.AddressOf(self.data.recoSuperCluster))
            self.tree_reco.SetBranchAddress("Met", root.AddressOf(self.data.recoMet))
            self.tree_reco.SetBranchAddress("Tracks", root.AddressOf(self.data.recoTrack))
            self.tree_reco.SetBranchAddress("Vertices", root.AddressOf(self.data.recoVertex))


        if self.do_muon:
            L1Ana.log.info("Setting branch addresses for MuonRecoTree.")
            self.data.recoMuon = root.L1Analysis.L1AnalysisRecoMuonDataFormat()
            self.data.recoRpcHit =  root.L1Analysis.L1AnalysisRecoRpcHitDataFormat()
            
            self.tree_muon.SetBranchAddress("Muon", root.AddressOf(self.data.recoMuon))
            if self.tree_muon.GetBranch("RpcHit"):
                self.tree_muon.SetBranchAddress("RpcHit", root.AddressOf(self.data.recoRpcHit))
            else:
                L1Ana.log.warning("RpcHit branch not present...")


        if self.do_l1menu:
            L1Ana.log.info("Setting branch addresses for L1Menu.")
            self.data.l1menu =  root.L1Analysis.L1AnalysisL1MenuDataFormat()
            self.tree_menu.SetBranchAddress("L1Menu", root.AddressOf(self.data.l1menu))
        if self.do_l1extra:
            L1Ana.log.info("Setting branch addresses for L1Extra.")
            self.data.l1extra = root.L1Analysis.L1AnalysisL1ExtraDataFormat()
            self.tree_extra.SetBranchAddress("L1Extra", root.AddressOf(self.data.l1emuextra))
        if self.do_l1emuextra:
            L1Ana.log.info("Setting branch addresses for L1EmuExtra.")
            self.data.l1emuextra = root.L1Analysis.L1AnalysisL1ExtraDataFormat()
            self.tree_emu_extra.SetBranchAddress("L1Extra", root.AddressOf(self.data.l1emuextra))

    def __len__(self):
        """
        RETURNS: number of entries
        """
        if not self.init:
            L1Ana.log.error("No estimate for elements, yet!")
            return -1
        return self.nentries

    def __getitem__(self, index):
        """
        This is the iterator, it will get the next entry and return the updated L1Data container
        TAKES: index of next event
        RETURNS: L1Data object with connected with L1Ntuple content
        """
        if not self.init:
            L1Ana.log.error("L1Ntuple is not yet initialized! Aborting iteration.")
            raise IndexError("L1Ntuple is not yet initialized!")
        if not index < self.nevents:
            raise IndexError("Reached the end")

        self.tree_main.GetEntry(index)
        return self.data


