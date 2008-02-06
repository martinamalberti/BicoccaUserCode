int AddToList (TList * BkgFileList, const char * name) ;


// ------------------------------------------------------------------------


int leggi () 
{
  TFile signal ("VBFElePlots_H160.root") ;

  //PG find the folder
  //PG ---------------
  
  TDirectory * histosFolder ;
  TIter nextkey (signal.GetListOfKeys ()) ;
  TKey * key ;
  while ((key = (TKey*) nextkey ())) 
    {
      TObject *obj = key->ReadObj () ;
      if (obj->IsA ()->InheritsFrom ("TDirectory"))        
        {
          histosFolder = (TDirectory *) obj ;
          std::cout << "bingo: " << histosFolder->GetName () << std::endl ; 
        } else delete obj ;      
    }

  //PG find the histos
  //PG ---------------
  
  std::vector<std::string> histosNameList ;
  TIter nextkey2 (histosFolder->GetListOfKeys ()) ;
  while ((key = (TKey*) nextkey2 ())) 
    {
      TObject *obj = key->ReadObj () ;
      if (obj->IsA ()->InheritsFrom ("TH1") )
        {
//          std::cout << "histo: " << obj->GetName () << std::endl ;
          histosNameList.push_back (std::string (obj->GetName ())) ;
        }
      delete obj ;      
    }

//  for (int i = 0 ; i < histosNameList.size () ; ++i)
//    std::cout << histosNameList[i] << std::endl ;

  BkgFileList = new TList () ;
  AddToList (BkgFileList, "VBFElePlots_*bkg.root") ;
  int numOfBkg = BkgFileList->GetSize () ;

//  TFile *first_source = (TFile*) BkgFileList->First(); 
//  first_source->ls () ; 
 
 
  TLegend *legend = new TLegend (0,0,1,1) ;
  legend->SetTextSize (0.04) ; 
  legend->SetFillColor (0) ;
  bool doLegend = true ;

//  std::vector<std::string> nomeTrend ;
  std::vector<TString> nomeTrend ;
 
  //PG loop over the histos to be plotted
  for (int i = 0 ; i < histosNameList.size () ; ++i)
    {
      //PG signal
      std::string histoName = std::string (histosFolder->GetName ()) + "/" + histosNameList[i] ;
//      std::cout << "PIPPA " << histoName << std::endl ;
      TH1F * h_signal = (TH1F *) signal.Get (histoName.c_str ()) ; 
      h_signal->SetFillColor (15) ;
      h_signal->Scale (1./h_signal->GetIntegral ()) ;
      //PG bkg
      TH1F ** h_bkg = new TH1F* [numOfBkg] ;
      TFile * first_bkg = (TFile *) BkgFileList->First () ;
      h_bkg[0] = (TH1F *) first_bkg->Get (histoName.c_str ()) ;
      h_bkg[0]->Scale (1./h_bkg[0]->GetIntegral ()) ;
      h_bkg[0]->SetLineColor (29) ;
      h_bkg[0]->SetLineWidth (2) ;

      if (doLegend)
        {
//          std::string dummy = first_bkg->GetName () ;
//          dummy.erase (0,dummy.rfind ("/", dummy.size ()) + 1) ;
//          nomeTrend.push_back (dummy) ;
          TString dummy = first_bkg->GetName () ;
          dummy.Remove (0,dummy.Last ('/')+1) ;
          dummy.Remove (dummy.Length ()-5,dummy.Length ()) ;
          nomeTrend.push_back (dummy) ;
        }

      TFile * next_bkg = (TFile*) BkgFileList->After (first_bkg) ;
      int it = 1 ;
      while ( next_bkg ) 
        {
          std::cout << "reading : " << it << " " << next_bkg->GetName() << std::endl ;
          h_bkg[it] = (TH1F *) next_bkg->Get (histoName.c_str ()) ;
          h_bkg[it]->Scale (1./h_bkg[it]->GetIntegral ()) ;
          h_bkg[it]->SetLineColor (29 + 5 * it) ;
          h_bkg[it]->SetLineWidth (2) ;
          if (doLegend)
            {
//              std::string dummy = next_bkg->GetName () ;
//              dummy.erase (0,dummy.rfind ("/", dummy.size ()) + 1) ;
//              nomeTrend.push_back (dummy) ;
              TString dummy = next_bkg->GetName () ;
              dummy.Remove (0,dummy.Last ('/')+1) ;
              dummy.Remove (dummy.Length ()-5,dummy.Length ()) ;
              nomeTrend.push_back (dummy) ;
            }
          next_bkg = (TFile *) BkgFileList->After (next_bkg) ;
          ++it ;
        }
     
      double max = h_signal->GetMaximum () ;
      TH1F * first = h_signal ;
      for (int itHisto = 0 ; itHisto < numOfBkg ; ++itHisto)
        {
          double localMax = h_bkg[itHisto]->GetMaximum () ;
          if (localMax > max)
            {
              max = localMax ;
              first = h_bkg[itHisto] ;
            }
        }

      TCanvas c1 ;
      first->Draw () ;
      h_signal->Draw ("same") ;
      for (int ii = 0 ; ii < numOfBkg ; ++ii) h_bkg[ii]->Draw ("same") ;

      c1.Print ((histosNameList[i] + ".gif").c_str (),"gif") ;

      if (doLegend)
        {
          legend->AddEntry (h_signal, "signal", "lf");
          for (int ii = 0 ; ii < numOfBkg ; ++ii) 
            legend->AddEntry (h_bkg[ii], nomeTrend[ii], "lf");
//            legend->AddEntry (h_bkg[ii], h_bkg[ii]->GetName (), "lf");
          TCanvas c2 ;
          legend->Draw () ;
          c2.Print ("legend.gif","gif") ;
          doLegend = false ;
        }

      delete h_signal ;
      delete [] h_bkg ;
    } //PG loop over the histos to be plotted
  
}


// ------------------------------------------------------------------------


int AddToList (TList * BkgFileList, const char * name) 
{

   // case with one single file
   if (!TString (name).MaybeWildcard ()) {
      BkgFileList->Add (TFile::Open (name) ) ;
      return 1 ;
//      return AddFile (name, nentries);
   }

   // wildcarding used in name
   Int_t nf = 0;
   TString basename(name);

   Int_t dotslashpos = basename.Index(".root/");
   TString behind_dot_root;
   if (dotslashpos>=0) {
      // Copy the tree name specification
      behind_dot_root = basename(dotslashpos+6,basename.Length()-dotslashpos+6);
      // and remove it from basename
      basename.Remove(dotslashpos+5);
   }

   Int_t slashpos = basename.Last('/');
   TString directory;
   if (slashpos>=0) {
      directory = basename(0,slashpos); // Copy the directory name
      basename.Remove(0,slashpos+1);      // and remove it from basename
   } else {
      directory = gSystem->UnixPathName(gSystem->WorkingDirectory());
   }

   const char *file;
   void *dir = gSystem->OpenDirectory(gSystem->ExpandPathName(directory.Data()));

   if (dir) {
      //create a TList to store the file names (not yet sorted)
      TList l;
      TRegexp re(basename,kTRUE);
      while ((file = gSystem->GetDirEntry(dir))) {
         if (!strcmp(file,".") || !strcmp(file,"..")) continue;
         TString s = file;
         if ( (basename!=file) && s.Index(re) == kNPOS) continue;
         l.Add(new TObjString(file));
      }
      gSystem->FreeDirectory(dir);
      //sort the files in alphanumeric order
      l.Sort();
      TIter next(&l);
      TObjString *obj;
      while ((obj = (TObjString*)next())) {
         file = obj->GetName();
         if (behind_dot_root.Length() != 0)
              BkgFileList->Add (TFile::Open (Form("%s/%s/%s",directory.Data(),file,behind_dot_root.Data())) ) ;
//            nf += AddFile(Form("%s/%s/%s",directory.Data(),file,behind_dot_root.Data()),kBigNumber);
         else
              BkgFileList->Add (TFile::Open (Form("%s/%s",directory.Data(),file)) ) ;
//            nf += AddFile(Form("%s/%s",directory.Data(),file),kBigNumber);
      }
      l.Delete();
   }

   return nf ;
}



