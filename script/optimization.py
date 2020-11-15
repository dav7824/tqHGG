#!/usr/bin/env python2

import Path, Util, Samples
import os, sys
from os.path import exists, join


# Create list of BDT score cuts
def CreateCutList(max, min, nstep):
    step = (max - min) / float(nstep)
    ls_cuts = []
    for i in range(nstep):
        ls_cuts.append( min+i*step )
    return ls_cuts


# Create fitting summary log
def CreateSummary(fsum):
    out = open(fsum, 'w')
    # First line: name of each column
    line = '{: ^3}    {: ^10}      {: ^10}    {: ^10}    {: ^10}    {: ^10}    {: ^10}    {: ^10}    {: ^10}    {: ^10}    {: ^10}    {: ^10}    {: ^10}\n'.format(
            'Idx', 'BDT_cut', 'N1', 'mean1', 'sigma1', 'N2', 'mean2', 'sigma2', 'N3', 'mean3', 'sigma3', 'total', 'window')
    out.write(line)
    out.close()


# Create fitting summary log for non-res bkg
def CreateSummary_nrbkg(fsum):
    out = open(fsum, 'w')
    # First line: name of each column
    line = '{: ^3}    {: ^10}      {: ^10}    {: ^10}    {: ^10}    {: ^10}    {: ^10}    {: ^10}    {: ^10}\n'.format(
            'Idx', 'BDT_cut', 'N1', 'tau1', 'N2', 'tau2', 'total', 'sideband', 'window')
    out.write(line)
    out.close()


# Fit gaussians for given samples for each cut values
def makeModel_gaus(ch, type, ls_cuts):
    if ch!='had' and ch!='lep':
        print '[ERROR] Invalid channel!'
        sys.exit(1)
    if type!='sig' and type!='bkg':
        print '[ERROR] Invalid sample type!'
        sys.exit(1)
    # Macro
    exe = join(Path.dir_tqHGG, 'macro/optimization', 'makeModel_gaus.cc')
    # Set input dir
    indir = join(Path.dir_2017, 'optimization_%s'%ch, 'output_BDT_v1')
    if not exists(indir):
        print '[ERROR] Input dir does not exist!'
        sys.exit(1)
    # Set output dir
    if type=='sig':
        outdir = join(indir, 'models/signal')
    else:
        outdir = join(indir, 'models/resbkg')
    # Plot dir
    plotdir = join(outdir, 'plots')
    Util.CreateDirs(plotdir)
    # RooWorkspace dir
    wsdir = join(outdir, 'wspace')
    Util.CreateDirs(wsdir)
    # Logs dir
    logdir = join(outdir, 'logs')
    Util.CreateDirs(logdir)
    # Fitting summary
    fsum = join(outdir, 'param.log')
    CreateSummary(fsum)

    # Command template
    # Bash will see: root -l -b -q <exe>\(\"<ch>\",\"<type>\",<idx>,<cut>\)
    # Root will see: <exe>("<ch>","<type>",<idx>,<cut>)
    cmd = 'root -l -b -q {exe}\(\\"{ch}\\",\\"{type}\\",{{cut_idx}},{{BDT_cut}}\)'.format(
            exe=exe, ch=ch, type=type)

    # Run given cut list
    idx = 0
    for val in ls_cuts:
        print 'Processing:    idx={}    cut={}'.format(idx, val)
        log = open( join(logdir,'result_%03d.log'%idx), 'w' )
        fp = os.popen( cmd.format(cut_idx=idx, BDT_cut=val) )
        log.write( fp.read() )    # Redirect stdout to log
        fp.close()
        log.close()
        idx += 1
# End of function makeModel_gaus


# Fit exponentials for given samples for each cut values
def makeModel_exp(ch, ls_cuts):
    if ch!='had' and ch!='lep':
        print '[ERROR] Invalid channel!'
        sys.exit(1)
    # Macro
    exe = join(Path.dir_tqHGG, 'macro/optimization', 'makeModel_exp.cc')
    # Set input dir
    indir = join(Path.dir_2017, 'optimization_%s'%ch, 'output_BDT_v1')
    if not exists(indir):
        print '[ERROR] Input dir does not exist!'
        sys.exit(1)
    # Set output dir
    outdir = join(indir, 'models/nrbkg')
    # Plot dir
    plotdir = join(outdir, 'plots')
    Util.CreateDirs(plotdir)
    # RooWorkspace dir
    wsdir = join(outdir, 'wspace')
    Util.CreateDirs(wsdir)
    # Logs dir
    logdir = join(outdir, 'logs')
    Util.CreateDirs(logdir)
    # Fitting summary
    fsum = join(outdir, 'param.log')
    CreateSummary_nrbkg(fsum)

    # Command template
    # Bash will see: root -l -b -q <exe>\(\"<ch>\",<idx>,<cut>\)
    # Root will see: <exe>("<ch>",<idx>,<cut>)
    cmd = 'root -l -b -q {exe}\(\\"{ch}\\",{{cut_idx}},{{BDT_cut}}\)'.format(
            exe=exe, ch=ch)

    # Run given cut list
    idx = 0
    for val in ls_cuts:
        print 'Processing:    idx={}    cut={}'.format(idx, val)
        log = open( join(logdir,'result_%03d.log'%idx), 'w' )
        fp = os.popen( cmd.format(cut_idx=idx, BDT_cut=val) )
        log.write( fp.read() )    # Redirect stdout to log
        fp.close()
        log.close()
        idx += 1
# End of function makeModel_exp


# Calculate Z_A from fitting results
def calc_ZA(ch):
    if ch!='had' and ch!='lep':
        print '[ERROR] Invalid channel!'
        sys.exit(1)
    # Macro
    exe = join(Path.dir_tqHGG, 'macro/optimization', 'calc_ZA.cc')

    # Command template
    cmd = 'root -l -b -q {exe}\(\\"{ch}\\"\)'.format(exe=exe, ch=ch)
    os.system(cmd)
# End of function calc_ZA


if __name__ == '__main__':
    # Make list of BDT score cuts
    cuts_had = CreateCutList(0.42, -0.15, 100)
    cuts_lep = CreateCutList(0.3, -0.16, 100)

    #makeModel_gaus('had', 'sig', cuts_had)
    #makeModel_gaus('had', 'bkg', cuts_had)
    #makeModel_gaus('lep', 'sig', cuts_lep)
    #makeModel_gaus('lep', 'bkg', cuts_lep)

    #makeModel_exp('had', cuts_had)
    #makeModel_exp('lep', cuts_lep)

    calc_ZA('had')
    calc_ZA('lep')

    print 'Complete!'
