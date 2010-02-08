#ifndef stdHisto_h
#define stdHisto_h

#include "treeReader.h"
#include "hFactory.h"

#include <iostream>
#include <vector>

#include "Math/GenVector/VectorUtil.h"



class stdHisto
{
 public:
  
  //!ctor
  stdHisto(treeReader&, const int&,
           const std::string& outFileName = "stdHisto.root");
  stdHisto(treeReader*, const int&,
           const std::string& outFileName = "stdHisto.root");
  
  //!dtor
  ~stdHisto();
  
  //! methods
  //void FillStdH1(const int&);
  void Add1(const std::string& histoName,
            const int& nStep);
  
  void Add2(const std::string& histoName,
            const int& nStep);
                 
  void Fill1(const std::string& histoName,
             const std::string& branchName,
             const int& nStep,
             std::vector<int>* selectionIt = NULL);

  void Fill2(const std::string& histoName,
             const std::string& branchName,
             const int& nStep,
             const int& it1, const int& it2);
  
 private:
 
  //void  AddStdH1();
  
  
  
 private:
  
  treeReader* m_reader; 
  int m_nStep;
  std::string m_outFileName;
  
  hFactory* m_hFactory;
  

};

#endif
