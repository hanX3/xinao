#include "trace.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <algorithm>

//
trace::trace(Short_t d[], Long64_t n)
{
  double sum = 0.;
  for(Long64_t i=0;i<n;i++){
    sum += (Double_t)d[i];
  }
  offset = sum/(Double_t)n;
  std::cout << "offset " << offset << std::endl;

  if(Polarity==1){
    for(Long64_t i=0;i<n;i++){
      v_data.push_back(d[i]-offset);
    }
  }else{
    for(Long64_t i=0;i<n;i++){
      v_data.push_back(offset-d[i]);
    }
  }

  threshold = std::abs(*(std::min_element(v_data.begin(),v_data.end())));
  int k = std::pow(10, std::ceil(std::log10(threshold))-1);
  threshold = k*(int)(threshold/k+1);
  std::cout << "threshold " << threshold << std::endl;
}

//
trace::~trace()
{

}

//
void trace::Analysis()
{
  MoveAverage(n_ma, v_data, v_data_ma);
  ComputeHits();
  ComputeEnergy();
}

//
void trace::ComputeHits()
{
  v_hit_tag.clear();
  m_hit_range.clear();

  std::vector<size_t> v_indices;

  for(size_t i=0;i<v_data.size();++i){
    if(v_data[i] > threshold){
      v_indices.push_back(i);
    }
  }

  v_hit_tag.push_back(v_indices[0]);
  for(size_t i=1;i<v_indices.size();++i){
    if(v_indices[i]-v_indices[i-1]==1) continue;
    else v_hit_tag.push_back(v_indices[i]);
  }

  std::cout << "total size " << v_hit_tag.size() << std::endl;
  // std::cout << "indices of elements > " << threshold << ": ";
  // for(size_t idx : v_hit_tag){
  //   std::cout << idx << " ";
  // }
  // std::cout << std::endl;

  //
  for(size_t idx : v_hit_tag){
    size_t x1=idx, x2=idx;
    while(1){
      if(v_data[x1]<0){
        break;
      }else x1--;
    }
    while(1){
      if(v_data[x2]<0){
        break;
      }else x2++;
    }
    m_hit_range[idx] = std::make_pair(x1, x2);
  }
  // for(auto &[id, range] : m_hit_range){
  //   std::cout << "ID: " << id << ", Range: [" << range.first << ", " << range.second << ")\n";
  // }

}

//
void trace::ComputeEnergy()
{
  double e = 0.;
  for(size_t idx : v_hit_tag){
    e = 0.;
    for(size_t i=m_hit_range[idx].first;i<=m_hit_range[idx].second;++i){
      e += v_data[i];
    }
    v_energy.push_back(e);
  }

  for(size_t e : v_energy){
    std::cout << e << " ";
  }
  std::cout << std::endl;
}

//
void trace::MoveAverage(int n, const std::vector<double> &v, std::vector<double> &ma)
{
  ma.clear();
  int size = v.size();
  if (size==0 || n<=0) return;

  double sum = 0.;
  for(int i=0;i<size;++i){
    sum += v[i];
    if (i>=n) sum -= v[i-n];
    int count = std::min(i+1, n);
    ma.push_back(sum/count);
  }
}

// Ref.: Digital Gamma Finder (DGF) PIXIE-16 user's manual, edition 1.40, XIA LLC, CA USA, 2009
void trace::CFD(xia_cfd_par par, const std::vector<double> &v, std::vector<double> &cfd)
{
  int fl = par.fl;
  int fg = par.fg;
  int d = par.d;
  double w = par.w;

  int n = v.size();
  cfd.resize(n, 0.);

  std::vector<double> ff(n, 0.);

  for(int i=0;i<n;++i){
    double ff1 = 0., ff2 = 0.;
    if(i>=(fl-1) && i>=(2*fl+fg-1)){
      for(int j=i-(fl-1);j<=i;j++)  ff1 += v[j];
      for(int j=i-(2*fl+fg-1);j<=i-(fl+fg);j++)  ff2 += v[j];
    }
    ff[i] = ff1-ff2;
  }

  for(int i=0;i<n-d;i++){
    cfd[i+d] = (ff[i+d]-ff[i]/pow(2., w+1.));
  }
}

// Ref.:Z. Guzik, T. Krakowski, Nukleonika, 58, 333 (2013)
void trace::RCCR2(rccr2_par par, const std::vector<double> &v, std::vector<double> &rccr2)
{
  int sf = par.sf;
  int rt = par.rt;

  int n = v.size();
  rccr2.resize(n, 0.);

  for(int i=2*(rt+sf);i<n;i++){
    rccr2[i] = XX(v, i, sf, rt) - XX(v, i-rt, sf, rt);
  }
}

// Ref.: Jordanov V T, et al, Nucl. Instr. A, 345 (1994) 337.
void trace::Trapezoid(trap_par par, const std::vector<double> &v, std::vector<double> &trap)
{
  int n = v.size();

  std::vector<double> p_prime(n, 0.);
  std::vector<double> d(n, 0.);
  std::vector<double> s(n, 0.);

  trap.resize(n, 0.);

  int k = par.rt;
  int l = par.rt+par.ft;
  int delay = par.d;

  for(int i=0;i<n;++i){
    if(i==0){
      d[i] = v[i];
      p_prime[i] = d[i];
      s[i] = p_prime[i]+delay*d[i];
    }else if(i>0 && i<k){
      d[i] = v[i];
      p_prime[i] = p_prime[i-1]+d[i];
      s[i] = s[i-1]+p_prime[i]+delay*d[i];
    }else if(i>=k && i<l){
      d[i] = v[i]-v[i-k];
      p_prime[i] = p_prime[i-1]+d[i];
      s[i] = s[i-1]+p_prime[i]+delay*d[i];
    }else if(i>=l && i<(k+l)){
      d[i] = v[i]-v[i-k]-v[i-l];
      p_prime[i] = p_prime[i-1]+d[i];
      s[i] = s[i-1]+p_prime[i]+delay*d[i];
    }else if(i>=(k+l)){
      d[i] = v[i]-v[i-k]-v[i-l]+v[i-k-l];
      p_prime[i] = p_prime[i-1]+d[i];
      s[i] = s[i-1]+p_prime[i]+delay*d[i];
    }
                
    trap[i] = s[i]/(Double_t)delay/(Double_t)k;
  }
}

//
Double_t trace::XX(const std::vector<double> &v, UInt_t i, UInt_t sf, UInt_t rt)
{
  if(i<rt || i>=v.size()){
    return 0.;
  }

  double x = YY(v, i, sf, rt) - YY(v, i-rt, sf, rt);
  return x;
}

//
Double_t trace::YY(const std::vector<double> &v, UInt_t i, UInt_t sf, UInt_t rt)
{
  if (i<sf) return 0.;

  double y = 0.;
  for(int m=sf;m>0;--m){
    y += v[i-m];
  }

  return (sf>=2) ? (y/(sf/2)) : y;
}

//
Int_t trace::n_ma;

xia_cfd_par trace::xcp;
rccr2_par trace::rp;
trap_par trace::tp;

//
void trace::InitPars() 
{
  n_ma = MoveAverageLength;

  xcp.fl = 1;
  xcp.fg = 1;
  xcp.d = 3;
  xcp.w = 0.25;

  rp.sf = 6;
  rp.rt = 4;

  tp.d = 12500;
  tp.rt = 20;
  tp.ft = 30;
}
