#include "include/Paths.h"

float get_ZA(float ysig, float ybkg) {
    return TMath::Sqrt( 2*( (ysig+ybkg)*TMath::Log(1+ysig/ybkg) - ysig ) );
}

void ScanZA(TString channel)
{
    TString indir = dir_res;
    TString outdir;
    int Ncut;

    // Set paths
    if (channel == "had") {
        indir += "optimization_had/";
        Ncut = 98;
    } else if (channel == "lep") {
        indir += "optimization_lep/";
        Ncut = 100;
    } else {
        std::cout << "[ERROR] Invalid channel\n";
        std::exit(1);
    }
    outdir = indir + "significance/";
    if (gSystem->AccessPathName(outdir))  gSystem->mkdir(outdir);

    // Read yields
    FILE *fsig = fopen( indir+"Fit_signal/param.log", "r" );
    FILE *fresbkg = fopen( indir+"Fit_resbkg/param.log", "r" );
    FILE *fnrbkg = fopen( indir+"Fit_nrbkg/param.log", "r" );
    std::vector<float> cuts(Ncut);
    std::vector<float> ySig(Ncut);
    std::vector<float> yResbkg(Ncut);
    std::vector<float> yNrbkg(Ncut);
    for (int i=0; i<Ncut; ++i) {
        fscanf(fsig, "%f: %*f %*f %*f %*f %*f %*f %f", &cuts[i], &ySig[i]);
        fscanf(fresbkg, "%*f: %*f %*f %*f %*f %*f %*f %f", &yResbkg[i]);
        fscanf(fnrbkg, "%*f: %*f %f", &yNrbkg[i]);
    }
    fclose(fsig);
    fclose(fresbkg);
    fclose(fnrbkg);

    // Calculate Z_A
    FILE *fza = fopen( outdir+"Z_A.log", "w" );
    std::vector<float> ZA(Ncut);
    for (int i=0; i<Ncut; ++i) {
        float za = get_ZA( ySig[i], yResbkg[i]+yNrbkg[i] );
        ZA[i] = za;
        fprintf( fza, "%f:\t%f\t%f\t%f\t%f\n", cuts[i], ySig[i], yResbkg[i], yNrbkg[i], za );
    }
    fclose(fza);

    // Make Z_A graph
    TGraph *gr_za = new TGraph( Ncut, cuts.data(), ZA.data() );
    char *gr_title = Form("%s Z_{A};BDT cut;Z_{A}", (channel=="had")?"hadronic":"leptonic");
    gr_za->SetTitle(gr_title);
    TCanvas *c = new TCanvas();
    gr_za->Draw("AL");
    c->SaveAs(outdir+"Z_A.png");
} // End of function ScanZA


void Opt_ZA()
{
    //ScanZA("had");
    ScanZA("lep");
}
