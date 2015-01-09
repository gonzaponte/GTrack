// ----------------------------------------------------------------------------
//  $Id: LinkDef.h 9556 2014-01-14 22:24:14Z gomez $
//
//  Author:  <paola.ferrario@ific.uv.es>
//  Created: 14 Feb 2013
//  
//  Copyright (c) 2013 NEXT Collaboration
// ---------------------------------------------------------------------------- 

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace GInfo+;
#pragma link C++ class GInfo::GSensor+;
#pragma link C++ class GInfo::GSlice+;
#pragma link C++ class GInfo::GPeak+;
#pragma link C++ class GInfo::GTrack+;

#pragma link C++ class std::map<int,double>+;
#pragma link C++ class std::vector<std::map<int,double>>+;
#pragma link C++ class std::vector< GInfo::GSensor >+;
#pragma link C++ class std::vector< GInfo::GSlice >+;

#endif
