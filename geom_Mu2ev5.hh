#ifndef geom_Mu2ev5_hh
#define geom_Mu2ev5_hh

class TGeoNode;
class TGeoVolume;

#include <iostream>
#include <string>
#include <vector>
namespace REX = ROOT::Experimental;

class geom_Mu2ev5{

public:

    geom_Mu2ev5(TGeoManager *geom, REX::REveManager *eveMng);

    void makeEveGeoShape(TGeoNode* n, REX::REveTrans& trans, REX::REveElement* holder);


    void setRecursiveColorTransp(TGeoVolume *vol, Int_t color, Int_t transp);

    //Function to hide element by its name
    void hideNodesByName(TGeoNode* n, const std::string& str, bool onOff, int _diagLevel, REX::REveTrans& trans,  REX::REveElement* holder, int maxlevel, int level, bool shift);

    //Function to hide all elements which are not inside DS:
    void SolenoidsOnly(TGeoNode* node, REX::REveTrans& trans,  REX::REveElement* holder, int maxlevel, int level);

    void addHits(REX::REveManager *eveMng);

    REX::REvePointSet *createPointSet(int npoints = 2, float s = 2, int color = 28);
  
};

#endif /* geom_Mu2ev5_hh */
