#include "dpp.h"
#include "set.h"

#include <iostream>
#include <fstream>
#include <assert.h>
#include <stdlib.h>

#include "TString.h"

int main(int argc, char const *argv[])
{
  if(argc != 2){
    std::cout << "need parameter" << std::endl;
    std::cout << "like: ./dpp 2025520130306" << std::endl;
    return -1;
  }

  TString fi = TString::Format("../rootfile/%s.root", argv[1]);
  std::cout << "dpp " << fi << std::endl;
  
  TString fo = TString::Format("../rootfile/%s_dpp.root", argv[1]);

  dpp *dp = new dpp(fi.Data(), fo.Data());
  dp->Process();

  delete dp;

  return 0;
}

