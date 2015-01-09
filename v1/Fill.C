

void Fill( )
{
//    gROOT->ProcessLine(".L GTrack.h++");
    gROOT->ProcessLine(".L GTrack.cxx++");
    //gSystem->Load("./gtracklib");
    GInfo::GTrack* _gtrack = 0;
    TFile* tf = new TFile("filled.root","recreate");
    TTree* tt = new TTree("GTree","GTree");
    TTree::SetBranchStyle(1);
    GTree->Branch("GTracks","GInfo::GTrack",&_gtrack);
    for (int i=0;i<10;i++)
    {
        _gtrack = new GInfo::GTrack();
        tt->Fill();
    }
    tt->Write();
    tf->Close();
    delete _gtrack;
    delete tt;
    delete tf;
}