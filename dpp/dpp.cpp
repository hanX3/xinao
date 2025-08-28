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
   return;
  }

  TString tr_title = fi->Get("tr")->GetTitle();
  sscanf(tr_title.Data(), "%lld", &n);
  std::cout << "n = " << n << std::endl;

  rw = new raw((TTree*)fi->Get("tr"));

  //
  fo = new TFile(fo_name.c_str(), "recreate");
  tr = new TTree("tr", "wave info");
  tr->Branch("data", &v_data);
  tr->Branch("data_ma", &v_data_ma);
  tr->Branch("data_ff", &v_data_ff);
  tr->Branch("baseline", &baseline);
  tr->Branch("threshold", &threshold);
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
  v_data_ma = tc->GetDataMA();
  v_data_ff = tc->GetDataFF();
  baseline = tc->GetBaseline();
  threshold = tc->GetThreshold();
  v_energy = tc->GetEnergy();

  std::cout << "v_data " << v_data.size() << std::endl;
  std::cout << "v_data_ma " << v_data_ma.size() << std::endl;
  std::cout << "v_data_ff " << v_data_ff.size() << std::endl;
  std::cout << "baseline " << baseline << std::endl;
  std::cout << "threshold " << threshold << std::endl;
  std::cout << "v_energy " << v_energy.size() << std::endl;

  // for(int i=10;i<20;++i){
  //   std::cout << v_data[i] << " " << v_data_ma[i] << " " << v_data_ff[i] << std::endl;
  // }

  tr->Fill();
}
