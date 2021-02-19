#!/usr/bin/env python2
import os, re
import Path

def CreateDir(dir):
    if not os.path.exists(dir):
        os.mkdir(dir)
    else:
        print '[INFO] The directory "{}" already exists.'.format(dir)


def CreateDirs(dirpath):
    if not os.path.exists(dirpath):
        os.makedirs(dirpath)
    else:
        print '[INFO] The dirpath "{}" already exists.'.format(dirpath)


'''
def ReadList(path):
    fin = open(path, 'r')
    ls = []
    for line in fin:
        if line.strip() == '':
            continue
        res = re.match(r'#', line.lstrip())
        if res:
            continue
        ls.append( line.split() )

    fin.close()
    return ls


def GenCode_FillHist(tag):
    fmain = open( os.path.join(Path.dir_src, 'FillHist.cc') )
    fhist = open( os.path.join(Path.dir_tqHGG, 'hist', 'hist_'+tag+'.cc') )
    fout_name = 'FillHist_'+tag+'.cc'
    fout = open( os.path.join(Path.dir_tqHGG, 'tmp', fout_name), 'w' )

    for i in fmain:
        fout.write(i)
        if re.search('INSERT CODE', i):
            for j in fhist:
                fout.write('\t'+j)

    fmain.close()
    fhist.close()
    fout.close()
    print '[INFO] %s is generated' % fout_name


def GenCode_MVAreco_train(fmethod_name, fout_name):
    fmain = open( os.path.join(Path.dir_src, 'MVAreco_train.cc') )
    fmethod = open( fmethod_name )
    fout = open( fout_name, 'w' )

    for i in fmain:
        fout.write(i)
        if re.search('BOOK_MVA_METHOD', i):
            for j in fmethod:
                fout.write('\t'+j)

    fmain.close()
    fmethod.close()
    fout.close()
    print '[INFO] Source code generated:', fout_name

'''
def submit_job(cmd, jobname):
    os.system( '{runner} --command="{cmd}" --name={name}'.format(runner=os.path.join(Path.dir_tqHGG,'qSub/submitJOB.py'), cmd=cmd, name=jobname) )
