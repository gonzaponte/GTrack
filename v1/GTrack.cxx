//
//  GTrack.cxx
//  
//
//  Created by Gonzalo on 09/12/14.
//
//

#include "GTrack.h"
#include <string>

using namespace GInfo;

ClassImp(GSensor);
ClassImp(GSlice);
ClassImp(GPeak);
ClassImp(GTrack);

GSensor::GSensor()
{
  ID = 0;
  Q = -1.;
}

GSensor::GSensor( int sensor_id, double sensor_q )
{
  ID = sensor_id;
  Q  = sensor_q;
}

GSensor::~GSensor() {}

double GSensor::X()
{
  return 0.;
}

double GSensor::Y()
{
  return 0.;
}

GSlice::GSlice()
{
  E = -1.;
  Q = -1.;
  X = 0.;
  Y = 0.;
  Z = 0.;
  NSiPMs = -1;
  PMTs = 0;
  SiPMs = 0;
}

GSlice::GSlice( std::map<int,double> cathode, std::map<int,double> anode, double z )
{
  std::map<int,double>::const_iterator it;
    
  E = 0.;
  Q = 0.;
  X = 0.;
  Y = 0.;
  Z = z ;
  NSiPMs = anode.size();

  PMTs = new TObjArray();
  SiPMs = new TObjArray();
  //PMTs = new TObjArray(sizeof(GSensor),19);
  //SiPMs = new TObjArray(sizeof(GSensor),256);
    
  for ( it = cathode.begin(); it != cathode.end(); it++ )
    {
      GSensor PMT = GSensor( it->first, it->second );
      E += PMT.Q;
      //      PMTs.push_back( PMT );
      PMTs->AddLast( &PMT );
    }
  if (! anode.size() )
    return;
  for ( it = anode.begin(); it != anode.end(); it++ )
    {
      GSensor SiPM = GSensor( it->first, it->second );
      X += SiPM.X() * SiPM.Q;
      Y += SiPM.Y() * SiPM.Q;
      Q += SiPM.Q;
      //      SiPMs.push_back( SiPM );
      SiPMs->AddLast( &SiPM );
    }
    
  X /= Q;
  Y /= Q;
}

GSlice::~GSlice()
{
  delete PMTs;
  delete SiPMs;
}

GPeak::GPeak()
{
  strcpy(type,"00");
  nslices = -1;
  Slices = 0;
}

GPeak::~GPeak()
{
  delete Slices;
}

GTrack::GTrack()
{
  E = -1.;
  Q = -1.;
  
  S1 = GPeak();
  S2 = GPeak();
}

GTrack::~GTrack()
{
}
