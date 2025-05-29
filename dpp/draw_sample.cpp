//
void draw_sample(int entry=0, int n1=0, int n2=1000)
{
  TCanvas *c1 = new TCanvas("c1", "", 500, 400);
  c1->cd();

  TTree *tr = (TTree*)gFile->Get("tr");

  std::vector<double> *data = nullptr;

  tr->SetBranchAddress("data", &data);

  tr->GetEntry(entry);

  TGraph *gr_data = new TGraph((n2-n1));
  for(int i=0;i<(n2-n1);++i){
    gr_data->SetPoint(i, n1+i, (*data)[n1+i]);
  }

  gr_data->SetTitle("");
  gr_data->Draw("AL");

  //
  TLine *l1 = new TLine(0, 6000, 60000000, 6000);
  l1->SetLineColor(kRed);
  l1->SetLineStyle(2);
  l1->SetLineWidth(2);
  l1->Draw();
}
