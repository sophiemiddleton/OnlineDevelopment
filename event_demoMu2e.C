 //macro allows for event navigation of faked random cosmics
 // eventually will add in 2D projections too
 
 #include <vector>
 #include <string>
 #include <iostream>
  
 #include "TClass.h"
 #include "TRandom.h"
 #include "TGeoTube.h"
 #include "TGeoSphere.h"
 #include "TParticle.h"
 #include "TApplication.h"
 #include "TMatrixDSym.h"
 #include "TVector.h"
 #include "TMatrixDEigen.h"
  
 #include <ROOT/REveGeoShape.hxx>
 #include <ROOT/REveScene.hxx>
 #include <ROOT/REveViewer.hxx>
 #include <ROOT/REveElement.hxx>
 #include <ROOT/REveManager.hxx>
 #include <ROOT/REveUtil.hxx>
 #include <ROOT/REveGeoShape.hxx>
 #include <ROOT/REveProjectionManager.hxx>
 #include <ROOT/REveProjectionBases.hxx>
 #include <ROOT/REvePointSet.hxx>
 #include <ROOT/REveJetCone.hxx>
 #include <ROOT/REveTrans.hxx>
  
 #include <ROOT/REveTrack.hxx>
 #include <ROOT/REveTrackPropagator.hxx>
 #include <ROOT/REveEllipsoid.hxx>
 
//for this prototype:
//#include "event_Nav.hh"
//#include "EventManager.hh"


 namespace REX = ROOT::Experimental;
  
 // globals
 REX::REveManager *eveMng = nullptr;
 REX::REveProjectionManager *mngRhoPhi = nullptr;
 REX::REveProjectionManager *mngRhoZ   = nullptr;
 REX::REveScene  *rPhiGeomScene = nullptr, *rPhiEventScene = nullptr;
 REX::REveScene  *rhoZGeomScene = nullptr, *rhoZEventScene = nullptr;
 REX::REveViewer *rphiView = nullptr;
 REX::REveViewer *rhoZView = nullptr;
  
 const Double_t kR_min = 240;
 const Double_t kR_max = 250;
 const Double_t kZ_d   = 300;
 
void makeEveGeoShape(TGeoNode* n, REX::REveTrans& trans, REX::REveElement* holder)
 {
    cout<<"Adding "<<n->GetName()<<endl;
    auto gss = n->GetVolume()->GetShape();
    auto b1s = new REX::REveGeoShape(n->GetName());
    b1s->InitMainTrans();
    b1s->RefMainTrans().SetFrom(trans.Array());
    b1s->SetShape(gss);
    b1s->SetMainColor(kWhite);
    b1s->SetMainTransparency(80);
   
    holder->AddElement(b1s);
 }
  

 

void hideNodesByName(TGeoNode* n, const std::string& str, bool onOff, int _diagLevel, REX::REveTrans& trans,  REX::REveElement* holder, int maxlevel, int level, bool shift) {
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
  void SolenoidsOnly(TGeoNode* node, REX::REveTrans& trans,  REX::REveElement* holder, int maxlevel, int level) {

    static std::vector <std::string> substrings  {"caloDisk"}; 
    for(auto& i: substrings) hideNodesByName(node,i,kFALSE, 0, trans, holder, maxlevel, level, true);
    static std::vector <std::string> substringst  {  "TrackerSupportServiceSectionEnvelope", "Panel","TrackerPlaneEnvelope"};
    for(auto& i: substringst) hideNodesByName(node,i,kFALSE, 0, trans, holder, maxlevel, level, false);

}

  
 void addTrackPoints()
 {
    REX::REveElement* event = eveMng->GetEventScene();
  
    auto pntHolder = new REX::REveElement("Hits");
    TRandom &r = *gRandom;
    REX::REvePointSet *ps1 = new REX::REvePointSet("Test Cosmic", "Test Cosmic Track", 200);
    double start_x = r.Uniform(-10,10);
    double start_z = r.Uniform(-150,150);
    double grad_z = r.Uniform(-1,1);
    double grad_x = r.Uniform(-1,1);
    for (Int_t i=0; i < 200; ++i){
        ps1->SetNextPoint(start_x+grad_x*(0-100+i),0-100+i,start_z+grad_z*(0-100+i));

    }
    ps1->SetMarkerColor(kRed);
    ps1->SetMarkerStyle(4);
    ps1->SetMarkerSize(4);
    pntHolder->AddElement(ps1);
    event->AddElement(pntHolder);
 }
  
  
 void makeEventScene()
 {
    addTrackPoints();
 }
  
 void makeGeometryScene()
 {
    TGeoManager *geom = TGeoManager::Import("fix.gdml");
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
    
  
 }
  
 class EventManager : public REX::REveElement
 {
 public:
    EventManager() = default;
  
    virtual ~EventManager() {}
  
    virtual void NextEvent()
    {
       eveMng->DisableRedraw();
       auto scene =  eveMng->GetEventScene();
       scene->DestroyElements();
       makeEventScene();
       eveMng->EnableRedraw();
       eveMng->DoRedraw3D();
    }
  
    virtual void QuitRoot()
    {
       printf("Quit ROOT\n");
       if (gApplication) gApplication->Terminate();
    }
  
 };

  
 void event_demoMu2e()
 {
  
    gRandom->SetSeed(0); // make random seed
  
    eveMng = REX::REveManager::Create();
  
    auto eventMng = new EventManager();
    eventMng->SetName("EventManager");
    eveMng->GetWorld()->AddElement(eventMng);
  
    eveMng->GetWorld()->AddCommand("QuitRoot", "sap-icon://log", eventMng, "QuitRoot()");
  
    eveMng->GetWorld()->AddCommand("NextEvent", "sap-icon://step", eventMng, "NextEvent()");
  
    makeGeometryScene();
    makeEventScene();
  
    eveMng->Show();
 }

