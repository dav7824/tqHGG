#!/usr/bin/env python2

import os, sys
from os.path import join

def Run(nt):
    exe = 'SFcalc_btag'
    fin = join('/wk_cms2/mc_cheng/public/tqHGG/2017_v2/Presel_had_phID_btag-L', nt+'.root')
    fout = 'test_'+nt+'.root'
    fcsv = 'DeepCSV_94XSF_V5_B_F.csv'

    os.system('{} {} {} {}'.format(exe, fin, fcsv, fout))
    print 'Complete!'


def RunPrint(nt):
    exe = 'SFcalc_print'
    fin = join('/wk_cms2/mc_cheng/public/tqHGG/2017_v2/Presel_had_phID_btag-L', nt+'.root')
    fcsv = 'DeepCSV_94XSF_V5_B_F.csv'

    log = open('Print_{}.log'.format(nt), 'w')
    fp = os.popen('./{} {} {}'.format(exe, fin, fcsv))
    log.write(fp.read())
    fp.close()
    log.close()


if __name__ == '__main__':
    #Run('DiPhotonJetsBox')
    #Run('TT_FCNC-TtoHJ_aThad_hct')
    #RunPrint('DiPhotonJetsBox')
    RunPrint('TT_FCNC-TtoHJ_aThad_hct')
