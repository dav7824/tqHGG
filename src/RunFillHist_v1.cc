#include "HistVar.h"
#include "TreeReader.h"
#include "utility.h"

#include "TH1D.h"
#include "TFile.h"

#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
using namespace std;

void CreateHist(TH1D**);
void FillHist(TreeReader&, TH1D**, const string&);

int main(int argc, char **argv)
{
    // get command line arguments
    char *name = argv[1];
    vector<string> path;    ParseCStringList(argv[2], path);
    char *treeName = argv[3];
    char *outFile = argv[4];
    string type = argv[5];
    if (type != "data" && type != "mc") {
	cout << "[ERROR] Unknown type for the dataset\n";
	exit(1);
    }

    cout << "Start processing n-tuple: " << name << endl << endl;

    // get input tree
    TreeReader inTree(treeName);
    for (int i=0; i<path.size(); ++i) {
	cout << "Adding file to TreeReader: " << path.at(i) << endl;
	inTree.Add(path.at(i).c_str());
    }
    cout << endl;
    inTree.Init();

    // create histograms
    TFile *fout = new TFile(outFile, "recreate");
    TH1D *hist[nhist];
    CreateHist(hist);

    // fill histograms
    FillHist(inTree, hist, type);

    // save the histograms
    fout->cd();
    for (int i=0; i<nhist; ++i) hist[i]->Write();
    fout->Close();
    cout << "\nOutput is saved at: " << outFile << endl;

    return 0;
}
