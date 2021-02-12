/*
 * Warning about a ROOT behavior:
 * If you create a histogram after openning a TFile, when the file is closed the histogram would be deleted!
 */

#include "include/Paths.h"
#include "include/utility.h"

TString indir = dir_res + "Presel_had/";
TString outdir = dir_res + "Data-driven_QCD/";
TString normdir = dir_res + "normfactor/";

// Main hist
TH1D *hid = 0;

TH1D *CreateHist() {
    return new TH1D("hFakePhotonIDMVA", ";fake #gamma IDMVA;", 38, -0.9, 1.);
}

void GetFakeHist_single(TString nt)
{
    cout << "Processing: " << nt << endl;

    // Read input tree
    TFile *fin = new TFile( indir+nt+".root" );
    TTree *T = (TTree*)fin->Get("T");
    // Set branches
    float genweight = 0;
    float dipho_weight = 0;
    float leadIDMVA = 0;
    float subleadIDMVA = 0;
    int leadGenMatchType = 0;
    int subleadGenMatchType = 0;
    T->SetBranchAddress("EvtInfo.genweight", &genweight);
    T->SetBranchAddress("DiPhoInfo.centralWeight", &dipho_weight);
    T->SetBranchAddress("DiPhoInfo.leadIDMVA", &leadIDMVA);
    T->SetBranchAddress("DiPhoInfo.subleadIDMVA", &subleadIDMVA);
    T->SetBranchAddress("DiPhoInfo.leadGenMatchType", &leadGenMatchType);
    T->SetBranchAddress("DiPhoInfo.subleadGenMatchType", &subleadGenMatchType);

    // Create histogram
    TH1D *hh = CreateHist();

    // Start event loop
    for (int evt=0; evt<T->GetEntries(); ++evt) {
        T->GetEntry(evt);

        if (subleadGenMatchType != 1){
            hh->Fill(subleadIDMVA, genweight*dipho_weight);
            continue;
        }
        else if (leadGenMatchType != 1) {
            hh->Fill(leadIDMVA, genweight*dipho_weight);
        }

    } // End event loop

    // Scale histogram
    float nf = GetNormFactor( (normdir+nt+".root").Data() );
    hh->Scale(nf);

    // Add to the main hist
    hid->Add(hh);

    fin->Close();
}

void GetFakeHist()
{
    // Create output file
    TFile *fout = new TFile(outdir+"FakePhotonHist_GJet.root", "recreate");
    // Create main hist
    hid = CreateHist();

    // Add individual sample
    GetFakeHist_single("GJet_Pt-20to40_MGG-80toInf");
    GetFakeHist_single("GJet_Pt-20toInf_MGG-40to80");
    GetFakeHist_single("GJet_Pt-40toInf_MGG-80toInf");
    /*GetFakeHist_single("QCD_Pt-30to40_MGG-80toInf");
    GetFakeHist_single("QCD_Pt-30toInf_MGG-40to80");
    GetFakeHist_single("QCD_Pt-40toInf_MGG-80toInf");*/

    // Draw hist
    TCanvas *c = new TCanvas();
    hid->SetMinimum(0);
    hid->Draw();
    c->SaveAs( outdir+"FakePhotonHist_GJet.png" );

    // Save hist
    fout->cd();
    hid->Write();
    fout->Close();
}
