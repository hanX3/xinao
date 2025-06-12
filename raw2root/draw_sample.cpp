#include <filesystem>

//
void draw_sample_single(string name, int entry=0, int n1=0, int n2=1000);

//
void draw_sample(string name)
{
  if(!std::filesystem::exists(TString::Format("fig/%s",name.c_str()).Data())){
    std::filesystem::create_directory(TString::Format("fig/%s",name.c_str()).Data());
    std::cout << "mkdir" << name << std::endl;
  }else{
    std::cout << "already have" << name << std::endl;
  }

  //
  TFile *fi = TFile::Open(TString::Format("../rootfile/%s.root",name.c_str()).Data());
  if(fi->IsZombie()){
    cout << "can not open " << name << endl;
    return;
  }
    
  TTree *tr = (TTree*)fi->Get("tr");
  tr->Print();

  TString tr_title = tr->GetTitle();
  size_t n = 0;
  sscanf(tr_title.Data(), "%zd", &n);

  int n_entry = tr->GetEntries();
  fi->Close();

  for(int i=0;i<n_entry;i++){
    draw_sample_single(name, i, 0, n);
  }
}

//
void draw_sample_single(string name, int entry=0, int n1=0, int n2=1000)
{
  TCanvas *c1 = new TCanvas("c1", "", 900, 400);
  c1->cd();

  TFile *fi = TFile::Open(TString::Format("../rootfile/%s.root",name.c_str()).Data());
  if(fi->IsZombie()){
    cout << "can not open " << name << endl;
    return;
  }
    
  TTree *tr = (TTree*)fi->Get("tr");
  tr->Print();

  TString tr_title = tr->GetTitle();
  // for sic
  /*
  double mb = 0, ms = 0;
  sscanf(tr_title.Data(), "%lfMB_%lfms", &mb, &ms);
  printf("MB = %.0f, MS = %.0f\n", mb, ms);
  size_t n = mb*ms*1000;
  std::cout << "n = " << n << std::endl;
  */

  // for auvx
  size_t n = 0;
  sscanf(tr_title.Data(), "%zd", &n);
  std::cout << "n = " << n << std::endl;

  Short_t *w = new Short_t[n];
  tr->SetBranchAddress("w", w);
  tr->GetEntry(entry);

  TGraph *gr = new TGraph((n2-n1));
  for(int i=0;i<(n2-n1);++i){
    gr->SetPoint(i, n1+i, w[n1+i]);
  }

  gr->SetTitle(TString::Format("%s-entry%02d",name.c_str(),entry));
  gr->GetXaxis()->SetTitle("sample");
  gr->GetYaxis()->SetTitle("amplitude");
  gr->Draw("AL");

  // c1->SaveAs(TString::Format("./fig/%s/%02d_wave.png",name.c_str(),entry));
  // fi->Close();
}
