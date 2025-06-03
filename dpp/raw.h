//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed May 28 09:05:39 2025 by ROOT version 6.32.12
// from TTree tr/1200.0MB_50.0ms
// found on file: ../rootfile/2025520130306.root
//////////////////////////////////////////////////////////

#ifndef raw_h
#define raw_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>


// Header file for the classes stored in the TTree if any.

class raw {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Short_t         w[999999999];

   // List of branches
   TBranch        *b_w;   //!

   raw(TTree *tree=0);
   virtual ~raw();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual bool     Notify();
   virtual void     Show(Long64_t entry = -1);

   virtual Long64_t GetEntries();
};

#endif

#ifdef raw_cxx
raw::raw(TTree *tree) : fChain(0) 
{
   Init(tree);
}

raw::~raw()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t raw::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t raw::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void raw::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("w", w, &b_w);
   Notify();
}

bool raw::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return true;
}

void raw::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t raw::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
Long64_t raw::GetEntries()
{
   return fChain->GetEntriesFast();
}
#endif // #ifdef raw_cxx
