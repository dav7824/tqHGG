#!/usr/bin/env python2

import Samples, Path, Util
import os, sys
from Samples import sig_MC_s, bkg_MC_s
from os.path import join, exists


def HistNorm(indir_name, outdir_name, ch):
    print '[INFO] Start normalizing histograms'
    exe = join(Path.dir_bin, 'HistNorm')
    indir = join(Path.dir_2017, indir_name)
    outdir = join(Path.dir_2017, outdir_name)
    normdir = join(Path.dir_2017, 'normfactor')

    if not exists(exe):
        print '[ERROR] Executable doesn\'t exist!'
        sys.exit(1)
    if not exists(indir):
        print '[ERROR] Input dir doesn\'t exist!'
        sys.exit(1)
    if ch!='had' and ch!='lep':
        print '[ERROR] Invalid channel!'
        sys.exit(1)
    Util.CreateDirs(outdir)

    # Command template
    cmd = '{bin} {indir}/{{nt}}.root {normdir}/{{nt}}.root {outdir}/{{nt}}.root'.format(
            bin=exe, indir=indir, normdir=normdir, outdir=outdir)

    # Process signal MC
    for sigtype in sig_MC_s:
        if sigtype[1] != ch:
            continue
        for nt in sig_MC_s[sigtype]:
            print 'Processing:', nt
            fp = os.popen( cmd.format(nt=nt) )
            fp.close()

    # Process bkg MC
    for cat in bkg_MC_s:
        for nt in bkg_MC_s[cat]:
            print 'Processing:', nt
            fp = os.popen( cmd.format(nt=nt) )
            fp.close()

    print '[INFO] Complete normalizing histograms'
# End of function HistNorm


def HistAdd(indir_name, outdir_name, ch):
    print '[INFO] Start adding histograms'
    exe = join(Path.dir_bin, 'HistAdd')
    indir = join(Path.dir_2017, indir_name)
    outdir = join(Path.dir_2017, outdir_name)

    if not exists(exe):
        print '[ERROR] Executable doesn\'t exist!'
        sys.exit(1)
    if not exists(indir):
        print '[ERROR] Input dir doesn\'t exist!'
        sys.exit(1)
    if ch!='had' and ch!='lep':
        print '[ERROR] Invalid channel!'
        sys.exit(1)
    Util.CreateDirs(outdir)

    # Command template
    cmd_1 = 'cp {indir}/{{ori_name}}.root {outdir}/{{new_name}}.root'.format(
            indir=indir, outdir=outdir)
    cmd_2 = '{bin} {{fin_list}} {indir} {outdir}/{{cat}}.root'.format(
            bin=exe, indir=indir, outdir=outdir)
    cmd_2a = '{bin} {{fin_list}} {indir} {outdir}/totbkg.root'.format(
            bin=exe, indir=outdir, outdir=outdir)

    # Process signal MC
    for sigtype in sig_MC_s:
        if sigtype[1] != ch:
            continue
        cat = ''.join(sigtype)
        print 'Processing:', cat
        if len( sig_MC_s[sigtype] ) == 1:
            os.system( cmd_1.format(ori_name=sig_MC_s[sigtype][0], new_name=cat) )
        else:
            samples = ','.join(sig_MC_s[sigtype])
            fp = os.popen( cmd_2.format(fin_list=samples, cat=cat) )
            fp.close()

    # Process bkg MC
    ls_bkg = []
    for cat in bkg_MC_s:
        print 'Processing:', cat
        if len( bkg_MC_s[cat] ) == 1:
            os.system( cmd_1.format(ori_name=bkg_MC_s[cat][0], new_name=cat) )
        else:
            samples = ','.join(bkg_MC_s[cat])
            fp = os.popen( cmd_2.format(fin_list=samples, cat=cat) )
            fp.close()
        ls_bkg.append(cat)
    # Process all bkg MC
    print 'Processing: total bkg'
    bkgs = ','.join(ls_bkg)
    fp = os.popen( cmd_2a.format(fin_list=bkgs) )
    fp.close()

    print '[INFO] Complete adding histograms'
# End of function HistAdd


def HistPlotter(indir_name, data_dir_name, outdir_name, ch, draw_sig=True, plot_dom=True, use_logy=False):
    print '[INFO] Painting histograms'
    exe = join(Path.dir_bin, 'HistPlotter')
    indir = join(Path.dir_2017, indir_name)
    outdir = join(Path.dir_2017, outdir_name)

    if not exists(exe):
        print '[ERROR] Executable doesn\'t exist!'
        sys.exit(1)
    if not exists(indir):
        print '[ERROR] Input dir doesn\'t exist!'
        sys.exit(1)
    if ch!='had' and ch!='lep':
        print '[ERROR] Invalid channel!'
        sys.exit(1)
    Util.CreateDirs(outdir)

    ls_fbkgs = []
    ls_legbkgs = []
    for cat in Samples.bkg_MC_stack_order:
        ls_fbkgs.append(cat)
        ls_legbkgs.append(Samples.bkg_MC_leg[cat])
    fbkgs = ','.join(ls_fbkgs)
    legbkgs = ','.join(ls_legbkgs)

    if draw_sig:
        fsigs = 'TT{ch}hct,ST{ch}hct,TT{ch}hut,ST{ch}hut'.format(ch=ch)
        legsigs = 'TT_hct,ST_hct,TT_hut,ST_hut'
    else:
        fsigs = 'none'
        legsigs = 'none'

    ftotbkg = join(indir, 'totbkg.root')
    fdata = join(Path.dir_2017, data_dir_name, 'data.root')

    if plot_dom:
        plot_dom = 1
    else:
        plot_dom = 0
    if use_logy:
        use_logy = 1
    else:
        use_logy = 0

    # Command template
    cmd = '{bin} {fbkgs} {legbkgs} {ftotbkg} {fsigs} {legsigs} {fdata} {bkgdir} {sigdir} {outdir} {ch} {plot_dom} {use_logy}'.format(
            bin=exe, fbkgs=fbkgs, legbkgs=legbkgs, ftotbkg=ftotbkg, fsigs=fsigs, legsigs=legsigs, fdata=fdata, bkgdir=indir, sigdir=indir,
            outdir=outdir, ch=ch, plot_dom=plot_dom, use_logy=use_logy)

    fp = os.popen( cmd )
    fp.close()

    print '[INFO] Complete painting histograms'
# End of function HistPlotter


if __name__ == '__main__':
    # Presel_had_phID_btag-L
    #indir_name = 'Presel_had_phID_btag-L__hist/hist_ori_samples'
    #outdir_name = 'Presel_had_phID_btag-L__hist/hist_ori_norm'
    #outdir2_name = 'Presel_had_phID_btag-L__hist/hist_ori_cat'
    #plotdir_name = 'Presel_had_phID_btag-L__hist/stackedplots_ori'
    #plotdir2_name = 'Presel_had_phID_btag-L__hist/stackedplots_log_ori'
    #ch = 'had'

    # Presel_had_phID_btag-L + PU + photon
    #indir_name = 'Presel_had_phID_btag-L__hist/hist_PU-pho_samples'
    #outdir_name = 'Presel_had_phID_btag-L__hist/hist_PU-pho_norm'
    #outdir2_name = 'Presel_had_phID_btag-L__hist/hist_PU-pho_cat'
    #plotdir_name = 'Presel_had_phID_btag-L__hist/stackedplots_PU-pho'
    #plotdir2_name = 'Presel_had_phID_btag-L__hist/stackedplots_log_PU-pho'
    #ch = 'had'

    # Presel_had_phID_btag-L + PU + photon + btag
    #indir_name = 'Presel_had_phID_btag-L__hist/hist_PU-pho-btag_samples'
    #outdir_name = 'Presel_had_phID_btag-L__hist/hist_PU-pho-btag_norm'
    #outdir2_name = 'Presel_had_phID_btag-L__hist/hist_PU-pho-btag_cat'
    #plotdir_name = 'Presel_had_phID_btag-L__hist/stackedplots_PU-pho-btag'
    #plotdir2_name = 'Presel_had_phID_btag-L__hist/stackedplots_log_PU-pho-btag'
    #ch = 'had'

    # Presel_lep_phID
    #indir_name = 'Presel_lep_phID__hist/hist_ori_samples'
    #outdir_name = 'Presel_lep_phID__hist/hist_ori_norm'
    #outdir2_name = 'Presel_lep_phID__hist/hist_ori_cat'
    #plotdir_name = 'Presel_lep_phID__hist/stackedplots_ori'
    #plotdir2_name = 'Presel_lep_phID__hist/stackedplots_log_ori'
    #ch = 'lep'

    # Presel_lep_phID + PU + elecID + elecReco + muonID + muonISO + photon
    #indir_name = 'Presel_lep_phID__hist/hist_PU-e-mu-pho_samples'
    #outdir_name = 'Presel_lep_phID__hist/hist_PU-e-mu-pho_norm'
    #outdir2_name = 'Presel_lep_phID__hist/hist_PU-e-mu-pho_cat'
    #plotdir_name = 'Presel_lep_phID__hist/stackedplots_PU-e-mu-pho'
    #plotdir2_name = 'Presel_lep_phID__hist/stackedplots_log_PU-e-mu-pho'
    #ch = 'lep'

    # Presel_lep_phID + PU + elecID + elecReco + muonID + muonISO + photon + btag
    indir_name = 'Presel_lep_phID__hist/hist_PU-e-mu-pho-btag_samples'
    outdir_name = 'Presel_lep_phID__hist/hist_PU-e-mu-pho-btag_norm'
    outdir2_name = 'Presel_lep_phID__hist/hist_PU-e-mu-pho-btag_cat'
    plotdir_name = 'Presel_lep_phID__hist/stackedplots_PU-e-mu-pho-btag'
    plotdir2_name = 'Presel_lep_phID__hist/stackedplots_log_PU-e-mu-pho-btag'
    ch = 'lep'

    # Run the workflow
    HistNorm(indir_name, outdir_name, ch)
    HistAdd(outdir_name, outdir2_name, ch)
    HistPlotter(outdir2_name, indir_name, plotdir_name, ch)
    HistPlotter(outdir2_name, indir_name, plotdir2_name, ch, use_logy=True)
