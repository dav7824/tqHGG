//float br_min = 1e-4;
//float br_max = 1e-2;
float br_min = 2e-2;  // Additional scan for hadronic channel
float br_max = 3e-2;  // Additional scan for hadronic channel
int Nscan = 100;
TString maindir = "/wk_cms2/mc_cheng/FCNH/CMSSW_8_1_0/src/tqHGG_syst";

void TTree2Text(TString coupling, TString channel)
{
    if (coupling != "Hut" && coupling != "Hct") {
        cout << "[ERROR] Invalid coupling\n";
        exit(1);
    }
    if (channel != "had" && channel != "lep" && channel != "comb") {
        cout << "[ERROR] Invalid channel\n";
        exit(1);
    }

    // Input dir
    TString scandir = maindir + Form("/scan_%s_%s/",channel.Data(),coupling.Data());
    // Output log
    TString out_name = maindir+Form("/Limits_%s_%s.txt",channel.Data(),coupling.Data());
    //FILE *out = fopen( out_name.Data(), "w" );
    FILE *out = fopen( out_name.Data(), "a" );

    // BR increment
    float br_int = (br_max - br_min) / Nscan;

    double limit_br = 0;
    double limit_exp[5] = {0};
    double limit_obs = 0;

    // Start scan point loop
    for (int i=0; i<Nscan; ++i) {
        /*if (i%10 == 0)
            cout << "---Processing scan no." << i << endl;*/
        // Read corresponding file
        TString fin_name = scandir + Form("higgsCombine%03d.AsymptoticLimits.mH120.root",i+200);
        TFile *fin = new TFile( fin_name );
        TTree *T = (TTree*)fin->Get("limit");

        // Read expected limits
        T->SetBranchAddress("limit", &limit_br);
        for (int j=0; j<5; ++j) { // Note: The 5th entry is the observed limit
            T->GetEntry(j);
            limit_exp[j] = limit_br;
        }
        T->GetEntry(5);
        limit_obs = limit_br;

        // Print limits
        fprintf( out, "%3d  %f      %f   %f   %f   %f   %f      %f\n",
               i+200, br_min+br_int*i, limit_exp[0], limit_exp[1], limit_exp[2], limit_exp[3], limit_exp[4], limit_obs);

        fin->Close();
    } // End scan point loop

    fclose(out);
}
