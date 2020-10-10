/*
 * Print the yields of each bkg MC category and calculate data/MC difference.
 * Usage:
 *   ./PrintYieldSummary <bkg_file_list> <bkg_name_list> <bkg_dir> <data.root>
 */

#include "include/utility.h"

#include "TString.h"
#include "TFile.h"

#include <cstdio>
#include <vector>
#include <cmath>
using namespace std;

int main(int argc, char **argv)
{
    // Get command line arguments
    char *fileList = argv[1];       // Files containing yield histograms. The file names do NOT contain ".root"
    char *nameList = argv[2];       // Sample names to be printed in summary
    TString indir = argv[3];        // Path of bkg files' directory
    TString data_name = argv[4];    // Path of the file containing data tree

    vector<TString> fins, samples;
    ParseCStringList(fileList, fins);
    ParseCStringList(nameList, samples);
    int Nbkgs = fins.size();

    // Calculate data yield
    TFile *fdata = new TFile(data_name);
    float yield_data = (float)((TTree*)fdata->Get("T"))->GetEntries();
    float yield_data_err = sqrt(yield_data);
    fdata->Close();

    // Calculate bkg MC yields
    vector<float*> yield_bkg(Nbkgs);
    float yield_bkg_tot = 0;
    float yield2_bkg_tot = 0;
    float yield_bkg_tot_err = 0;
    for (int i=0; i<Nbkgs; ++i) {
        TString filename = indir+"/"+fins[i]+".root";
        yield_bkg[i] = new float[2];
        GetYieldInfo(filename.Data(), yield_bkg[i]);
        yield_bkg_tot += yield_bkg[i][0];
        yield2_bkg_tot += yield_bkg[i][1];
    }
    yield_bkg_tot_err = sqrt(yield2_bkg_tot);

    // Calculate difference btw data & MC
    float ratio = yield_bkg_tot / yield_data - 1.;  // Define data-MC diff ratio: (MC-data)/data = MC/data - 1
    // Err of (MC/data) - 1 = err of MC/data
    // Error propagation of multiplication: c = a*b -> (dc / c)^2 = (da / a)^2 + (db / b)^2    (assuming a and b are independent of each other)
    float ratio_err = sqrt( pow(yield_data_err/yield_data,2) + pow(yield_bkg_tot_err/yield_bkg_tot,2) ) * (yield_bkg_tot/yield_data);

    // Print results on stdout
    char format[512] = "%35s\t\t%15.2f +- %15.2f\n";
    for (int i=0; i<Nbkgs; ++i) {
        printf(format, samples[i].Data(), yield_bkg[i][0], sqrt(yield_bkg[i][1]));
    }
    printf(format, "Total MC bkg", yield_bkg_tot, yield_bkg_tot_err);
    printf(format, "Data", yield_data, yield_data_err);
    printf(format, "(MC-data) / data", ratio, ratio_err);

    return 0;
}
