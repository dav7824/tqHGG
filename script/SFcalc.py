#!/usr/bin/env python2

import Path, Util, Samples
import os, sys
from os.path import join, exists

def RunSF_elec( filename ):
	fp = os.popen( cmd_SF_elec.format(file=filename) )
	print fp.read()
	fp.close()
def RunSF_muon( filename ):
	fp = os.popen( cmd_SF_muon.format(file=filename) )
	print fp.read()
	fp.close()

# I/O directories
indir = join(Path.dir_2017, sys.argv[1])
if not exists(indir):
	print '[ERROR] Input dir doesn\'t exist!'
	sys.exit(1)

# SF root files
SFdir = join(Path.dir_2017, 'SF')
felec_reco0 = join(SFdir, 'egammaEffi.txt_EGM2D_runBCDEF_passingRECO_lowEt.root')
felec_reco1 = join(SFdir, 'egammaEffi.txt_EGM2D_runBCDEF_passingRECO.root')
felec_id = join(SFdir, '2017_ElectronMedium.root')
fmuon_id = join(SFdir, 'RunBCDEF_SF_ID.root')
fmuon_iso = join(SFdir, 'RunBCDEF_SF_ISO.root')

# Executables
exe_SF_elec = join(Path.dir_bin, 'SFcalc_Electron')
exe_SF_muon = join(Path.dir_bin, 'SFcalc_Muon')

# Electron SF command template
cmd_SF_elec = '{bin} {indir}/{{file}} {fSF_reco0} {fSF_reco1} {fSF_ID}'.format(
		bin=exe_SF_elec, indir=indir, fSF_reco0=felec_reco0, fSF_reco1=felec_reco1, fSF_ID=felec_id
	)
# Muon SF command template
cmd_SF_muon = '{bin} {indir}/{{file}} {fSF_ID} {fSF_ISO} '.format(
		bin=exe_SF_muon, indir=indir, fSF_ID=fmuon_id, fSF_ISO=fmuon_iso
	)

# Get input file list
files = os.listdir(indir)

# Run executables on input root files
for file in files:
	if not file.endswith('.root'):
		continue
	print 'Processing:', file
	#RunSF_elec(file)
	RunSF_muon(file)

print 'Complete!'
