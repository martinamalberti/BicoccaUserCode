/*

  This macro will add histograms multiplied by their cross-section
  from a list of root files and write them
  to a target root file. The target file is newly created and must not be
  identical to one of the source files.


  Author: Sven A. Schmidt, sven.schmidt@cern.ch
  Date:   13.2.2001

  Editing Author: Michael B. Anderson, mbanderson@hep.wisc.edu
  Date:  July 12, 2007

  This code is based on the hadd.C example by Rene Brun and Dirk Geppert,
  which had a problem with directories more than one level deep.
  (see macro hadd_old.C for this previous implementation).

  The macro from Sven has been enhanced by
     Anne-Sylvie Nicollerat <Anne-Sylvie.Nicollerat@cern.ch>
   to automatically add Trees (via a chain of trees).

  To use this macro, modify the file names in function hadd.

  NB: This macro is provided as a tutorial.
      Use $ROOTSYS/bin/hadd to merge many histogram files

 */


#include <string.h>
#include "TChain.h"
#include "TFile.h"
#include "TH1.h"
#include "TTree.h"
#include "TKey.h"
#include "Riostream.h"
#include "TNtuple.h"

TList *FileList;
TFile *Target;

void MergeRootfile( TDirectory *target, TList *sourcelist, double crossArray[] );


void hadd() {
   // in an interactive ROOT session, edit the file names
   // Target and FileList, then
   // root > .L hadd.C
   // root > hadd()

  Target = TFile::Open( "ttnjWeighted_histos.root", "RECREATE" );

  FileList = new TList();

  // ************************************************************ ttbar *********
  // List of Files
  FileList->Add( TFile::Open("tt0j_histos.root") );// tt0j
  FileList->Add( TFile::Open("tt1j_histos.root") );// tt1j
  FileList->Add( TFile::Open("tt2j_histos.root") );// tt2j
  FileList->Add( TFile::Open("tt3j_histos.root") );// tt3j
  FileList->Add( TFile::Open("tt4j_histos.root") );// tt4j

  // List of Cross-sections (pb) divided by num of events produced
  double crossSections[5]= {334.51/42000. // tt0j 
							90.23/26082., // tt1j
							8.75/20000.,  // tt2j
							3.16/12000.,   // tt3j
							0.82/2959.};  // tt4j
						
  // ****************************************************** Zeej *************
  /*
  Target = TFile::Open( "ZeejWeighted_histos.root", "RECREATE" );

  FileList = new TList();

  //List of Files
  FileList->Add( TFile::Open("Zeej_0_15_histos.root") );// 
  FileList->Add( TFile::Open("Zeej_15_20_histos.root") );//
  FileList->Add( TFile::Open("Zeej_20_30_histos.root") );// 
  FileList->Add( TFile::Open("Zeej_30_50_histos.root") );// 
  FileList->Add( TFile::Open("Zeej_50_80_histos.root") );// 
  FileList->Add( TFile::Open("Zeej_80_120_histos.root") );// 
  FileList->Add( TFile::Open("Zeej_120_170_histos.root") );//
  FileList->Add( TFile::Open("Zeej_170_230_histos.root") );//

  // List of Cross-sections (pb) divided by num of events produced
  double crossSections[8]= {104400/6000, // 0-15
                            353.1/6000, // 15 -20
                            326.7/6000, // 20 -30 
							227.0/19997, // 30-50
							93.17/6000,  // 50-80
							31.48/6000, // 80-120
							9.63/6000, // 120-170
							2.92/4000}; // 170-230
*/
  MergeRootfile( Target, FileList, crossSections );

}

void MergeRootfile( TDirectory *target, TList *sourcelist, double crossArray[] ) {

  //  cout << "Target path: " << target->GetPath() << endl;
  TString path( (char*)strstr( target->GetPath(), ":" ) );
  path.Remove( 0, 2 );

  TFile *first_source = (TFile*)sourcelist->First();

  first_source->cd( path );
  TDirectory *current_sourcedir = gDirectory;
  //gain time, do not add the objects in the list in memory
  Bool_t status = TH1::AddDirectoryStatus();
  TH1::AddDirectory(kFALSE);

  // loop over all keys in this directory
  TChain *globChain = 0;
  TIter nextkey( current_sourcedir->GetListOfKeys() );
  TKey *key, *oldkey=0;
  while ( (key = (TKey*)nextkey())) {

    //keep only the highest cycle number for each key
    if (oldkey && !strcmp(oldkey->GetName(),key->GetName())) continue;

    // read object from first source file
    first_source->cd( path );
    TObject *obj = key->ReadObj();

    if ( obj->IsA()->InheritsFrom( "TH1" ) ) {
      // descendant of TH1 -> merge it

      cout << "Merging histogram " << obj->GetName() << endl;
      TH1 *h1 = (TH1*)obj;

      // Scale by the cross-section factor
      h1->Scale(crossArray[0]);

      // loop over all source files and add the content of the
      // correspondant histogram to the one pointed to by "h1"
      TFile *nextsource = (TFile*)sourcelist->After( first_source );

      int q = 1; // This keeps track of which
                 // cross section factor to use

      while ( nextsource ) {
        // make sure we are at the correct directory level by cd'ing to path
        nextsource->cd( path );
        TKey *key2 = (TKey*)gDirectory->GetListOfKeys()->FindObject(h1->GetName());
        if (key2) {
           TH1 *h2 = (TH1*)key2->ReadObj();

	   // Scale by the cross section factor
           // before adding.
           h2->Scale(crossArray[q]);
           h1->Add( h2 );
           q++;
           delete h2;
        }

        nextsource = (TFile*)sourcelist->After( nextsource );
      }
    }
    else if ( obj->IsA()->InheritsFrom( "TTree" ) ) {

      // loop over all source files create a chain of Trees "globChain"
      const char* obj_name= obj->GetName();

      globChain = new TChain(obj_name);
      globChain->Add(first_source->GetName());
      TFile *nextsource = (TFile*)sourcelist->After( first_source );
      //      const char* file_name = nextsource->GetName();
      // cout << "file name  " << file_name << endl;
     while ( nextsource ) {

       globChain->Add(nextsource->GetName());
       nextsource = (TFile*)sourcelist->After( nextsource );
     }
	 }	 
     else if ( obj->IsA()->InheritsFrom( "TDirectory" ) ) {
      // it's a subdirectory

      cout << "Found subdirectory " << obj->GetName() << endl;

      // create a new subdir of same name and title in the target file
      target->cd();
      TDirectory *newdir = target->mkdir( obj->GetName(), obj->GetTitle() );

      // newdir is now the starting point of another round of merging
      // newdir still knows its depth within the target file via
      // GetPath(), so we can still figure out where we are in the recursion
      MergeRootfile( newdir, sourcelist, crossArray );

    } else {

      // object is of no type that we know or can handle
      cout << "Unknown object type, name: "
           << obj->GetName() << " title: " << obj->GetTitle() << endl;
    }

    // now write the merged histogram (which is "in" obj) to the target file
    // note that this will just store obj in the current directory level,
    // which is not persistent until the complete directory itself is stored
    // by "target->Write()" below
    if ( obj ) {
      target->cd();

      //!!if the object is a tree, it is stored in globChain...
	if(obj->IsA()->InheritsFrom( "TTree" ))
          globChain->Merge(target->GetFile(),0,"keep");
	else
	obj->Write( key->GetName() );
    }

  } // while ( ( TKey *key = (TKey*)nextkey() ) )

  // save modifications to target file
  target->SaveSelf(kTRUE);
  TH1::AddDirectory(status);
}
