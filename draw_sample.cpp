//
void draw_esample(int entry = 0, int n_p = 1000)
{
  TTree *tr = (TTree*)gFile->Get("tr");

  Short_t *w = new Short_t[60000000];
  tr->SetBranchAddress("w", w);
  tr->GetEntry(entry);

  TGraph *gr = new TGraph(n_p);
  for(int i=0;i<n_p;++i){
    gr->SetPoint(i, i, w[i]);
  }

  gr->Draw("AL");
}
