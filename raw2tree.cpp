
//
void readcsv(int run, std::vector<double> &v_y);

//
void raw2tree()
{
  //
  TFile *fo = new TFile("./rootfile/xinao.root", "recreate");
  TTree *tr = new TTree("tr", "xinao data");

  int run;
  std::vector<double> v_data;

  tr->Branch("run", &run);
  tr->Branch("data", &v_data);

  for(run=9374;run<=9426;run++){
    readcsv(run, v_data);
    tr->Fill();
    cout << run << " size " << v_data.size() << endl;

    for(int i=0;i<v_data.size();i++){
      if(i%1000000==0) cout << v_data[i] << endl;
    }

    v_data.clear();
  }
  
  fo->cd();
  tr->Write();
  fo->Close();
}

//
void readcsv(int run, std::vector<double> &v_y)
{
  TString name = TString::Format("/mnt/d/xinao/data/%05d.csv", run);
  cout << name << endl;

  ifstream fi;
  fi.open(name.Data());
  if(!fi.is_open()){
    std::cerr << "Error: Cannot open file " << name << std::endl;
    return;
  }

  double time, ch2;

  std::string line;
  bool header = false;

  Long64_t i = 0;
  while(std::getline(fi,line)){
    if(i%1000000==0) cout << i << endl;
    i++;

    std::stringstream ss(line);
    std::string a, b;

    if(!header){
      cout << line << endl;
      if(line.find("Label") != std::string::npos){
        std::getline(fi, line);
        header = true;
      }
      continue;
    }

    if(std::getline(ss,a,',') && std::getline(ss,b,',')){
      try{
        time = std::stod(a);
        ch2 = std::stod(b);
        v_y.push_back(ch2);
      }catch(const std::exception &e){
        std::cerr << "error: " << e.what() << std::endl;
      }
    }
  }

  fi.close();
}