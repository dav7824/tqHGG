#!/usr/bin/env python2

import os, sys, re


maindir = '/wk_cms2/mc_cheng/FCNH/CMSSW_8_1_0/src/tqHGG_syst'

cmd = 'cd {indir}; combine -n {idx:03d} datacard_{idx:03d}.txt > output_{idx:03d}.log'


def MakeDatacard(coupling, channel, sig_scale, idx):
    fin = open( os.path.join(maindir,'parametric_{}_{}.txt'.format(channel,coupling)) )
    fout = open( os.path.join(maindir,'scan_{}_{}'.format(channel,coupling),'datacard_{:03d}.txt'.format(idx)), 'w' )

    for line in fin:
        if (not re.match(r'rate ',line)):
            fout.write(line)
            continue
        rates = line.split()
        if channel != 'comb':
            lineOut = 'rate    {:.3f}  {:.3f}  {}  1\n'.format(
                    float(rates[1])*sig_scale, float(rates[2])*sig_scale, rates[3])
        else:
            lineOut = 'rate    {:.3f}  {:.3f}  {}  1    {:.3f}  {:.3f}  {}  1\n'.format(
                    float(rates[1])*sig_scale, float(rates[2])*sig_scale, rates[3],
                    float(rates[5])*sig_scale, float(rates[6])*sig_scale, rates[7])
        fout.write(lineOut)

    fout.close()
    fin.close()
# End of function MakeDatacard


def MakeScan(coupling, channel, br_N=100, br_min=1e-4, br_max=1e-2):
    if not (coupling == 'Hut' or coupling == 'Hct'):
        print '[ERROR] Invalid coupling'
        sys.exit(1)
    if not (channel == 'had' or channel == 'lep' or channel == 'comb'):
        print '[ERROR] Invalid channel'
        sys.exit(1)

    scandir = os.path.join(maindir, 'scan_{}_{}'.format(channel,coupling))
    if not os.path.exists(scandir):
        os.mkdir(scandir)
        os.system( 'cp {}/{} {}'.format(maindir,'tHqhadronic_ws.root',scandir) )
        os.system( 'cp {}/{} {}'.format(maindir,'tHqleptonic_ws.root',scandir) )

    br_int = (br_max - br_min) / br_N
    for i in range(br_N):
        br_scan = br_min + br_int * i
        print 'Processing BR = {:%}'.format(br_scan)
        sig_scale = br_scan / 1e-3
        #MakeDatacard(coupling, channel, sig_scale, i)
        #fp = os.popen( cmd.format(indir=scandir,idx=i) )
        MakeDatacard(coupling, channel, sig_scale, i+200)  # Adjust index for additional BR scan points for hadronic channel
        fp = os.popen( cmd.format(indir=scandir,idx=i+200) )  # Adjust index for additional BR scan points for hadronic channel
        fp.close()
# End of function MakeScan


if __name__ == '__main__':
    #MakeScan('Hut', 'had')
    #MakeScan('Hut', 'lep')
    #MakeScan('Hut', 'comb')
    #MakeScan('Hct', 'had')
    #MakeScan('Hct', 'lep')
    #MakeScan('Hct', 'comb')

    # Additional scan for hadronic channel
    #MakeScan('Hut', 'had', br_min=1e-2, br_max=2e-2)
    #MakeScan('Hct', 'had', br_min=1e-2, br_max=2e-2)
    #MakeScan('Hut', 'had', br_min=2e-2, br_max=3e-2)
    MakeScan('Hct', 'had', br_min=2e-2, br_max=3e-2)
