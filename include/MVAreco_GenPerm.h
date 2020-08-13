#ifndef MVARECO_GENPERM_H
#define MVARECO_GENPERM_H

#include "TRandom3.h"

#include <cstdio>
#include <vector>
using namespace std;

TRandom3 rnd;

class IdxPerm_TThad
{
public:
	// Constructor
	IdxPerm_TThad(int bJet_, int M1Jet_, int WJet1_, int WJet2_) {
		if (bJet_ == M1Jet_ || bJet_ == WJet1_ || bJet_ == WJet2_ ||
				M1Jet_ == WJet1_ || M1Jet_ == WJet2_ || WJet1_ == WJet2_) {
			bJet = 0;  M1Jet = 1;  WJet1 = 2;  WJet2 = 3;
			return;
		}
		bJet = bJet_;
		M1Jet = M1Jet_;
		WJet1 = WJet1_;
		WJet2 = WJet2_;
		if (WJet1 > WJet2) {
			int tmp = WJet1;
			WJet1 = WJet2;
			WJet2 = tmp;
		}
	}
	// Test of equality btw perms
	bool operator==(const IdxPerm_TThad &perm) const {
		return bJet==perm.bJet && M1Jet==perm.M1Jet && WJet1==perm.WJet1 && WJet2==perm.WJet2;
	}
	// Check if this perm is already included by a vector of perms
	bool IncludedBy(const vector<IdxPerm_TThad> &perms) const {
		for (int i=0; i<perms.size(); ++i)
			if (*this == perms.at(i))  return true;
		return false;
	}
	// Print perm info
	void Print() const {
		printf("TThad_perm:\t%4d\t%4d\t%4d\t%4d\n", bJet, M1Jet, WJet1, WJet2);
	}

	// Static functions
	// Randomly generate a new perm given number of jets in an event
	static IdxPerm_TThad GetRandom(int njet) {
		bool valid;
		int bJet_, M1Jet_, WJet1_, WJet2_;

		do {
			valid = true;
			bJet_ = (int)rnd.Uniform(njet);
			M1Jet_ = (int)rnd.Uniform(njet);
			WJet1_ = (int)rnd.Uniform(njet);
			WJet2_ = (int)rnd.Uniform(njet);
			if ( bJet_>=njet || M1Jet_>=njet || WJet1_>=njet || WJet2_>=njet )  valid = false;
			if ( bJet_ == M1Jet_ || bJet_ == WJet1_ || bJet_ == WJet2_ ||
					M1Jet_ == WJet1_ || M1Jet_ == WJet2_ || WJet1_ >= WJet2_ )  valid = false;
		} while (!valid);

		return IdxPerm_TThad(bJet_, M1Jet_, WJet1_, WJet2_);
	}

	// Data members
	// indices for each reco jet
	int bJet;
	int M1Jet;
	int WJet1;
	int WJet2;
};

class IdxPerm_SThad
{
public:
	// Constructor
	IdxPerm_SThad(int bJet_, int WJet1_, int WJet2_) {
		if (bJet_ == WJet1_ || bJet_ == WJet2_ || WJet1_ == WJet2_) {
			bJet = 0;  WJet1 = 1;  WJet2 = 2;
			return;
		}
		bJet = bJet_;
		WJet1 = WJet1_;
		WJet2 = WJet2_;
		if (WJet1 > WJet2) {
			int tmp = WJet1;
			WJet1 = WJet2;
			WJet2 = tmp;
		}
	}
	// Test of equality btw perms
	bool operator==(const IdxPerm_SThad &perm) const {
		return bJet==perm.bJet && WJet1==perm.WJet1 && WJet2==perm.WJet2;
	}
	// Check if this perm is already included by a vector of perms
	bool IncludedBy(const vector<IdxPerm_SThad> &perms) const {
		for (int i=0; i<perms.size(); ++i)
			if (*this == perms.at(i))  return true;
		return false;
	}
	// Print perm info
	void Print() const {
		printf("SThad_perm:\t%4d\t%4d\t%4d\n", bJet, WJet1, WJet2);
	}

	// Static functions
	// Randomly generate a new perm given number of jets in an event
	static IdxPerm_SThad GetRandom(int njet) {
		bool valid;
		int bJet_, WJet1_, WJet2_;

		do {
			valid = true;
			bJet_ = (int)rnd.Uniform(njet);
			WJet1_ = (int)rnd.Uniform(njet);
			WJet2_ = (int)rnd.Uniform(njet);
			if ( bJet_>=njet || WJet1_>=njet || WJet2_>=njet )  valid = false;
			if ( bJet_ == WJet1_ || bJet_ == WJet2_ || WJet1_ >= WJet2_ )  valid = false;
		} while (!valid);

		return IdxPerm_SThad(bJet_, WJet1_, WJet2_);
	}

	// Data members
	// indices for each reco jet
	int bJet;
	int WJet1;
	int WJet2;
};

#endif
