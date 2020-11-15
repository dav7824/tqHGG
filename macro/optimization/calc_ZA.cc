const int Ncut = 100;

double get_ZA(double ysig, double ybkg) {
    return TMath::Sqrt( 2*( (ysig+ybkg)*TMath::Log(1+ysig/ybkg) - ysig ) );
}

// Read signal & res-bkg
void readLog_v1(TString fin, double *v_yield) {
    // Read input file
    FILE *input = fopen(fin.Data(), "r");
    // Skip the first row
    for (int i=0; i<13; ++i) fscanf(input, "%*s");

    // Read file content
    for (int i=0; i<Ncut; ++i) {
        fscanf(input, "%*d %*lf %*lf %*lf %*lf %*lf %*lf %*lf %*lf %*lf %*lf %*lf %lf", &v_yield[i]);
    }
    // Close file
    fclose(input);
}

// Read non-res-bkg
void readLog_v2(TString fin, double *v_cut, double *v_yield) {
    // Read input file
    FILE *input = fopen(fin.Data(), "r");
    // Skip the first row
    for (int i=0; i<9; ++i) fscanf(input, "%*s");

    // Read file content
    for (int i=0; i<Ncut; ++i) {
        fscanf(input, "%*d %lf %*lf %*lf %*lf %*lf %*lf %*lf %lf", &v_cut[i], &v_yield[i]);
    }
    // Close file
    fclose(input);
}

void calc_ZA(TString ch)
{
    if (ch!="had" && ch!="lep") {
        cout << "[ERROR] Invalid channel!\n";
        return;
    }

    // Set file paths
    TString iodir;
    if (ch == "had") {
        iodir = "/wk_cms2/mc_cheng/public/tqHGG/2017_v2/optimization_had/output_BDT_v1/models/";
    } else {
        iodir = "/wk_cms2/mc_cheng/public/tqHGG/2017_v2/optimization_lep/output_BDT_v1/models/";
    }
    TString fin_sig = iodir + "signal/param.log";
    TString fin_res = iodir + "resbkg/param.log";
    TString fin_nr = iodir + "nrbkg/param.log";

    // Input arrays
    double cuts[Ncut] = {0};
    double yield_sig[Ncut] = {0};
    double yield_res[Ncut] = {0};
    double yield_nr[Ncut] = {0};
    double Z_A[Ncut] = {0};

    // Read input logs
    readLog_v1(fin_sig, yield_sig);  // signal
    readLog_v1(fin_res, yield_res);  // res-bkg
    readLog_v2(fin_nr, cuts, yield_nr);  // non-res-bkg

    // Calculate Z_A
    for (int i=0; i<Ncut; ++i) {
        Z_A[i] = get_ZA(yield_sig[i], yield_res[i]+yield_nr[i]);
    }

    // Print result to log
    FILE *out = fopen((iodir+"Z_A.log").Data(), "w");
    fprintf(out, "%10s    %10s    %10s      %10s\n", "cut", "signal", "bkg", "Z_A");
    for (int i=0; i<Ncut; ++i) {
        fprintf(out, "%10f    %10f    %10f      %10f\n", cuts[i], yield_sig[i], yield_res[i]+yield_nr[i], Z_A[i]);
    }
    fclose(out);

    // Make TGraph
    TGraph *gr_ZA = new TGraph(Ncut, cuts, Z_A);
    char *gr_title = Form("%s Z_{A};BDT cut;Z_{A}", (ch=="had")?"hadronic":"leptonic");
    gr_ZA->SetTitle(gr_title);
    TCanvas *c = new TCanvas();
    gr_ZA->Draw("AL");
    c->SaveAs(iodir+"Z_A.png");
}
