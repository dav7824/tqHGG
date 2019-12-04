#include "ScaleFactor.h"
#include "TreeReader.h"

#include "TFile.h"
#include "TH2.h"

#include <cmath>
#include <vector>
using namespace std;

void PrepareSF(vector<TFile*> &fin, vector<TH2*> &hist)
{
    fin.push_back( new TFile("/wk_cms2/mc_cheng/public/tqHGG/2017/SF/egammaEffi.txt_EGM2D_runBCDEF_passingRECO.root") ); // electron RECO SF
    fin.push_back( new TFile("/wk_cms2/mc_cheng/public/tqHGG/2017/SF/2017_ElectronMedium.root") ); // electron ID SF
    fin.push_back( new TFile("/wk_cms2/mc_cheng/public/tqHGG/2017/SF/RunBCDEF_SF_ID.root") ); // muon ID SF
    fin.push_back( new TFile("/wk_cms2/mc_cheng/public/tqHGG/2017/SF/RunBCDEF_SF_ISO.root") ); // muon ISO SF

    hist.resize(fin.size());
    hist[0] = (TH2F*)fin[0]->Get("EGamma_SF2D");
    hist[1] = (TH2F*)fin[1]->Get("EGamma_SF2D");
    hist[2] = (TH2D*)fin[2]->Get("NUM_TightID_DEN_genTracks_pt_abseta");
    hist[3] = (TH2D*)fin[3]->Get("NUM_LooseRelIso_DEN_TightIDandIPCut_pt_abseta");
}

void ApplySF(TreeReader &inTree, vector<TH2*> &hist, float &weight)
{
    // photon SFs
    weight *= inTree.DiPhoInfo_centralWeight;
    // electron SFs
    for (int i=0; i<inTree.ElecInfo_Size; ++i) {
	weight *= hist[0]->GetBinContent(hist[0]->FindBin(inTree.ElecInfo_Eta->at(i), inTree.ElecInfo_Pt->at(i)));
	weight *= hist[1]->GetBinContent(hist[1]->FindBin(inTree.ElecInfo_Eta->at(i), inTree.ElecInfo_Pt->at(i)));
    }
    // muon SFs
    for (int i=0; i<inTree.MuonInfo_Size; ++i) {
	weight *= hist[2]->GetBinContent(hist[2]->FindBin(inTree.MuonInfo_Pt->at(i), fabs(inTree.MuonInfo_Eta->at(i))));
	weight *= hist[3]->GetBinContent(hist[3]->FindBin(inTree.MuonInfo_Pt->at(i), fabs(inTree.MuonInfo_Eta->at(i))));
    }
}

void CloseSF(vector<TFile*> &fin)
{
    for (int i=0; i<fin.size(); ++i) fin[i]->Close();
}
