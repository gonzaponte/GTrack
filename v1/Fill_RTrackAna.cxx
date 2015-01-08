//
//  RTrackAna.cpp
//
//
//  Created by Gonzalo on 01/09/13.
//
//

#include <JobControl/jobc.h>

#include "./RTrackAna.h"
#include "Utilities/VectorOperations.h"
#include "Utilities/HistoOperations.h"
#include "Utilities/Interval.h"
#include "XYReco/XYRecoFactory.h"
#include "RecoBaseTemp/Peak.h"
#include "RecoBaseTemp/PMap.h"
#include "RecoBaseTemp/PTrack.h"
#include "RecoBaseTemp/RTrack.h"
#include "RecoBaseTemp/RSlice.h"
#include "RecoBaseTemp/RCluster.h"
#include "RecoBaseTemp/Voxel.h"
#include "RecoBaseTemp/XYZE.h"
#include "ClusterReco/ClusterRecoUtils.h"
#include "ClusterReco/ClusterRecoFactory.h"
#include "FourierReco/FourierFactory.h"
#include "Database/Table.h"
#include "TGraph.h"
#include "Database/Sensor.h"
#include "Database/SensorDatabase.h"
#include <algorithm>
#include <string>
#include <map>
#include <stdexcept>
#include "GTrack/GTrack.h"

MODULE_DECL(RTrackAna);

namespace recobase { class RTrack; }
namespace util { class Interval; }

RTrackAna::RTrackAna ( const char* version ) : jobc::Module ( "RTrackAna" )
{
    std::cout << "### Initializing "  << this->Name() <<  ", " <<
    version   << " configuration ###" << std::endl;

     //add your initialization code that does not depend on module
     //configuration parameters here
    //    std::cout << "Antes!"<<std::endl;
    this->SetCfgVersion(version);
    //    std::cout << "Desoius!"<<std::endl;
    fclusterreco = ClusterRecoFactory::CreateClusterReco(fclustertype);
    fxyreco = XYRecoFactory::CreateXYReco(fxyrecotype);

    double FourierParameters[3] = { fVoxelSize,
				    fELdistance,
				    fSmearingSigma };
    //    fFourierFactory
    fFourierReco = FourierFactory::CreateFourierReco( fFourierRecoVersion, FourierParameters );

    tf         = new TFile("/data4/NEXT/users/gmartinez/topo/Tracks8/cs1mustree.root","RECREATE");
    treetracks = new TTree("treetracks","treetracks");
    treeslices = new TTree("treeslices","treeslices");
    treesipms  = new TTree("treesipms" ,"treesipms" );
    treepmts  = new TTree("treepmts" ,"treepmts" );
    //    tracks = treetracks->Branch("tracks", &track, "E/D:Q:CorrZ:CorrXY:nslices/I:Emaxpos:Qmaxpos:isright/O");
    tracks = treetracks->Branch("tracks", &track, "E/D:Q:nslices/I:Emaxpos:Qmaxpos:isright/O");
    slices = treeslices->Branch("slices",&slice, "E/D:Q:X:Y:Z:IDmax/I:nsipms");
    SiPMs  = treesipms->Branch("SiPMs",&sipm,"Q/D:ID/I:corona/O");
    PMTs  = treepmts->Branch("PMTs",&pmt,"E/D:ID/I");

    _gtrack = 0;
    GFile = new TFile("/data4/NEXT/users/gmartinez/topo/G.root","RECREATE");
    GTree = new TTree("GTree","GTree");
    TTree::SetBranchStyle(1);
    GTree->Branch("GTracks","GInfo::GTrack",&_gtrack);

    /// Change from data elecid to MC elecid
    io::Table* datasimTable = new io::Table("xml/DataSimIDTable.xml");
  
    int dataID, MCID;
 
    for (unsigned int irow=0; irow<datasimTable->size(); irow++) {
      dbi::Row& row = (*datasimTable)[irow];
      row.Get("DataNumber", dataID);   
      row.Get("SimMCNumber", MCID);
      fdata2MC[dataID] = MCID;
    }

}

RTrackAna::~RTrackAna( )
{
    std::cout       << "### Terminating "   << this->Name() <<  ", " <<
    this->Version() << " configuration ###" <<  std::endl;
    
    tf->Write();
    GFile->Write();
    GFile->Close();

    delete tf;
    delete fxyreco;
    delete fclusterreco;
    delete fFourierReco;
}

GInfo::GTrack* RTrackAna::GInterface( const recobase::RTrack* rtrack )
{
  const recobase::PTrack& ptrack = rtrack->GetPTrack();
  GInfo::GTrack* gtrack = this->GInterface( &ptrack );
  return gtrack;
}

GInfo::GTrack* RTrackAna::GInterface( const recobase::PTrack* ptrack )
{
  const recobase::PMap* s1 = ptrack->GetS1PMap();
  const recobase::PMap* s2 = ptrack->GetS2PMap();
  double t0 = s1->GetPeak()->Tpeak();
  double v0 = ptrack->DriftVelocity();

  double totalE = 0.;
  double totalQ = 0.;
    
  GInfo::GPeak S1 = GInfo::GPeak();
  GInfo::GPeak S2 = GInfo::GPeak();

  //S1.Slices = new TObjArray(sizeof(GInfo::GSlice),1);
  //S2.Slices = new TObjArray(sizeof(GInfo::GSlice),100);
  S1.Slices = new TObjArray();
  S2.Slices = new TObjArray();

  strcpy(S1.type,"S1");
  strcpy(S2.type,"S2");
  
  const std::vector< std::map<int,double> >& anodes1   = s1->GetAnodeMap();
  const std::vector< std::map<int,double> >& cathodes1 = s1->GetCathodeMap();
  const std::vector< std::map<int,double> >& anodes2   = s2->GetAnodeMap();
  const std::vector< std::map<int,double> >& cathodes2 = s2->GetCathodeMap();

  if (! ( anodes2.size() == cathodes2.size() ) )
    throw std::runtime_error("Different number of slices between anode and cathode for S2!");

  S1.nslices = cathodes1.size();
  S2.nslices = cathodes2.size();
    
  double tmin   = s1->GetPeak()->Tmin();
  double twidth = s2->WidthOfTimeSlices();
    
  for (int i=0; i<S1.nslices; i++)
    {
      double ti = tmin + (i+1)*twidth;
      double z  = (ti-t0) * v0;
      std::map<int,double> dummy;
      GInfo::GSlice slice = GInfo::GSlice( cathodes1[i], dummy, z );
      S1.Slices->AddLast( &slice );
    }
    
  for (int i=0; i<S2.nslices; i++)
    {
      double ti = tmin + (i+1)*twidth;
      double z = (ti-t0) * v0;
      GInfo::GSlice slice = GInfo::GSlice( cathodes2[i], anodes2[i], z );
      totalE += slice.E;
      totalQ += slice.Q;
      S2.Slices->AddLast( &slice );
    }
    
  GInfo::GTrack* gtrack = new GInfo::GTrack();
    
  gtrack->E  = totalE;
  gtrack->Q  = totalQ;
  gtrack->S1 = S1;
  gtrack->S2 = S2;
    
  return gtrack;
}

void RTrackAna::GTrackFill( const recobase::RTrack* rtrack )
{
  //_gtrack = this->GInterface( rtrack );
  _gtrack = new GInfo::GTrack();
  GTree->Fill();
  return;
}
