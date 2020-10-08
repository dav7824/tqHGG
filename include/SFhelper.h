#ifndef SFHELPER_H
#define SFHELPER_H

#include "TString.h"
#include "TTree.h"

#include <iostream>
#include <bitset>
using namespace std;

class SFhelper
{
public:
    // Number of different kinds of scale factors
    static const int Nsf = 6;

    // Constructor
    // Create event weight expression by default
    SFhelper(bitset<Nsf> &inTable) :
        Weight(1), SF_pileup(1), SF_electronID(1), SF_electronReco(1), SF_muonID(1), SF_muonISO(1), SF_photon(1), //SF_btag(1),
        T(0), weightExpression("EvtInfo.genweight"), SFapplyTable(inTable)
    {
        if (SFapplyTable.test(0)) {
            cout << "<SFHelper> Apply pileup SF\n";
            weightExpression += "*SF_pileup.SF_pileup";
        }
        if (SFapplyTable.test(1)) {
            cout << "<SFHelper> Apply electron ID SF\n";
            weightExpression += "*SF_Elec.ID";
        }
        if (SFapplyTable.test(2)) {
            cout << "<SFHelper> Apply electron reco SF\n";
            weightExpression += "*SF_Elec.Reco";
        }
        if (SFapplyTable.test(3)) {
            cout << "<SFHelper> Apply muon ID SF\n";
            weightExpression += "*SF_Muon.ID";
        }
        if (SFapplyTable.test(4)) {
            cout << "<SFHelper> Apply muon ISO SF\n";
            weightExpression += "*SF_Muon.ISO";
        }
        if (SFapplyTable.test(5)) {
            cout << "<SFHelper> Apply photon SF\n";
            weightExpression += "*DiPhoInfo.centralWeight";
        }
        /*if (SFapplyTable.test(6)) {
            cout << "<SFHelper> Apply b-tagging SF\n";
        }*/
    }

    // Get event weight expression which can be used in TTree::Draw()
    TString GetWeightExpression() { return weightExpression; }

    // Set tree branches.  Used when events have to be looped
    void SetTreeBranches(TTree *Tptr)
    {
        T = Tptr;

        T->SetBranchStatus("EvtInfo.genweight", 1);
        T->SetBranchAddress("EvtInfo.genweight", &Weight);

        if (SFapplyTable.test(0)) {
            T->SetBranchStatus("SF_pileup.SF_pileup", 1);
            T->SetBranchAddress("SF_pileup.SF_pileup", &SF_pileup);
        }
        if (SFapplyTable.test(1)) {
            T->SetBranchStatus("SF_Elec.ID", 1);
            T->SetBranchAddress("SF_Elec.ID", &SF_electronID);
        }
        if (SFapplyTable.test(2)) {
            T->SetBranchStatus("SF_Elec.Reco", 1);
            T->SetBranchAddress("SF_Elec.Reco", &SF_electronReco);
        }
        if (SFapplyTable.test(3)) {
            T->SetBranchStatus("SF_Muon.ID", 1);
            T->SetBranchAddress("SF_Muon.ID", &SF_muonID);
        }
        if (SFapplyTable.test(4)) {
            T->SetBranchStatus("SF_Muon.ISO", 1);
            T->SetBranchAddress("SF_Muon.ISO", &SF_muonISO);
        }
        if (SFapplyTable.test(5)) {
            T->SetBranchStatus("DiPhoInfo.centralWeight", 1);
            T->SetBranchAddress("DiPhoInfo.centralWeight", &SF_photon);
        }
        /*if (SFapplyTable.test(6)) {
            T->SetBranchStatus("", 1);
            T->SetBranchAddress("", &);
        }*/
        cout << "<SFHelper> TTree branches set successfully!\n";
    }

    // Get calculated event weight after calling TTree::GetEntry().  Used when events
    // have to be looped.  Used only after calling SetTreeBranches(TTree*)
    float GetWeight()
    {
        return Weight * SF_pileup * SF_electronID * SF_electronReco * SF_muonID * SF_muonISO * SF_photon;
    }


    // Event weight
    float Weight;
    // Pileup SF
    float SF_pileup;
    // Electron ID SF
    float SF_electronID;
    // Electron reco SF
    float SF_electronReco;
    // Muon ID SF
    float SF_muonID;
    // Muon ISO SF
    float SF_muonISO;
    // Photon SF
    float SF_photon;
    // b-tagging SF
    //float SF_btag;

private:
    // Bitset describing applied SF
    bitset<Nsf> SFapplyTable;
    // Event weight expression
    TString weightExpression;
    // Pointer to the tree with SF
    TTree *T;
};

#endif
