//
//  GTrack.cxx
//  
//
//  Created by Gonzalo on 09/12/14.
//
//

#include "GTrack.h"

using namespace GInfo;

ClassImp(GSensor);
ClassImp(GSlice);
ClassImp(GPeak);
ClassImp(GTrack);

GSensor::GSensor( int sensor_id, double sensor_q )
{
  ID = sensor_id;
  Q  = sensor_q;
}

double GSensor::X()
{
  return 0.;
}

double GSensor::Y()
{
  return 0.;
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
    
  for ( it = cathode.begin(); it != cathode.end(); it++ )
    {
      GSensor PMT = GSensor( it->first, it->second );
      E += PMT.Q;
      PMTs.push_back( PMT );
    }
  if (! anode.size() )
    return;
  for ( it = anode.begin(); it != anode.end(); it++ )
    {
      GSensor SiPM = GSensor( it->first, it->second );
      X += SiPM.X() * SiPM.Q;
      Y += SiPM.Y() * SiPM.Q;
      Q += SiPM.Q;
      SiPMs.push_back( SiPM );
    }
    
  X /= Q;
  Y /= Q;
}
