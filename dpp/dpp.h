#ifndef _dpp_h_
#define _dpp_h_

#include "raw.h"
#include "trace.h"

#include "TFile.h"
#include "TTree.h"
#include "TBenchmark.h"
#include "TRandom3.h"

//
class dpp
{
public:
  dpp(const std::string &fi_name, const std::string &fo_name);
  virtual ~dpp();

public:
  void Process();
  void Process_entry(Long64_t n_entry);

private:
  std::vector<double> v_data;
  std::vector<double> v_energy;

private:
  TBenchmark *benchmark;
  TRandom3 *rndm;

private:
  Long64_t n;

private:
  TFile *fi;
  raw *rw;
  trace *tc;

  TFile *fo;
  TTree *tr;
};




#endif