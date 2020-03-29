#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include "TString.h"

#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

int main(int argc, char **argv)
{
    // Get input arguments
    TString fin_name = argv[1];
    TString inTree_name = argv[2];
    TString fout_name = argv[3];

    // Create TTree for input n-tuple
    TChain *inTree = new TTree(inTree_name);
    inTree->Add(fin_name);

    // Create TTree for output n-tuple
    TFile *fout = new TFile(fout_name, "recreate");
    TTree *outTree = new TTree("flashggStdTree", "");

    /* BEGIN: Common variables for input & output TTree */
    /* END: Common variables for input & output TTree */

    /* BEGIN: Variables for input TTree */
    /* END: Variables for input TTree */

    /* BEGIN: Variables for output TTree */
    /* END: Variables for output TTree */

    /* BEGIN: Set branches for input TTree */
    /* END: Set branches for input TTree */

    /* BEGIN: Set branches for output TTree */
    /* END: Set branches for output TTree */

    // Loop over input TTree to do preselection
    for (int evt=0; evt<inTree->GetEntries(); ++evt)
    {
	inTree->GetEntry(evt);

	/* BEGIN: Prepare area A */
	/* END: Prepare area A */

	/* BEGIN: Selection cuts on whole events */
	/* END: Selection cuts on whole events */

	// If the event passes the whole-event cuts, do selection on its physical objects and fill it to output TTree

	/* BEGIN: Reset output branches */
	/* END: Reset output branches */

	/* BEGIN: Prepare area B */
	/* END: Prepare area B */

	/* BEGIN: Select electrons */
	/* END: Select electrons */

	/* BEGIN: Select muons */
	/* END: Select muons */

	/* BEGIN: Select jets */
	/* END: Select jets */

	/* BEGIN: Cleanup */
	/* END: Cleanup */

	// If the event passes the preselection, fill it to output TTree
	outTree->Fill();
	// Process the next event
    } // End of preselection loop over input TTree

    // Write output TTree to output TFile
    fout->cd();
    outTree->Write();
    // Close output TFile
    fout->Close();
}
