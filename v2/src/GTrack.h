//
//  GTrack.h
//  
//
//  Created by Gonzalo on 09/12/14.
//
//

#ifndef ____GTrack__
#define ____GTrack__

#include <map>
#include <vector>
#include <string>
#include "TObject.h"
#include "TObjArray.h"

namespace GInfo {

class GSensor : public TObject
{
 public:
  GSensor();
  GSensor( int sensor_id, double sensor_q );
  ~GSensor();
    
  int ID;
  double Q;
    
  double X();
  double Y();
  ClassDef(GSensor,1);
};

class GSlice : public TObject
{
 public:
  GSlice();
  GSlice( std::map<int,double> cathode, std::map<int,double> anode, double z );
  ~GSlice();
    
  double E;
  double Q;
  double X;
  double Y;
  double Z;
  int NSiPMs;
    
  //  std::vector< GSensor > PMTs;
  //  std::vector< GSensor > SiPMs;
  TObjArray* PMTs;
  TObjArray* SiPMs;
  ClassDef(GSlice,1);
};

class GPeak : public TObject
{
 public:
  GPeak();
  ~GPeak();
    
  char type[3];
  int nslices;
  //  std::vector< GSlice > Slices;
  TObjArray* Slices;
  ClassDef(GPeak,1);    
};

class GTrack : public TObject
{
 public:
  GTrack();
  ~GTrack();
    
  double E;
  double Q;

  GPeak* S1;
  GPeak* S2;

  ClassDef(GTrack,1);
};

} // namespace GInfo
#endif /* defined(____GTrack__) */
