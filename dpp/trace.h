#ifndef _trace_h_
#define _trace_h_

#include "set.h"

#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TTree.h"
#include "TF1.h"
#include "TRandom3.h"
#include "TBenchmark.h"
#include "TMath.h"
#include "TGraph.h"
#include "TSystem.h"

#include <iostream>
#include <vector>
#include <map>

// Trigger and Timing Filter
struct xia_cfd_par {
  UInt_t fl; // fast length (in points)
  UInt_t fg; // fast gap length (in points)
  UInt_t d;  // delay length (in points)
  Double_t w; // scale factor (option: 0.25, 0.125, 0.0625, 0.03125)

  xia_cfd_par() : fl(0), fg(0), d(0), w(0.0) {}

  xia_cfd_par(UInt_t fl_, UInt_t fg_, UInt_t d_, Double_t w_)
    : fl(fl_), fg(fg_), d(d_), w(w_) {}
};

// RC-CR2
struct rccr2_par {
  UInt_t sf;  // smooth factor (in points)
  UInt_t rt;  // rise time

  rccr2_par() : sf(0), rt(0) {}

  rccr2_par(UInt_t sf_, UInt_t rt_)
    : sf(sf_), rt(rt_) {}
};

// Trapezoidal filter
struct trap_par {
  UInt_t d;  // decay time of input trace (in points)
  UInt_t rt; // rise time length (in points)
  UInt_t ft; // flat top length (in points)

  trap_par() : d(0), rt(0), ft(0) {}

  trap_par(UInt_t d_, UInt_t rt_, UInt_t ft_)
    : d(d_), rt(rt_), ft(ft_) {}
};

//
class trace
{
public:
  trace() = default;
  trace(Short_t d[], Long64_t n);
  virtual ~trace();

  void Analysis();

  void ComputeHits();
  void ComputeEnergy();

  std::vector<double> GetData() const { return v_data; }
  std::vector<double> GetDataMA() const { return v_data_ma; }
  std::vector<double> GetDataCFD() const { return v_data_cfd; }
  std::vector<double> GetDataRCCR2() const { return v_data_rccr2; }
  std::vector<double> GetDataTrap() const { return v_data_trap; }

  std::vector<double> GetEnergy() const { return v_energy; }

private:
  TBenchmark *benchmark;  
  TRandom3 *rndm;

public:
  //
  void MoveAverage(int n, const std::vector<double> &v, std::vector<double> &ma);
  void CFD(xia_cfd_par par, const std::vector<double> &v, std::vector<double> &cfd);
  void RCCR2(rccr2_par par, const std::vector<double> &v, std::vector<double> &rccr2);
  void Trapezoid(trap_par par, const std::vector<double> &v, std::vector<double> &trap);

  double XX(const std::vector<double> &v, UInt_t i, UInt_t sf, UInt_t rt);
  double YY(const std::vector<double> &v, UInt_t i, UInt_t sf, UInt_t rt);

private:
  double offset;
  double threshold;

  std::vector<size_t> v_hit_tag;
  std::map<size_t, std::pair<size_t, size_t>> m_hit_range;
  std::vector<double> v_energy;

  std::vector<double> v_data;
  std::vector<double> v_data_ma;
  std::vector<double> v_data_cfd;
  std::vector<double> v_data_rccr2;
  std::vector<double> v_data_trap;

private:
  static int n_ma;

  static xia_cfd_par xcp;
  static rccr2_par rp;
  static trap_par tp;

  // function for TF1
  static Double_t OneWFFun(Double_t *x, Double_t *par);
  static Double_t TwoWFFun(Double_t *x, Double_t *par);

public:
  static void InitPars();
};

#endif