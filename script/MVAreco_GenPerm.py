#!/usr/bin/env python2
# Run MVAreco_GenPerm.

import Path
import Util
import os

# Process bkg MC & data
indir_had = Path.dir_2017+'/Presel_had_btag-M_phID'
outdir_had = Path.dir_2017+'/MVAreco_perm_had_btag-M_phID'
indir_lep = Path.dir_2017+'/Presel_lep_btag-L_phID'
outdir_lep = Path.dir_2017+'/MVAreco_perm_lep_btag-L_phID'

f_bkg = Path.dir_list+'/datasets_2017.txt'
exe = '{}/MVAreco_GenPerm'.format( Path.dir_bin )

Util.CreateDir( outdir_had )
Util.CreateDir( outdir_lep )
ls_bkg = Util.ReadList( f_bkg )
n_job = 0

for nt in ls_bkg:
	filename = nt[0]+'.root'
	cmd = exe + ' {} T {} bkg {} 2017 {}'.format(indir_had+'/'+filename, outdir_had+'/'+filename, 'had', 'medium')
	print 'Running: {}'.format(cmd)
	os.system( cmd )
	cmd = exe + ' {} T {} bkg {} 2017 {}'.format(indir_lep+'/'+filename, outdir_lep+'/'+filename, 'lep', 'loose')
	print 'Running: {}'.format(cmd)
	os.system( cmd )
	n_job += 2

print '[INFO] Completed {} jobs'.format(n_job)

# Process signal MC
'''
ls_sig_TThad = ['TT_FCNC-TtoHJ_aThad_hct.root', 'TT_FCNC-TtoHJ_aThad_hut.root', 'TT_FCNC-aTtoHJ_Thad_hct.root', 'TT_FCNC-aTtoHJ_Thad_hut.root']
ls_sig_SThad = ['ST_FCNC-TH_Thad_hct.root', 'ST_FCNC-TH_Thad_hut.root']
ls_sig_TTlep = ['TT_FCNC-TtoHJ_aTlep_hct.root', 'TT_FCNC-TtoHJ_aTlep_hut.root', 'TT_FCNC-aTtoHJ_Tlep_hct.root', 'TT_FCNC-aTtoHJ_Tlep_hut.root']
ls_sig_STlep = ['ST_FCNC-TH_Tlep_hct.root', 'ST_FCNC-TH_Tlep_hut.root']

n_job = 0
indir = Path.dir_2017 + '/Presel_had_btag-M_phID'
outdir = Path.dir_2017 + '/MVAreco_perm_had_btag-M_phID'
Util.CreateDir(outdir)
for nt in ls_sig_TThad:
	cmd = '{}/MVAreco_GenPerm {} T {} {} {} 2017 {}'.format(Path.dir_bin, indir+'/'+nt, outdir+'/'+nt, 'TT', 'had', 'medium')
	print 'Running: {}'.format(cmd)
	os.system( cmd )
	n_job += 1
for nt in ls_sig_SThad:
	cmd = '{}/MVAreco_GenPerm {} T {} {} {} 2017 {}'.format(Path.dir_bin, indir+'/'+nt, outdir+'/'+nt, 'ST', 'had', 'medium')
	print 'Running: {}'.format(cmd)
	os.system( cmd )
	n_job += 1

indir = Path.dir_2017 + '/Presel_lep_btag-L_phID'
outdir = Path.dir_2017 + '/MVAreco_perm_lep_btag-L_phID'
Util.CreateDir(outdir)
for nt in ls_sig_TTlep:
	cmd = '{}/MVAreco_GenPerm {} T {} {} {} 2017 {}'.format(Path.dir_bin, indir+'/'+nt, outdir+'/'+nt, 'TT', 'lep', 'loose')
	print 'Running: {}'.format(cmd)
	os.system( cmd )
	n_job += 1
for nt in ls_sig_STlep:
	cmd = '{}/MVAreco_GenPerm {} T {} {} {} 2017 {}'.format(Path.dir_bin, indir+'/'+nt, outdir+'/'+nt, 'ST', 'lep', 'loose')
	print 'Running: {}'.format(cmd)
	os.system( cmd )
	n_job += 1

print '[INFO] Completed {} jobs'.format(n_job)
'''
