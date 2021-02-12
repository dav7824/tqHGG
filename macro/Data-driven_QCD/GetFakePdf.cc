/*
 * Fit fake photon MVA ID histogram for PDF.
 */

#include "include/Paths.h"
using namespace RooFit;

TString indir = dir_res + "Data-driven_QCD/";

void GetFakePdf()
{
    // Read histogram
    TFile *fin = new TFile( indir+"FakePhotonHist_GJet.root" );
    TH1D *hh = (TH1D*)fin->Get("hFakePhotonIDMVA");

    // Create roofit obj
    RooRealVar id("id", "#gamma MVA ID", -0.9, 1.);
    RooDataHist hid("hid", "Fake photon IDMVA distribution", id, hh);
    // Plot hist
    RooPlot *idframe = id.frame();
    hid.plotOn(idframe);

    // Const variables
    RooConstVar rc0("rc0", "const 0", 0);
    RooConstVar rc1("rc1", "const 1", 1);
    // Construct PDF
    RooRealVar a1("a1", "a1", 0, -1e6, 1e6);
    RooRealVar a2("a2", "a2", 0, -1e6, 1e6);
    RooRealVar a3("a3", "a3", -1, -1e6, 1e6);
    RooRealVar a4("a4", "a4", 0, -1e6, 1e6);
    RooRealVar a5("a5", "a5", 0, -1e6, 1e6);
    RooRealVar a6("a6", "a6", 2, -1e6, 1e6);
    RooRealVar a7("a7", "a7", 0, -1e6, 1e6);
    RooRealVar a8("a8", "a8", 2, -1e6, 1e6);
    RooRealVar a9("a9", "a9", -1, -1e6, 1e6);
    RooPolynomial pdffake("pdfFakePhotonIDMVA", "fake photon MVA ID PDF", id,
            RooArgList(a1,a2,a3,a4,a5,a6));

    // Fit
    RooFitResult *res = pdffake.fitTo( hid, Save(true), SumW2Error(true) );
    res->Print("v");
    pdffake.plotOn(idframe);
    // (For extended likelihood fit)
    //pdffake.plotOn(idframe, Normalization(1,RooAbsReal::Relative));

    // Get fitted NLL value
    RooAbsReal *nll = pdffake.createNLL(hid);
    cout << "[INFO] NLL = " << nll->getVal() << endl;

    // Save plot
    TCanvas *c = new TCanvas();
    idframe->Draw();
    c->SaveAs( indir+"FakePhotonPdf_poly6.png" );

    // Save PDF
    RooWorkspace *w = new RooWorkspace("w", "workspace");
    w->import(pdffake);
    w->writeToFile( indir+"FakePhotonPdf_poly6.root" );

    fin->Close();
}
