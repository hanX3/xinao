//
void draw_energy()
{
  TCanvas *c1 = new TCanvas("c1", "", 500, 400);
  c1->cd();

  TTree *tr = (TTree*)gFile->Get("tr");

  std::vector<double> *energy = nullptr;

  tr->SetBranchAddress("energy", &energy);

  TH1D *h = new TH1D("h", "", 128,0,4096);

  for(int i=0;i<tr->GetEntries();++i){
    tr->GetEntry(i);

    for(size_t j=0;j<energy->size();++j){
      h->Fill((*energy)[j]/100.);
    }
  }

  h->Draw();
}
