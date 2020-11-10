 //Run these commands in the ROOT (with EVE-7) terminal
 void geom(){
    namespace REX = ROOT::Experimental;

    gROOT->ProcessLine(".L geom_Mu2ev5.cc+");
    auto eveMng = REX::REveManager::Create();
    TFile::SetCacheFileDir(".");

    //Extract geometry from the GDML and pass to shape maker:
    TGeoManager *geom = TGeoManager::Import("fix.gdml");

    geom_Mu2ev5 g(geom, eveMng);
    
    eveMng->Show();
 }
