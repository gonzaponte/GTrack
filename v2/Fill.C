//#include "./include/gtrack/GTrack.h"
//#include "./src/GTrack.cxx"
//#include "TFile.h"
//#include "TTree.h"
//#include <iostream>

void Fill( )
{
//    gROOT->ProcessLine(".L /Users/Gonzalo/github/GTrack/include/GTrack.h++");
//    gROOT->ProcessLine(".L GTrack.cxx++");
    gSystem->Load("./lib/libgtrack");
//    std::cout << "OK";
    GInfo::GTrack* _gtrack = 0;
    TFile* tf = new TFile("filled.root","recreate");
    TTree* tt = new TTree("GTree","GTree");
    TTree::SetBranchStyle(1);
    tt->Branch("GTracks","GInfo::GTrack",&_gtrack);
    for (int i=0;i<10;i++)
    {
        _gtrack = new GInfo::GTrack();
        tt->Fill();
    }
    tt->Write();
    tf->Close();
//    delete _gtrack;
//    delete tt;
//    delete tf;
}