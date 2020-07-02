/*
 * Print the yields of each bkg MC category and calculate data/MC difference.
 * Usage:
 *   ./PrintYieldSummary <bkg_filename_list> <bkg_name_list> <bkg_dir> <data.root> <data_tree_name> <output.log>
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
	char *bkg_filename_ = argv[1];  // Files containing yield histograms. The file names do NOT contain ".root"
	char *bkg_name_ = argv[2];      // Sample names to be printed in output log
	TString bkg_dir = argv[3];      // Path of bkg files' directory
	TString data_path = argv[4];    // Path of the file containing data tree
	TString data_tree_name = argv[5];
	TString log_path = argv[6];     // Path of output log

	vector<TString> bkg_filename, bkg_name;
	ParseCStringList(bkg_filename_, bkg_filename);
	ParseCStringList(bkg_name_, bkg_name);
	int Nbkgs = bkg_filename.size();

	// Calculate data yield
	TFile *fdata = new TFile(data_path);
	float yield_data = (float)((TTree*)fdata->Get(data_tree_name))->GetEntries();
	float yield_data_err = sqrt(yield_data);
	fdata->Close();

	// Calculate bkg MC yields
	vector<float*> yield_bkg(Nbkgs);
	float yield_bkg_tot = 0;
	float yield2_bkg_tot = 0;
	float yield_bkg_tot_err = 0;
	for (int i=0; i<Nbkgs; ++i) {
		TString filename = bkg_dir+"/"+bkg_filename[i]+".root";
		yield_bkg[i] = new float[2];
		GetYieldInfo(filename.Data(), yield_bkg[i]);
		yield_bkg_tot += yield_bkg[i][0];
		yield2_bkg_tot += yield_bkg[i][1];
	}
	yield_bkg_tot_err = sqrt(yield2_bkg_tot);

	// Calculate difference btw data & MC
	float ratio = 1 - yield_bkg_tot / yield_data;  // Define data-MC diff ratio: (data-MC)/data = 1 - MC/data
	// Err of 1-(MC/data) = err of MC/data
	// Error propagation of multiplication: c = a*b -> (dc / c)^2 = (da / a)^2 + (db / b)^2    (assuming a and b are independent variables)
	float ratio_err = sqrt( pow(ratio,2) * (pow(yield_data_err/yield_data,2) + pow(yield_bkg_tot_err/yield_bkg_tot,2)) );

	// Print results
	char format[512] = "%35s\t\t%20f +- %15f\n";
	FILE *output = fopen(log_path.Data(), "w");
	for (int i=0; i<Nbkgs; ++i) {
		fprintf(output, format, bkg_name[i].Data(), yield_bkg[i][0], sqrt(yield_bkg[i][1]));
	}
	fprintf(output, format, "Total MC", yield_bkg_tot, yield_bkg_tot_err);
	fprintf(output, format, "Data", yield_data, yield_data_err);
	fprintf(output, format, "(data-MC)/data", ratio, ratio_err);
	fclose(output);

	return 0;
}
