#!/usr/bin/env python
from ToolBox import parse_options_and_init_log
# have to do this first or ROOT masks the -h messages
opts = parse_options_and_init_log()

from L1Analysis import L1Ana, L1Ntuple
from analysis_tools.plotting import HistManager
from analysis_tools.selections import MuonSelections, Matcher
import ROOT as root

def set_root_style():
    markers = [root.kFullTriangleUp, root.kFullTriangleDown]
    root.gStyle.SetTitleFont(font)
    root.gStyle.SetStatFont(font)
    root.gStyle.SetTextFont(font)
    root.gStyle.SetLabelFont(font)
    root.gStyle.SetLegendFont(font)
    root.gStyle.SetMarkerStyle(20)
    root.gStyle.SetOptStat(0)
    root.gStyle.SetOptFit(0)
    root.gStyle.SetOptTitle(0)
    root.gPad.SetTopMargin(0.08)
    root.gPad.SetLeftMargin(0.14)
    root.gPad.SetRightMargin(0.06)
    root.gPad.SetTickx(1)
    root.gPad.SetTicky(1)


def book_histograms(pt_cuts):
    # define pt binning
    pt_bins = range(0, 60, 2)
    pt_bins += range(60, 80, 5)
    pt_bins += range(80, 100, 10)
    pt_bins.append(100)

    vars_bins = [['pt', -1]+pt_bins, ['eta', 60, -3., 3.], ['phi', 35, -3.5, 3.5]]

    varnames = []
    binnings = {}

    for pt_min in pt_cuts:
        gen_pt_min = pt_min
        gen_cut_str = '_ptmin'+str(gen_pt_min)
        cut_str = '_ptmin'+str(pt_min)

        varnames.append('n_gen_muons'+gen_cut_str)
        varnames.append('n_gmt_muons'+cut_str) 
        varnames.append('n_ugmt_muons'+cut_str)
        varnames.append('n_bmtf_ugmt_muons'+cut_str)
        varnames.append('n_omtf_ugmt_muons'+cut_str)
        varnames.append('n_emtf_ugmt_muons'+cut_str)
        varnames.append('n_gen_matched_gmt_muons'+cut_str)
        varnames.append('n_gen_matched_ugmt_muons'+cut_str)
        varnames.append('n_gen_matched_bmtf_ugmt_muons'+cut_str)
        varnames.append('n_gen_matched_omtf_ugmt_muons'+cut_str)
        varnames.append('n_gen_matched_emtf_ugmt_muons'+cut_str)
        varnames.append('n_gmt'+cut_str+'_matched_to_a_gen_muon')
        varnames.append('n_gen_matched_to_a_gmt_muon'+cut_str)
        varnames.append('n_ugmt'+cut_str+'_matched_to_a_gen_muon')
        varnames.append('n_gen_matched_to_a_ugmt_muon'+cut_str)
        varnames.append('ugmt'+cut_str+'_tfLink')
        for var_bin in vars_bins:
            varnames.append('gen_muon'+gen_cut_str+'.'+var_bin[0])
            varnames.append('gmt_muon'+cut_str+'.'+var_bin[0])
            varnames.append('ugmt_muon'+cut_str+'.'+var_bin[0])
            varnames.append('bmtf_ugmt_muon'+cut_str+'.'+var_bin[0])
            varnames.append('omtf_ugmt_muon'+cut_str+'.'+var_bin[0])
            varnames.append('emtf_ugmt_muon'+cut_str+'.'+var_bin[0])
            varnames.append('gen_matched_gmt_muon'+cut_str+'.'+var_bin[0])
            varnames.append('gmt'+cut_str+'_matched_gen_muon.'+var_bin[0])
            varnames.append('best_gen_matched_gmt_muon'+cut_str+'.'+var_bin[0])
            varnames.append('best_gmt'+cut_str+'_matched_gen_muon.'+var_bin[0])
            varnames.append('best_gen_matched_ugmt_muon'+cut_str+'.'+var_bin[0])
            varnames.append('best_ugmt'+cut_str+'_matched_gen_muon.'+var_bin[0])
            varnames.append('best_gen_matched_bmtf_ugmt_muon'+cut_str+'.'+var_bin[0])
            varnames.append('best_bmtf_ugmt'+cut_str+'_matched_gen_muon.'+var_bin[0])
            varnames.append('best_gen_matched_omtf_ugmt_muon'+cut_str+'.'+var_bin[0])
            varnames.append('best_omtf_ugmt'+cut_str+'_matched_gen_muon.'+var_bin[0])
            varnames.append('best_gen_matched_emtf_ugmt_muon'+cut_str+'.'+var_bin[0])
            varnames.append('best_emtf_ugmt'+cut_str+'_matched_gen_muon.'+var_bin[0])
            if not best_only:
                varnames.append('gen_matched_ugmt_muon'+cut_str+'.'+var_bin[0])
                varnames.append('ugmt'+cut_str+'_matched_gen_muon.'+var_bin[0])
                varnames.append('gen_matched_bmtf_ugmt_muon'+cut_str+'.'+var_bin[0])
                varnames.append('bmtf_ugmt'+cut_str+'_matched_gen_muon.'+var_bin[0])
                varnames.append('gen_matched_omtf_ugmt_muon'+cut_str+'.'+var_bin[0])
                varnames.append('omtf_ugmt'+cut_str+'_matched_gen_muon.'+var_bin[0])
                varnames.append('gen_matched_emtf_ugmt_muon'+cut_str+'.'+var_bin[0])
                varnames.append('emtf_ugmt'+cut_str+'_matched_gen_muon.'+var_bin[0])

        binnings['n_gen_muons'+gen_cut_str] = [10, 0, 10, 'n_gen_muons'+gen_cut_str]
        binnings['n_gmt_muons'+cut_str] = [10, 0, 10, 'n_gmt_muons'+cut_str]
        binnings['n_ugmt_muons'+cut_str] = [10, 0, 10, 'n_ugmt_muons'+cut_str]
        binnings['n_bmtf_ugmt_muons'+cut_str] = [10, 0, 10, 'n_bmtf_ugmt_muons'+cut_str]
        binnings['n_omtf_ugmt_muons'+cut_str] = [10, 0, 10, 'n_omtf_ugmt_muons'+cut_str]
        binnings['n_emtf_ugmt_muons'+cut_str] = [10, 0, 10, 'n_emtf_ugmt_muons'+cut_str]
        binnings['n_gen_matched_gmt_muons'+cut_str] = [10, 0, 10, 'n_gen_matched_gmt_muons'+cut_str]
        binnings['n_gen_matched_ugmt_muons'+cut_str] = [10, 0, 10, 'n_gen_matched_ugmt_muons'+cut_str]
        binnings['n_gen_matched_bmtf_ugmt_muons'+cut_str] = [10, 0, 10, 'n_gen_matched_bmtf_ugmt_muons'+cut_str]
        binnings['n_gen_matched_omtf_ugmt_muons'+cut_str] = [10, 0, 10, 'n_gen_matched_omtf_ugmt_muons'+cut_str]
        binnings['n_gen_matched_emtf_ugmt_muons'+cut_str] = [10, 0, 10, 'n_gen_matched_emtf_ugmt_muons'+cut_str]
        binnings['n_gmt'+cut_str+'_matched_to_a_gen_muon'] = [5, 0, 5, 'n_gmt'+cut_str+'_matched_to_a_gen_muon']
        binnings['n_gen_matched_to_a_gmt_muon'+cut_str] = [5, 0, 5, 'n_gen_matched_to_a_gmt_muon'+cut_str]
        binnings['n_ugmt'+cut_str+'_matched_to_a_gen_muon'] = [5, 0, 5, 'n_ugmt'+cut_str+'_matched_to_a_gen_muon']
        binnings['n_gen_matched_to_a_ugmt_muon'+cut_str] = [5, 0, 5, 'n_gen_matched_to_a_ugmt_muon'+cut_str]
        binnings['ugmt'+cut_str+'_tfLink'] = [3, 0, 3, 'ugmt'+cut_str+'_tfLink']
        for var_bin in vars_bins:
            binnings['gen_muon'+gen_cut_str+'.'+var_bin[0]] = var_bin[1:]+['gen_muon'+gen_cut_str+'.'+var_bin[0]]
            binnings['gmt_muon'+cut_str+'.'+var_bin[0]] = var_bin[1:]+['gmt_muon'+cut_str+'.'+var_bin[0]]
            binnings['ugmt_muon'+cut_str+'.'+var_bin[0]] = var_bin[1:]+['ugmt_muon'+cut_str+'.'+var_bin[0]]
            binnings['bmtf_ugmt_muon'+cut_str+'.'+var_bin[0]] = var_bin[1:]+['bmtf_ugmt_muon'+cut_str+'.'+var_bin[0]]
            binnings['omtf_ugmt_muon'+cut_str+'.'+var_bin[0]] = var_bin[1:]+['omtf_ugmt_muon'+cut_str+'.'+var_bin[0]]
            binnings['emtf_ugmt_muon'+cut_str+'.'+var_bin[0]] = var_bin[1:]+['emtf_ugmt_muon'+cut_str+'.'+var_bin[0]]
            binnings['gen_matched_gmt_muon'+cut_str+'.'+var_bin[0]] = var_bin[1:]+['gen_matched_gmt_muon'+cut_str+'.'+var_bin[0]]
            binnings['gmt'+cut_str+'_matched_gen_muon.'+var_bin[0]] = var_bin[1:]+['gmt'+cut_str+'_matched_gen_muon.'+var_bin[0]]
            binnings['best_gen_matched_gmt_muon'+cut_str+'.'+var_bin[0]] = var_bin[1:]+['best_gen_matched_gmt_muon'+cut_str+'.'+var_bin[0]]
            binnings['best_gmt'+cut_str+'_matched_gen_muon.'+var_bin[0]] = var_bin[1:]+['best_gmt'+cut_str+'_matched_gen_muon.'+var_bin[0]]
            binnings['best_gen_matched_ugmt_muon'+cut_str+'.'+var_bin[0]] = var_bin[1:]+['best_gen_matched_ugmt_muon'+cut_str+'.'+var_bin[0]]
            binnings['best_ugmt'+cut_str+'_matched_gen_muon.'+var_bin[0]] = var_bin[1:]+['best_ugmt'+cut_str+'_matched_gen_muon.'+var_bin[0]]
            binnings['best_gen_matched_bmtf_ugmt_muon'+cut_str+'.'+var_bin[0]] = var_bin[1:]+['best_gen_matched_bmtf_ugmt_muon'+cut_str+'.'+var_bin[0]]
            binnings['best_bmtf_ugmt'+cut_str+'_matched_gen_muon.'+var_bin[0]] = var_bin[1:]+['best_bmtf_ugmt'+cut_str+'_matched_gen_muon.'+var_bin[0]]
            binnings['best_gen_matched_omtf_ugmt_muon'+cut_str+'.'+var_bin[0]] = var_bin[1:]+['best_gen_matched_omtf_ugmt_muon'+cut_str+'.'+var_bin[0]]
            binnings['best_omtf_ugmt'+cut_str+'_matched_gen_muon.'+var_bin[0]] = var_bin[1:]+['best_omtf_ugmt'+cut_str+'_matched_gen_muon.'+var_bin[0]]
            binnings['best_gen_matched_emtf_ugmt_muon'+cut_str+'.'+var_bin[0]] = var_bin[1:]+['best_gen_matched_emtf_ugmt_muon'+cut_str+'.'+var_bin[0]]
            binnings['best_emtf_ugmt'+cut_str+'_matched_gen_muon.'+var_bin[0]] = var_bin[1:]+['best_emtf_ugmt'+cut_str+'_matched_gen_muon.'+var_bin[0]]
            if not best_only:
                binnings['gen_matched_ugmt_muon'+cut_str+'.'+var_bin[0]] = var_bin[1:]+['gen_matched_ugmt_muon'+cut_str+'.'+var_bin[0]]
                binnings['ugmt'+cut_str+'_matched_gen_muon.'+var_bin[0]] = var_bin[1:]+['ugmt'+cut_str+'_matched_gen_muon.'+var_bin[0]]
                binnings['gen_matched_bmtf_ugmt_muon'+cut_str+'.'+var_bin[0]] = var_bin[1:]+['gen_matched_bmtf_ugmt_muon'+cut_str+'.'+var_bin[0]]
                binnings['bmtf_ugmt'+cut_str+'_matched_gen_muon.'+var_bin[0]] = var_bin[1:]+['bmtf_ugmt'+cut_str+'_matched_gen_muon.'+var_bin[0]]
                binnings['gen_matched_omtf_ugmt_muon'+cut_str+'.'+var_bin[0]] = var_bin[1:]+['gen_matched_omtf_ugmt_muon'+cut_str+'.'+var_bin[0]]
                binnings['omtf_ugmt'+cut_str+'_matched_gen_muon.'+var_bin[0]] = var_bin[1:]+['omtf_ugmt'+cut_str+'_matched_gen_muon.'+var_bin[0]]
                binnings['gen_matched_emtf_ugmt_muon'+cut_str+'.'+var_bin[0]] = var_bin[1:]+['gen_matched_emtf_ugmt_muon'+cut_str+'.'+var_bin[0]]
                binnings['emtf_ugmt'+cut_str+'_matched_gen_muon.'+var_bin[0]] = var_bin[1:]+['emtf_ugmt'+cut_str+'_matched_gen_muon.'+var_bin[0]]

    return HistManager(varnames, binnings)

def fill_matched_muons(evt, hm, matched_muons, muon_type='', cut_str=''):
    gmt_type = 1
    if muon_type == 'u': # uGMT
        muon_str = 'ugmt'
    elif muon_type == 'b': # BMTF uGMT
        muon_str = 'bmtf_ugmt'
    elif muon_type == 'o': # OMTF uGMT
        muon_str = 'omtf_ugmt'
    elif muon_type == 'e': # EMTF uGMT
        muon_str = 'emtf_ugmt'
    else: # GMT
        muon_str = 'gmt'
        gmt_type = 0

    for matched_muon in matched_muons:
        # fill histograms with matched (u)gmt muon data
        if gmt_type == 1:
            hm.fill('gen_matched_'+muon_str+'_muon'+cut_str+'.pt', evt.ugmt.pt[matched_muon[0]])
            hm.fill('gen_matched_'+muon_str+'_muon'+cut_str+'.eta', evt.ugmt.eta[matched_muon[0]])
            hm.fill('gen_matched_'+muon_str+'_muon'+cut_str+'.phi', evt.ugmt.phi[matched_muon[0]])
        else:
            hm.fill('gen_matched_'+muon_str+'_muon'+cut_str+'.pt', evt.gmt.Pt[matched_muon[0]])
            hm.fill('gen_matched_'+muon_str+'_muon'+cut_str+'.eta', evt.gmt.Eta[matched_muon[0]])
            hm.fill('gen_matched_'+muon_str+'_muon'+cut_str+'.phi', Matcher.norm_phi(evt.gmt.Phi[matched_muon[0]]))
        # fill histograms with matched gen muon data
        hm.fill(muon_str+cut_str+'_matched_gen_muon.pt', evt.gen.pt[matched_muon[1]])
        hm.fill(muon_str+cut_str+'_matched_gen_muon.eta', evt.gen.eta[matched_muon[1]])
        hm.fill(muon_str+cut_str+'_matched_gen_muon.phi', evt.gen.phi[matched_muon[1]])

def analyse(evt, hm, pt_min, gen_muon_idcs_cache=[]):
    # USER HOOK
    # do what you want to do with the ntuples here
    # example:
    #print "GMT:", evt.gmt.N
    #print "UGMT:", evt.ugmt.n
    #print "GEN:", evt.gen.pt.size()

    gen_pt_min = pt_min
    gen_cut_str = '_ptmin'+str(gen_pt_min)
    cut_str = '_ptmin'+str(pt_min)

    if len(gen_muon_idcs_cache) < 1:
        gen_muon_idcs_cache = MuonSelections.select_gen_muons(evt.gen)

    # fill the histograms with the generated muon values
    gen_muons_ptmin0p5_idcs = MuonSelections.select_gen_muons(evt.gen, pt_min=0.5, abs_eta_max=2.4, only_pos_eta=only_pos_eta, preselection=gen_muon_idcs_cache)
    gen_muons_idcs = MuonSelections.select_gen_muons(evt.gen, pt_min=gen_pt_min, abs_eta_max=2.4, only_pos_eta=only_pos_eta, preselection=gen_muon_idcs_cache)
    hm.fill('n_gen_muons'+gen_cut_str, len(gen_muons_idcs))
    for gen_muon_index in gen_muons_idcs:
        hm.fill('gen_muon'+gen_cut_str+'.pt', evt.gen.pt[gen_muon_index])
        hm.fill('gen_muon'+gen_cut_str+'.eta', evt.gen.eta[gen_muon_index])
        hm.fill('gen_muon'+gen_cut_str+'.phi', evt.gen.phi[gen_muon_index])

    # fill the histograms with the gmt muon values
    gmt_muons_idcs = MuonSelections.select_gmt_muons(evt.gmt, pt_min=pt_min, only_pos_eta=only_pos_eta)
    hm.fill('n_gmt_muons'+cut_str, len(gmt_muons_idcs))
    for gmt_muon_index in gmt_muons_idcs:
        hm.fill('gmt_muon'+cut_str+'.pt', evt.gmt.Pt[gmt_muon_index])
        hm.fill('gmt_muon'+cut_str+'.eta', evt.gmt.Eta[gmt_muon_index])
        hm.fill('gmt_muon'+cut_str+'.phi', Matcher.norm_phi(evt.gmt.Phi[gmt_muon_index]))

    # fill the histograms with the ugmt muon values
    ugmt_muons_idcs = MuonSelections.select_ugmt_muons(evt.ugmt, pt_min=pt_min, only_pos_eta=only_pos_eta)
    bmtf_ugmt_muons_idcs = MuonSelections.select_ugmt_muons(evt.ugmt, pt_min=pt_min, tftype=[0], only_pos_eta=only_pos_eta)
    omtf_ugmt_muons_idcs = MuonSelections.select_ugmt_muons(evt.ugmt, pt_min=pt_min, tftype=[1], only_pos_eta=only_pos_eta)
    emtf_ugmt_muons_idcs = MuonSelections.select_ugmt_muons(evt.ugmt, pt_min=pt_min, tftype=[2], only_pos_eta=only_pos_eta)
    hm.fill('n_ugmt_muons'+cut_str, len(ugmt_muons_idcs))
    hm.fill('n_bmtf_ugmt_muons'+cut_str, len(bmtf_ugmt_muons_idcs))
    hm.fill('n_omtf_ugmt_muons'+cut_str, len(omtf_ugmt_muons_idcs))
    hm.fill('n_emtf_ugmt_muons'+cut_str, len(emtf_ugmt_muons_idcs))
    for ugmt_muon_index in ugmt_muons_idcs:
        hm.fill('ugmt_muon'+cut_str+'.pt', evt.ugmt.pt[ugmt_muon_index])
        hm.fill('ugmt_muon'+cut_str+'.eta', evt.ugmt.eta[ugmt_muon_index])
        hm.fill('ugmt_muon'+cut_str+'.phi', evt.ugmt.phi[ugmt_muon_index])
        tf = evt.ugmt.tfLink[ugmt_muon_index].tf
        hm.fill('ugmt'+cut_str+'_tfLink', tf)
    for bmtf_ugmt_muon_index in bmtf_ugmt_muons_idcs:
        hm.fill('bmtf_ugmt_muon'+cut_str+'.pt', evt.ugmt.pt[bmtf_ugmt_muon_index])
        hm.fill('bmtf_ugmt_muon'+cut_str+'.eta', evt.ugmt.eta[bmtf_ugmt_muon_index])
        hm.fill('bmtf_ugmt_muon'+cut_str+'.phi', evt.ugmt.phi[bmtf_ugmt_muon_index])
    for omtf_ugmt_muon_index in omtf_ugmt_muons_idcs:
        hm.fill('omtf_ugmt_muon'+cut_str+'.pt', evt.ugmt.pt[omtf_ugmt_muon_index])
        hm.fill('omtf_ugmt_muon'+cut_str+'.eta', evt.ugmt.eta[omtf_ugmt_muon_index])
        hm.fill('omtf_ugmt_muon'+cut_str+'.phi', evt.ugmt.phi[omtf_ugmt_muon_index])
    for emtf_ugmt_muon_index in emtf_ugmt_muons_idcs:
        hm.fill('emtf_ugmt_muon'+cut_str+'.pt', evt.ugmt.pt[emtf_ugmt_muon_index])
        hm.fill('emtf_ugmt_muon'+cut_str+'.eta', evt.ugmt.eta[emtf_ugmt_muon_index])
        hm.fill('emtf_ugmt_muon'+cut_str+'.phi', evt.ugmt.phi[emtf_ugmt_muon_index])

    ##########################################################################
    # match selected gmt muons to selected generated muons
    matched_gmt_muons = Matcher.match_dr(evt.gmt.Eta, evt.gmt.Phi, evt.gen.eta, evt.gen.phi, idcs1=gmt_muons_idcs, idcs2=gen_muons_ptmin0p5_idcs)
    hm.fill('n_gen_matched_gmt_muons'+cut_str, len(matched_gmt_muons))
 
    # how many gmt matches did we find for each generated muon
    for gen_muons_idx in gen_muons_ptmin0p5_idcs:
        gmt_muon_cntr = 0
        histo_filled = False
        for i in range(0, len(matched_gmt_muons)):
            if gen_muons_idx == matched_gmt_muons[i][1]:
                gmt_muon_cntr += 1
                # fill muon values only for the first (and therefore best) match to this gen muon
                if not histo_filled:
                    hm.fill('best_gmt'+cut_str+'_matched_gen_muon.pt', evt.gen.pt[matched_gmt_muons[i][1]])
                    hm.fill('best_gmt'+cut_str+'_matched_gen_muon.eta', evt.gen.eta[matched_gmt_muons[i][1]])
                    hm.fill('best_gmt'+cut_str+'_matched_gen_muon.phi', evt.gen.phi[matched_gmt_muons[i][1]])
                    histo_filled = True
        hm.fill('n_gmt'+cut_str+'_matched_to_a_gen_muon', gmt_muon_cntr)
    # how many generator matches did we find for each gmt muon
    for gmt_muons_idx in gmt_muons_idcs:
        gen_muon_cntr = 0
        histo_filled = False
        for i in range(0, len(matched_gmt_muons)):
            if gmt_muons_idx == matched_gmt_muons[i][0]:
                gen_muon_cntr += 1
                # fill muon values only for the first (and therefore best) match to this gmt muon
                if not histo_filled:
                    hm.fill('best_gen_matched_gmt_muon'+cut_str+'.pt', evt.gmt.Pt[matched_gmt_muons[i][0]])
                    hm.fill('best_gen_matched_gmt_muon'+cut_str+'.eta', evt.gmt.Eta[matched_gmt_muons[i][0]])
                    hm.fill('best_gen_matched_gmt_muon'+cut_str+'.phi', Matcher.norm_phi(evt.gmt.Phi[matched_gmt_muons[i][0]]))
                    histo_filled = True
        hm.fill('n_gen_matched_to_a_gmt_muon'+cut_str, gen_muon_cntr)

    # fill all matched gmt muons
    if not best_only:
        fill_matched_muons(evt, hm, matched_gmt_muons, cut_str=cut_str)

    ##########################################################################
    # match selected ugmt muons to selected generated muons
    matched_ugmt_muons = Matcher.match_dr(evt.ugmt.eta, evt.ugmt.phi, evt.gen.eta, evt.gen.phi, idcs1=ugmt_muons_idcs, idcs2=gen_muons_ptmin0p5_idcs)
    matched_bmtf_ugmt_muons = Matcher.match_dr(evt.ugmt.eta, evt.ugmt.phi, evt.gen.eta, evt.gen.phi, idcs1=bmtf_ugmt_muons_idcs, idcs2=gen_muons_ptmin0p5_idcs)
    matched_omtf_ugmt_muons = Matcher.match_dr(evt.ugmt.eta, evt.ugmt.phi, evt.gen.eta, evt.gen.phi, idcs1=omtf_ugmt_muons_idcs, idcs2=gen_muons_ptmin0p5_idcs)
    matched_emtf_ugmt_muons = Matcher.match_dr(evt.ugmt.eta, evt.ugmt.phi, evt.gen.eta, evt.gen.phi, idcs1=emtf_ugmt_muons_idcs, idcs2=gen_muons_ptmin0p5_idcs)
    hm.fill('n_gen_matched_ugmt_muons'+cut_str, len(matched_ugmt_muons))
    hm.fill('n_gen_matched_bmtf_ugmt_muons'+cut_str, len(matched_bmtf_ugmt_muons))
    hm.fill('n_gen_matched_omtf_ugmt_muons'+cut_str, len(matched_omtf_ugmt_muons))
    hm.fill('n_gen_matched_emtf_ugmt_muons'+cut_str, len(matched_emtf_ugmt_muons))

    # how many ugmt matches did we find for each generated muon
    for gen_muons_idx in gen_muons_ptmin0p5_idcs:
        ugmt_muon_cntr = 0
        histo_filled = False
        for i in range(0, len(matched_ugmt_muons)):
            if gen_muons_idx == matched_ugmt_muons[i][1]:
                ugmt_muon_cntr += 1
                # fill muon values only for the first (and therefore best) match to this gen muon
                if not histo_filled:
                    hm.fill('best_ugmt'+cut_str+'_matched_gen_muon.pt', evt.gen.pt[matched_ugmt_muons[i][1]])
                    hm.fill('best_ugmt'+cut_str+'_matched_gen_muon.eta', evt.gen.eta[matched_ugmt_muons[i][1]])
                    hm.fill('best_ugmt'+cut_str+'_matched_gen_muon.phi', evt.gen.phi[matched_ugmt_muons[i][1]])
                    tf = evt.ugmt.tfLink[matched_ugmt_muons[i][0]].tf
                    if tf == 0:
                        hm.fill('best_bmtf_ugmt'+cut_str+'_matched_gen_muon.pt', evt.gen.pt[matched_ugmt_muons[i][1]])
                        hm.fill('best_bmtf_ugmt'+cut_str+'_matched_gen_muon.eta', evt.gen.eta[matched_ugmt_muons[i][1]])
                        hm.fill('best_bmtf_ugmt'+cut_str+'_matched_gen_muon.phi', evt.gen.phi[matched_ugmt_muons[i][1]])
                    elif tf == 1:
                        hm.fill('best_omtf_ugmt'+cut_str+'_matched_gen_muon.pt', evt.gen.pt[matched_ugmt_muons[i][1]])
                        hm.fill('best_omtf_ugmt'+cut_str+'_matched_gen_muon.eta', evt.gen.eta[matched_ugmt_muons[i][1]])
                        hm.fill('best_omtf_ugmt'+cut_str+'_matched_gen_muon.phi', evt.gen.phi[matched_ugmt_muons[i][1]])
                    else:
                        hm.fill('best_emtf_ugmt'+cut_str+'_matched_gen_muon.pt', evt.gen.pt[matched_ugmt_muons[i][1]])
                        hm.fill('best_emtf_ugmt'+cut_str+'_matched_gen_muon.eta', evt.gen.eta[matched_ugmt_muons[i][1]])
                        hm.fill('best_emtf_ugmt'+cut_str+'_matched_gen_muon.phi', evt.gen.phi[matched_ugmt_muons[i][1]])
                    histo_filled = True
        hm.fill('n_ugmt'+cut_str+'_matched_to_a_gen_muon', ugmt_muon_cntr)
    # how many generator matches did we find for each ugmt muon
    for ugmt_muons_idx in ugmt_muons_idcs:
        gen_muon_cntr = 0
        histo_filled = False
        for i in range(0, len(matched_ugmt_muons)):
            if ugmt_muons_idx == matched_ugmt_muons[i][0]:
                gen_muon_cntr += 1
                # fill muon values only for the first (and therefore best) match to this ugmt muon
                if not histo_filled:
                    hm.fill('best_gen_matched_ugmt_muon'+cut_str+'.pt', evt.ugmt.pt[matched_ugmt_muons[i][0]])
                    hm.fill('best_gen_matched_ugmt_muon'+cut_str+'.eta', evt.ugmt.eta[matched_ugmt_muons[i][0]])
                    hm.fill('best_gen_matched_ugmt_muon'+cut_str+'.phi', evt.ugmt.phi[matched_ugmt_muons[i][0]])
                    tf = evt.ugmt.tfLink[matched_ugmt_muons[i][0]].tf
                    if tf == 0:
                        hm.fill('best_gen_matched_bmtf_ugmt_muon'+cut_str+'.pt', evt.ugmt.pt[matched_ugmt_muons[i][0]])
                        hm.fill('best_gen_matched_bmtf_ugmt_muon'+cut_str+'.eta', evt.ugmt.eta[matched_ugmt_muons[i][0]])
                        hm.fill('best_gen_matched_bmtf_ugmt_muon'+cut_str+'.phi', evt.ugmt.phi[matched_ugmt_muons[i][0]])
                    elif tf == 1:
                        hm.fill('best_gen_matched_omtf_ugmt_muon'+cut_str+'.pt', evt.ugmt.pt[matched_ugmt_muons[i][0]])
                        hm.fill('best_gen_matched_omtf_ugmt_muon'+cut_str+'.eta', evt.ugmt.eta[matched_ugmt_muons[i][0]])
                        hm.fill('best_gen_matched_omtf_ugmt_muon'+cut_str+'.phi', evt.ugmt.phi[matched_ugmt_muons[i][0]])
                    else:
                        hm.fill('best_gen_matched_emtf_ugmt_muon'+cut_str+'.pt', evt.ugmt.pt[matched_ugmt_muons[i][0]])
                        hm.fill('best_gen_matched_emtf_ugmt_muon'+cut_str+'.eta', evt.ugmt.eta[matched_ugmt_muons[i][0]])
                        hm.fill('best_gen_matched_emtf_ugmt_muon'+cut_str+'.phi', evt.ugmt.phi[matched_ugmt_muons[i][0]])
                    histo_filled = True
        hm.fill('n_gen_matched_to_a_ugmt_muon'+cut_str, gen_muon_cntr)

    # fill all matched ugmt muons
    if not best_only:
        fill_matched_muons(evt, hm, matched_ugmt_muons, 'u', cut_str)
        fill_matched_muons(evt, hm, matched_bmtf_ugmt_muons, 'b', cut_str)
        fill_matched_muons(evt, hm, matched_omtf_ugmt_muons, 'o', cut_str)
        fill_matched_muons(evt, hm, matched_emtf_ugmt_muons, 'e', cut_str)

    return gen_muon_idcs_cache

def plot_histos(hm, cList=[]):
    # draw all histograms in hm
    # cList: list with all canvases
    counter = 0
    for varname in hm.get_varnames():
        cList.append(root.TCanvas(varname, varname, 100, 100, 600, 600))
        cList[len(cList)-1].cd()

        set_root_style()

        histo = hm.get(varname)
        histo.GetYaxis().SetTitleOffset(1.5)
        histo.SetLineWidth(2)
        histo.SetLineColor(root.kBlue)
        histo.Draw('hist')

        tex = root.TLatex()
        tex.SetNDC()
        tex.SetTextFont(font)
        tex.SetTextSize(0.04)
        tex.DrawLatex(0.14, 0.93, 'Simulation, 13 TeV')
        #tex.DrawLatex(0.14, 0.93, 'CMS Simulation, 13 TeV')

        #counter += 1
        #if counter > 2:
        #    break
    return cList

def plot_ratio(name, num, den, hm, cList=[]):
    # cList: list with all canvases
    cList.append(root.TCanvas(name, name, 100, 100, 600, 600))
    cList[len(cList)-1].cd()

    set_root_style()

    histo = hm.get_ratio(num, den)
    histo.GetYaxis().SetTitleOffset(1.5)
    histo.SetLineWidth(2)
    histo.SetLineColor(root.kBlue)
    histo.Draw('hist')

    tex = root.TLatex()
    tex.SetNDC()
    tex.SetTextFont(font)
    tex.SetTextSize(0.04)
    tex.DrawLatex(0.14, 0.93, 'Simulation, 13 TeV')
    #tex.DrawLatex(0.14, 0.93, 'CMS Simulation, 13 TeV')

    cList[len(cList)-1].Modified()
    cList[len(cList)-1].Update()

    return cList

def plot_ratio_stack(num, den, var, hm, cList=[]):
    # cList: list with all canvases
    num_str = num+'.'+var
    den_str = den+'.'+var
    canvas_name = 'ratio_'+num+'_to_'+den_str
    canvas_title = canvas_name
    #check that the canvas does not exist yet
    for canvas in cList:
        if canvas.GetName() == canvas_name:
            return cList
    cList.append(root.TCanvas(canvas_name, canvas_title, 100, 100, 600, 600))
    cList[len(cList)-1].cd()

    set_root_style()

    gmt_ratio_total = hm.get_ratio(num_str.replace('ugmt', 'gmt'), den_str.replace('ugmt', 'gmt'))
    tf_ugmt_ratio_stack = hm.get_ratio_stack([num_str.replace('ugmt', 'emtf_ugmt'), num_str.replace('ugmt', 'omtf_ugmt'), num_str.replace('ugmt', 'bmtf_ugmt')], den_str)[1].GetStack()
    ugmt_ratio_total = hm.get_ratio(num_str, den_str)
    ugmt_ratio_total.GetYaxis().SetTitleOffset(1.5)
    ugmt_ratio_total.GetYaxis().SetTitle('ratio')
    maxBinValue = ugmt_ratio_total.GetBinContent(ugmt_ratio_total.GetMaximumBin())
    if maxBinValue <= 1.:
        ugmt_ratio_total.GetYaxis().SetRangeUser(0., 1.3)
    else:
        ugmt_ratio_total.GetYaxis().SetRangeUser(0., 1.2*maxBinValue)
    ugmt_ratio_total.Draw()
    bmtf_ugmt_ratio = tf_ugmt_ratio_stack[2]
    #bmtf_ugmt_ratio.SetLineWidth(2)
    bmtf_ugmt_ratio.SetLineColor(root.kBlack)
    bmtf_ugmt_ratio.SetFillColor(root.kBlue-4)
    bmtf_ugmt_ratio.Draw('histsame')
    omtf_ugmt_ratio = tf_ugmt_ratio_stack[1]
    #omtf_ugmt_ratio.SetLineWidth(2)
    omtf_ugmt_ratio.SetLineColor(root.kBlack)
    omtf_ugmt_ratio.SetFillColor(root.kGreen-4)
    omtf_ugmt_ratio.Draw('histsame')
    emtf_ugmt_ratio = tf_ugmt_ratio_stack[0]
    #emtf_ugmt_ratio.SetLineWidth(2)
    emtf_ugmt_ratio.SetLineColor(root.kBlack)
    emtf_ugmt_ratio.SetFillColor(root.kRed-4)
    emtf_ugmt_ratio.Draw('histsame')
    ugmt_ratio_total.SetLineWidth(2)
    ugmt_ratio_total.SetLineColor(root.kBlack)
    ugmt_ratio_total.Draw('same')
    gmt_ratio_total.SetLineWidth(2)
    gmt_ratio_total.SetLineColor(root.kCyan)
    gmt_ratio_total.Draw('histsame')
    ugmt_ratio_total.Draw('sameaxis')

    legends.append(root.TLegend(0.67, 0.77, 0.94, 0.90))
    legends[len(legends)-1].SetTextFont(font)
    legends[len(legends)-1].SetTextSize(0.03)
    legends[len(legends)-1].SetBorderSize(0)
    legends[len(legends)-1].SetFillColor(19)
    legends[len(legends)-1].SetFillStyle(0)
    legends[len(legends)-1].SetNColumns(2)
    legends[len(legends)-1].AddEntry(gmt_ratio_total, 'GMT', 'l')
    legends[len(legends)-1].AddEntry(bmtf_ugmt_ratio, 'BMTF', 'f')
    legends[len(legends)-1].AddEntry(ugmt_ratio_total, 'uGMT', 'l')
    legends[len(legends)-1].AddEntry(omtf_ugmt_ratio, 'OMTF', 'f')
    legends[len(legends)-1].AddEntry('empty', '', '')
    legends[len(legends)-1].AddEntry(emtf_ugmt_ratio, 'EMTF', 'f')
    legends[len(legends)-1].Draw('same')

    tex = root.TLatex()
    tex.SetNDC()
    tex.SetTextFont(font)
    tex.SetTextSize(0.04)
    tex.DrawLatex(0.14, 0.93, 'Simulation, 13 TeV')
    #tex.DrawLatex(0.14, 0.93, 'CMS Simulation, 13 TeV')

    cList[len(cList)-1].Modified()
    cList[len(cList)-1].Update()

    return cList

def plot_ratios(hm, cList=[], pt_cuts=[0.5, [0.5]]):
    # generate and draw all ratio plots from hm
    # cList: list with all canvases
    gen_pt_min = pt_cuts[0]
    gen_cut_str = '_ptmin'+str(gen_pt_min)
    for pt_min in pt_cuts[1]:
        cut_str = '_ptmin'+str(pt_min)

        cList = plot_ratio_stack('best_ugmt'+cut_str+'_matched_gen_muon', 'gen_muon'+gen_cut_str, 'pt', hm, cList)
        cList = plot_ratio_stack('best_ugmt'+cut_str+'_matched_gen_muon', 'gen_muon'+gen_cut_str, 'eta', hm, cList)
        cList = plot_ratio_stack('best_ugmt'+cut_str+'_matched_gen_muon', 'gen_muon'+gen_cut_str, 'phi', hm, cList)
        cList = plot_ratio_stack('best_gen_matched_ugmt_muon'+cut_str, 'ugmt_muon'+cut_str, 'pt', hm, cList)
        cList = plot_ratio_stack('best_gen_matched_ugmt_muon'+cut_str, 'ugmt_muon'+cut_str, 'eta', hm, cList)
        cList = plot_ratio_stack('best_gen_matched_ugmt_muon'+cut_str, 'ugmt_muon'+cut_str, 'phi', hm, cList)
        if not best_only:
            cList = plot_ratio_stack('ugmt'+cut_str+'_matched_gen_muon', 'gen_muon'+gen_cut_str, 'pt', hm, cList)
            cList = plot_ratio_stack('ugmt'+cut_str+'_matched_gen_muon', 'gen_muon'+gen_cut_str, 'eta', hm, cList)
            cList = plot_ratio_stack('ugmt'+cut_str+'_matched_gen_muon', 'gen_muon'+gen_cut_str, 'phi', hm, cList)
            cList = plot_ratio_stack('gen_matched_ugmt_muon'+cut_str, 'ugmt_muon'+cut_str, 'pt', hm, cList)
            cList = plot_ratio_stack('gen_matched_ugmt_muon'+cut_str, 'ugmt_muon'+cut_str, 'eta', hm, cList)
            cList = plot_ratio_stack('gen_matched_ugmt_muon'+cut_str, 'ugmt_muon'+cut_str, 'phi', hm, cList)

    return cList

def plot_turn_on_curves(hm, cList, histos=[], pt_cuts=[0.5]):
    colors = [root.kBlue, root.kRed, root.kGreen, root.kMagenta, root.kCyan, root.kYellow, root.kPink, root.kOrange]
    canvas_name = 'turn_on_ugmt'
    canvas_title = canvas_name
    cList.append(root.TCanvas(canvas_name, canvas_title, 100, 100, 600, 600))
    cList[len(cList)-1].cd()
    
    set_root_style()

    legends.append(root.TLegend(0.67, 0.77, 0.94, 0.90))
    drawstring = ''
    for (color, pt_min) in zip(colors, pt_cuts):
        cut_str = '_ptmin'+str(pt_min)
        histos.append(hm.get_ratio('best_ugmt'+cut_str+'_matched_gen_muon.pt', 'gen_muon_ptmin0.5.pt').Clone(canvas_name+cut_str))
        histos[len(histos)-1].GetXaxis().SetTitleOffset(1.1)
        histos[len(histos)-1].GetXaxis().SetTitle('p_{T}^{gen}')
        histos[len(histos)-1].GetYaxis().SetTitleOffset(1.5)
        histos[len(histos)-1].GetYaxis().SetTitle('ratio')
        histos[len(histos)-1].SetLineWidth(2)
        histos[len(histos)-1].SetLineColor(color)
        histos[len(histos)-1].Draw(drawstring)
        drawstring = 'same'
        legends[len(legends)-1].AddEntry(histos[len(histos)-1], str(pt_min)+' GeV', 'l')
    
    legends[len(legends)-1].SetTextFont(font)
    legends[len(legends)-1].SetTextSize(0.03)
    legends[len(legends)-1].SetBorderSize(0)
    legends[len(legends)-1].SetFillColor(19)
    legends[len(legends)-1].SetFillStyle(0)
    #legends[len(legends)-1].SetNColumns(2)
    legends[len(legends)-1].Draw('same')

    tex = root.TLatex()
    tex.SetNDC()
    tex.SetTextFont(font)
    tex.SetTextSize(0.04)
    tex.DrawLatex(0.14, 0.93, 'Simulation, 13 TeV')
    #tex.DrawLatex(0.14, 0.93, 'CMS Simulation, 13 TeV')

    cList[len(cList)-1].Modified()
    cList[len(cList)-1].Update()

    return cList

def calc_total_eff(hm, pt_cuts=[0.5, [0.5]]):
    gen_pt_min = pt_cuts[0]
    gen_cut_str = '_ptmin'+str(gen_pt_min)
    for pt_min in pt_cuts[1]:
        cut_str = '_ptmin'+str(pt_min)

        print 'evts with matched gmt muons '+cut_str+'/ evts with gen muons'+gen_cut_str+':  {0} / {1} = {2}'.format(hm.get('n_gen_matched_gmt_muons'+cut_str).Integral(2, 10), hm.get('n_gen_muons'+gen_cut_str).Integral(2, 10), hm.get('n_gen_matched_gmt_muons'+cut_str).Integral(2, 10)/hm.get('n_gen_muons'+gen_cut_str).Integral(2, 10))
        print 'evts with matched ugmt muons '+cut_str+'/ evts with gen muons'+gen_cut_str+': {0} / {1} = {2}'.format(hm.get('n_gen_matched_ugmt_muons'+cut_str).Integral(2, 10), hm.get('n_gen_muons'+gen_cut_str).Integral(2, 10), hm.get('n_gen_matched_ugmt_muons'+cut_str).Integral(2, 10)/hm.get('n_gen_muons'+gen_cut_str).Integral(2, 10))

def main():
    if batchRun:
        root.gROOT.SetBatch(True)

    L1Ana.init_l1_analysis()
    print ""

    pt_cuts = [0.5, 12, 16, 20, 24, 30]
    # book the histograms
    hm = book_histograms(pt_cuts)

    ntuple = L1Ntuple(opts.nevents)

    if opts.flist:
        ntuple.open_with_file_list(opts.flist)
    if opts.fname:
        ntuple.open_with_file(opts.fname)

    for i, event in enumerate(ntuple):
        if (i+1) % 1000 == 0:
            L1Ana.log.info("Processing event: {n}".format(n=i+1))
        gen_muon_idcs_cache = []
        for pt_min in pt_cuts:
            gen_muon_idcs_cache = analyse(event, hm, pt_min, gen_muon_idcs_cache)

    cList = []
    # plot the histograms
    cList = plot_histos(hm, cList)

    gen_pt_cuts = pt_cuts
    for gen_pt_min in gen_pt_cuts:
        pt_cuts_used = []
        for pt_cut in pt_cuts:
            #if pt_cut < gen_pt_min:
            #    continue
            pt_cuts_used.append(pt_cut)
        # plot the efficiencies
        cList = plot_ratios(hm, cList, [gen_pt_min, pt_cuts_used])
        # calculate some overall efficiencies
        calc_total_eff(hm, [gen_pt_min, pt_cuts_used])
    # plot the turn on curves
    histos = []
    cList = plot_turn_on_curves(hm, cList, histos, pt_cuts)

    # save canvases to root file
    if savePlots:
        output = root.TFile('./l1eff_plots.root', 'recreate')
        output.cd()
        for canvas in cList:
            canvas.Write(canvas.GetName())
        output.Close()

    # wait
    if not batchRun:
        raw_input("Press ENTER to quit.")

if __name__ == "__main__":
    only_pos_eta = True
    savePlots = True
    batchRun = True
    best_only = True
    font = 42
    legends = []
    main()

