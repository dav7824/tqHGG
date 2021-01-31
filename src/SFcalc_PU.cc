#include "TFile.h"
#include "TH1D.h"
#include "TTree.h"
#include "TString.h"
using namespace std;

int main(int argc, char **argv)
{
    // Get input arguments
    TString fin_name = argv[1];
    TString fpu_name = argv[2];

    // Open event file
    TFile *fin = new TFile(fin_name, "update");
    TTree *Tin = (TTree*)fin->Get("T");
    float NPu = 0;
    Tin->SetBranchAddress("EvtInfo.NPu", &NPu);

    // Create output tree
    float SF_pileup = 0;
    TTree *Tout = Tin->CloneTree(0);
    Tout->Branch("SF_pileup", &SF_pileup);

    // Open pileup SF file
    TFile *fpu = new TFile(fpu_name);
    TH1D *hpu = (TH1D*)fpu->Get("puhist");

    // Start event loop
    for (int evt=0; evt<Tin->GetEntries(); ++evt)
    {
        Tin->GetEntry(evt);

        SF_pileup = hpu->GetBinContent((int)NPu);
        Tout->Fill();
    }
    // End evnet loop

    // Save result
    fin->WriteTObject(Tout, "", "Overwrite");
    fin->Close();

    return 0;
}
