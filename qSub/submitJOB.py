#!/usr/bin/env python2
# Execute one command by default.
# Usage:
#  in command line
#    ./submitJOB.py --command='cmsRun myJob.py && doSomeThing' --name='myQJobSent'
#
#  in python ( to use for loop to submit jobs )
#    os.system( './submitJOB.py --command={} --name={}'.format(command.format(...), 'job_{:02d}'.foramt(...)) )
#
# Note, you need to check if qsub can write to the output directories.


# Output directories & name of user
defaultStorageFolder = '/wk_cms2/mc_cheng/public/tqHGG/qSubResult/'
defaultMessageFolder = '/wk_cms2/mc_cheng/public/tqHGG/qSubMessage/'
defaultErrorFolder   = '/wk_cms2/mc_cheng/public/tqHGG/qSubMessage/'
USER = 'mc_cheng'


###############################
# Template file to be submitted
###############################
submitTemplate = '''
#!/usr/bin/env sh

#PBS -V
#PBS -j oe
#PBS -q cms
#PBS -d {0}
#PBS -o {1}
#PBS -e {2}

set -eo pipefail

# Setup CMSSW env
cd {3} && eval `scramv1 runtime -sh` && cd {4}
# Commands to be run
{5}
'''


def getCMSSWVersion():
    import commands
    baseDir = commands.getstatusoutput( 'echo $CMSSW_BASE' )
    if baseDir[0]:
        print '[ERROR] You haven\'t set CMSSW environment'
        exit()
    print '[INFO] Current CMSSW env: {0}'.format( baseDir[1] )
    return '{}/src'.format( baseDir[1] )


def getCurrentPath():
    import commands
    return commands.getoutput('echo $PWD')


# Parser for the script
def addOption():
    import argparse
    parser = argparse.ArgumentParser( description='' )
    parser.add_argument(
            '--command', '-c', type=str, default=None,
            help='The command to be executed'
            )
    parser.add_argument(
            '--name', '-N', type=str, default='job',
            help='Job name'
            )
    parser.add_argument(
            '--user', '-u', type=str, default=USER,
            help='User name'
            )
    parser.add_argument(
            '--lowPriority', action='store_true',
            help=''
            )
    return parser.parse_args()


# Main part of the script
if __name__ == '__main__':
    args = addOption()
    if not args.command:
        print '[ERROR] You have to specify the options! See [--help].'
        exit()

    cmsEnv = getCMSSWVersion()
    pwd = getCurrentPath()

    jobfile = '/wk_cms2/mc_cheng/FCNH/CMSSW_9_4_11/src/tqHGG_DeepJet/tqHGG/tmp/qjob_submit.sh'
    file = open( jobfile, 'w' )
    file.write( submitTemplate.format(defaultStorageFolder, defaultMessageFolder, defaultErrorFolder, cmsEnv, pwd, args.command) )
    file.close()

    import os
    if args.lowPriority:
        os.system( 'qsub {} -N {} -p -500'.format(jobfile, args.name) )
    else:
        os.system( 'qsub {} -N {} -p 1'.format(jobfile, args.name) )

    print '[INFO] End of job submit of "{}".'.format(args.name)
