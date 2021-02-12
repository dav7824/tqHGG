/*
 * Create a new tree with reweighted events from sideband data.
 * See macro/Data-driven_QCD/note for info about tested functions.
 *
 * Note:
 * RooAbsReal::asTF
 *   Get the TF1 from roofit function.
 *   More info: https://root-forum.cern.ch/t/problem-in-extracting-a-tf1-from-rooabspdf/13704
 */

#include "include/Paths.h"
using namespace RooFit;

TString fin_name = dir_res + "Presel_had_phIDSB/data.root";
TString fout_name = dir_res + "Presel_had_phID_btag-L/Data-driven_QCD.root";
TString fpdf_name = dir_res + "Data-driven_QCD/FakePhotonPdf_poly7.root";

float cut_idmva = -0.7;

void ReweightData()
{
    // Read input tree
    TFile *fin = new TFile( fin_name );
    TTree *Tin = (TTree*)fin->Get("T");
    float Evt_genweight = 0;
    float leadIDMVA = 0;
    float subleadIDMVA = 0;
    Tin->SetBranchAddress("EvtInfo.genweight", &Evt_genweight);
    Tin->SetBranchAddress("DiPhoInfo.leadIDMVA", &leadIDMVA);
    Tin->SetBranchAddress("DiPhoInfo.subleadIDMVA", &subleadIDMVA);

    // Get fake photon PDF
    TFile *fpdf = new TFile( fpdf_name );
    RooWorkspace *w = (RooWorkspace*)fpdf->Get("w");
    // Fake photon RooAbsPdf
    RooAbsPdf *fakepdf_ = w->pdf("pdfFakePhotonIDMVA");
    // Observable
    RooRealVar *id = w->var("id");
    // Parameters
    RooRealVar *a1 = w->var("a1");
    RooRealVar *a2 = w->var("a2");
    RooRealVar *a3 = w->var("a3");
    RooRealVar *a4 = w->var("a4");
    RooRealVar *a5 = w->var("a5");
    RooRealVar *a6 = w->var("a6");
    RooRealVar *a7 = w->var("a7");
    // Fake photon TF1
    TF1 *fakepdf = fakepdf_->asTF(*id, RooArgList(*a1,*a2,*a3,*a4,*a5,*a6,*a7), *id);
    // Integral from lower limit (-0.9) to cut
    float int_low = fakepdf->Integral(-0.9, cut_idmva);

    // Create output tree
    TFile *fout = new TFile( fout_name, "recreate" );
    TTree *Tout = Tin->CloneTree(0);

    // Start event loop
    for (int evt=0; evt<Tin->GetEntries(); ++evt)
    {
        Tin->GetEntry(evt);

        // Find max & min of IDMVA
        float *idmva_max, *idmva_min;
        if (leadIDMVA > subleadIDMVA) {
            idmva_max = &leadIDMVA;
            idmva_min = &subleadIDMVA;
        } else {
            idmva_max = &subleadIDMVA;
            idmva_min = &leadIDMVA;
        }

        // Integral from cut to max IDMVA
        float int_high = fakepdf->Integral(cut_idmva, *idmva_max);
        // Randomly generate a new low MVA ID value above the cut
        *idmva_min = fakepdf->GetRandom(cut_idmva, *idmva_max);
        // Reweight event
        Evt_genweight *= int_high / int_low;

        Tout->Fill();
    } // End event loop

    // Save result
    fout->cd();
    Tout->Write();
    fout->Close();
    fpdf->Close();
    fin->Close();
}
