// Fisher v1
factory->BookMethod( dataloader, TMVA::Types::kFisher, "Fisher_v1", "H:V:VarTransform=G" );
/*
// BDT v1
factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDT_v1", "H:V:IgnoreNegWeightsInTraining:NTrees=800:BoostType=AdaBoost:AdaBoostBeta=0.5" );
// BDTG v1
factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDTG_v1", "H:V:IgnoreNegWeightsInTraining:NTrees=800:BoostType=Grad:Shrinkage=1" );
// ANN v1
factory->BookMethod( dataloader, TMVA::Types::kMLP, "ANN_v1", "H:V:IgnoreNegWeightsInTraining:VarTransform=N" );
*/
