#include "dpp.h"
#include <ctime>
#include <cstdio>

#include "TString.h"

//
dpp::dpp(const std::string &fi_name, const std::string &fo_name)
{
  benchmark = new TBenchmark;
  rndm = new TRandom3((Long64_t)time(0));

  fi = TFile::Open(fi_name.c_str());
  if(fi->IsZombie()){
   std::cout << "open file " << fi_name << " error!" << std::endl;
   delete fi;
   return ;
  }

  TString tr_title = fi->Get("tr")->GetTitle();
  double mb = 0, ms = 0;
  sscanf(tr_title.Data(), "%lfMB_%lfms", &mb, &ms);
  printf("MB = %.0f, MS = %.0f\n", mb, ms);
  n = mb*ms*1000;
  std::cout << "n = " << n << std::endl;

  rw = new raw((TTree*)fi->Get("tr"));

  //
  fo = new TFile(fo_name.c_str(), "recreate");
  tr = new TTree("tr", "wave info");
  // tr->Branch("data", &v_data);
  tr->Branch("energy", &v_energy);

  //
  tc = new trace();
  tc->InitPars();
}

//
dpp::~dpp()
{
  fi->Close();
}

//
void dpp::Process()
{
  std::cout << "all entries " << rw->GetEntries() << std::endl;

  for(Long64_t i=0;i<rw->GetEntries();++i){
    Process_entry(i);
  }

  fo->cd();
  tr->Write();
  fo->Close();
}

//
void dpp::Process_entry(Long64_t n_entry)
{
  rw->GetEntry(n_entry);

  tc = new trace(rw->w, n);
  tc->Analysis();

  v_data = tc->GetData();
  v_energy = tc->GetEnergy();

  std::cout << "v_data " << v_data.size() << std::endl;
  std::cout << "v_energy " << v_energy.size() << std::endl;

  tr->Fill();
}
