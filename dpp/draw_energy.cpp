//
void draw_energy(string name)
{
  TCanvas *c1 = new TCanvas("c1", "", 500, 400);
  c1->cd();

  TFile *fo = new TFile(TString::Format("../rootfile/%s_dpp_hist.root",name.c_str()).Data(), "recreate");

  TFile *fi = TFile::Open(TString::Format("../rootfile/%s_dpp.root",name.c_str()).Data());
  if(fi->IsZombie()){
    cout << "can not open " << name << endl;
    return;
  }

  TTree *tr = (TTree*)fi->Get("tr");

  std::vector<double> *energy = nullptr;

  tr->SetBranchAddress("energy", &energy);

  TH1D *h = new TH1D("h", "", 128,0,4096);

  for(int i=0;i<tr->GetEntries();++i){
    tr->GetEntry(i);

    for(size_t j=0;j<energy->size();++j){
      h->Fill((*energy)[j]/100.);
    }
  }

  h->SetTitle(name.c_str());
  h->GetXaxis()->SetTitle("channel");
  h->GetYaxis()->SetTitle("counts");
  h->Draw();

  fo->cd();
  h->Write();
  fo->Close();

  c1->SaveAs(TString::Format("./fig/%s_hist.png",name.c_str()));
  fi->Close();
}
