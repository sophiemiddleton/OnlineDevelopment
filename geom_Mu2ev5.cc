#include <sstream>
 #include <iostream>
  
 #include <TGeoManager.h>
 #include <TGeoVolume.h>
 #include <TGeoMaterial.h>
 #include <TGeoMatrix.h>

 #include <TRandom.h>
 #include <TSystem.h>
 #include <TFile.h>
 #include <ROOT/REveGeoShape.hxx>
 #include <ROOT/REveTrans.hxx>
 #include <ROOT/REveScene.hxx>
 #include <ROOT/REveViewer.hxx>
 #include <ROOT/REveElement.hxx>
 #include <ROOT/REveManager.hxx>
 #include <ROOT/REvePointSet.hxx>

#include <TGeoBoolNode.h>
#include <TGeoNode.h>
#include <TGeoPhysicalNode.h>

#include "geom_Mu2ev5.hh"

#include "TGeoNode.h"
#include "TGeoVolume.h"


using namespace std;
 namespace REX = ROOT::Experimental;

 R__LOAD_LIBRARY(libGeom);
 R__LOAD_LIBRARY(libROOTEve);
  
namespace REX = ROOT::Experimental;

geom_Mu2ev5::geom_Mu2ev5(TGeoManager *geom, REX::REveManager *eveMng){

    TGeoVolume* topvol = geom->GetTopVolume();
    gGeoManager->SetTopVolume(topvol);
    gGeoManager->SetTopVisible(kFALSE);
    TGeoNode* topnode = gGeoManager->GetTopNode(); 
    std::string name(topnode->GetName());

    //Iniitate the EVE& Visualization
    auto geoManager = eveMng->GetGeometry("fix.gdml");
    
    // display the tracker in browser
    {
        auto holder = new REX::REveElement("Inside DS");
        eveMng->GetGlobalScene()->AddElement(holder);
        REX::REveTrans trans;
        SolenoidsOnly(topnode, trans, holder,6,0);
        
    }


    REX::REveElement *event = eveMng->GetEventScene();

    REX::REvePointSet *ps = new REX::REvePointSet("Test Cosmic", "Test Cosmic Track", 200);
    for (Int_t i=0; i < 200; ++i){
        ps->SetNextPoint(0,0-100+i,0);
    }
    ps->SetMarkerColor(kRed);
    ps->SetMarkerStyle(4);
    ps->SetMarkerSize(4);
    event->AddElement(ps);
    
 }
 

  
 void geom_Mu2ev5::makeEveGeoShape(TGeoNode* n, REX::REveTrans& trans, REX::REveElement* holder)
 {
    cout<<"Adding "<<n->GetName()<<endl;
    auto gss = n->GetVolume()->GetShape();
    //setRecursiveColorTransp(n->GetVolume(), kWhite-10,70);
    auto b1s = new REX::REveGeoShape(n->GetName());
    b1s->InitMainTrans();
    b1s->RefMainTrans().SetFrom(trans.Array());
    b1s->SetShape(gss);
    b1s->SetMainColor(kWhite);
    b1s->SetMainTransparency(80);
   
    holder->AddElement(b1s);
 }
  


  void geom_Mu2ev5::setRecursiveColorTransp(TGeoVolume *vol, Int_t color, Int_t transp)
  {
    if(color>=0)vol->SetLineColor(color);
    if(transp>=0)vol->SetTransparency(transp);
    Int_t nd = vol->GetNdaughters();
    for (Int_t i=0; i<nd; i++) {
     setRecursiveColorTransp(vol->GetNode(i)->GetVolume(), color, transp);
    }
  }



 //Function to hide element by its name
  void geom_Mu2ev5::hideNodesByName(TGeoNode* n, const std::string& str, bool onOff, int _diagLevel, REX::REveTrans& trans,  REX::REveElement* holder, int maxlevel, int level, bool shift) {
    ++level;
    if (level > maxlevel)
       return;
    std::string name(n->GetName());
    
    int ndau = n->GetNdaughters();
    for ( int i=0; i<ndau; ++i ){
        TGeoNode * pn = n->GetDaughter(i);

        if (name.find(str)!= std::string::npos ){
         
            TGeoMaterial* material = n->GetVolume()->GetMaterial();
            REX::REveTrans ctrans;
            ctrans.SetFrom(trans.Array());

            {
                TGeoMatrix     *gm = n->GetMatrix();
                const Double_t *rm = gm->GetRotationMatrix();
                const Double_t *tv = gm->GetTranslation();
                REX::REveTrans t;
                t(1,1) = rm[0]; t(1,2) = rm[1]; t(1,3) = rm[2];
                t(2,1) = rm[3]; t(2,2) = rm[4]; t(2,3) = rm[5];
                t(3,1) = rm[6]; t(3,2) = rm[7]; t(3,3) = rm[8];
                

                if(shift){
                    t(1,4) = tv[0]; t(2,4) = tv[1]; t(3,4) = tv[2]+250;
                } else {
                    t(1,4) = tv[0]; t(2,4) = tv[1]; t(3,4) = tv[2];
                }
                ctrans *= t;
            }

            n->ls();

            makeEveGeoShape(n, ctrans, holder);
        }
        
       hideNodesByName( pn, str, onOff, _diagLevel, trans,  holder, maxlevel, level, shift);
       }
    
  }
  

 //Function to hide all elements which are not PS,TS, DS:
  void geom_Mu2ev5::SolenoidsOnly(TGeoNode* node, REX::REveTrans& trans,  REX::REveElement* holder, int maxlevel, int level) {

    static std::vector <std::string> substrings  {"caloDisk"}; 
    for(auto& i: substrings) hideNodesByName(node,i,kFALSE, 0, trans, holder, maxlevel, level, true);
    static std::vector <std::string> substringst  {  "TrackerSupportServiceSectionEnvelope", "Panel","TrackerPlaneEnvelope"};
    for(auto& i: substringst) hideNodesByName(node,i,kFALSE, 0, trans, holder, maxlevel, level, false);


  }
  
