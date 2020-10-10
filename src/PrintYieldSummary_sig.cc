/*
 * Print the yields of each sig MC category.
 * Usage:
 *   ./PrintYieldSummary_sig <sig_file_list> <sig_name_list> <sig_dir>
 */

#include "include/utility.h"

#include "TString.h"

#include <cstdio>
#include <vector>
#include <cmath>
using namespace std;

int main(int argc, char **argv)
{
    // Get command line arguments
    char *fileList = argv[1];       // Files containing yield histograms. The file names do NOT contain ".root"
    char *nameList = argv[2];       // Sample names to be printed in summary
    TString indir = argv[3];        // Input directory

    vector<TString> fins, samples;
    ParseCStringList(fileList, fins);
    ParseCStringList(nameList, samples);
    int Nfin = fins.size();

    // Calculate MC yields
    vector<float*> yield_sig(Nfin);
    for (int i=0; i<Nfin; ++i) {
        TString filename = indir+"/"+fins[i]+".root";
        yield_sig[i] = new float[2];
        GetYieldInfo(filename.Data(), yield_sig[i]);
    }

    // Print results
    char format[512] = "%35s\t\t%15.2f +- %15.2f\n";
    for (int i=0; i<Nfin; ++i) {
        printf(format, samples[i].Data(), yield_sig[i][0], sqrt(yield_sig[i][1]));
    }

    return 0;
}
