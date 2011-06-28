void draw(TString nameInFileRoot, TString nameOutputDir, TString nameDATA){
 
 
 TFile* fileIn = new TFile(nameInFileRoot);
 
 
 
 TString nameHisto;
 TCanvas* cc;
 TH1F* h;
 TH1F* hMC;
 TCanvas* cJoint = new TCanvas("cJoint","cJoint",1000,1000);
 cJoint->SetLeftMargin(0.10);
 cJoint->SetRightMargin(0.25);
 cJoint->SetTopMargin(0.05);
 cJoint->SetBottomMargin(0.20);
 
 
 
 TString toDoShell;
 toDoShell = Form("rm -r %s ;",nameOutputDir.Data());
 system(toDoShell.Data());
 toDoShell = Form("mkdir %s ;",nameOutputDir.Data());
 system(toDoShell.Data());
 toDoShell = Form("mkdir %s/plot ;",nameOutputDir.Data());
 system(toDoShell.Data());
 
 TString toDoROOT;
 
 std::vector<double> DATA_Dphi_ePlus;
 std::vector<double> DATA_Dphi_eMinus;
 std::vector<double> DATA_Deta;
 std::vector<double> DATA_err_Dphi_ePlus;
 std::vector<double> DATA_err_Dphi_eMinus;
 std::vector<double> DATA_err_Deta;

 std::vector<double> MC_Dphi_ePlus;
 std::vector<double> MC_Dphi_eMinus;
 std::vector<double> MC_Deta;
 std::vector<double> MC_err_Dphi_ePlus;
 std::vector<double> MC_err_Dphi_eMinus;
 std::vector<double> MC_err_Deta;
 
 
 ///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 
 ///=============
 ///==== EE+ ====
 
 ///~~~~ Dphi e+ ~~~~
 cc = (TCanvas*) fileIn->Get("All/Var/10_7_Canvas");
 nameHisto = Form ("%s_10_7_Tot_temp",nameDATA.Data());
 h = (TH1F*) cc->GetPrimitive(nameHisto);
 hMC = (TH1F*) cc->GetPrimitive("W_10_7_Tot_temp");
 cJoint->cd();
 h->Draw();
 hMC->Draw("HISTsame");
 cJoint->SetGrid();
 TLegend* leg = cJoint->BuildLegend(0.8,0.55,0.98,0.75);
 leg->SetFillColor(kWhite);
 cJoint->SetGrid(); 
 
 toDoROOT = Form("%s/plot/Dphi_EEPlus_eplus.png",nameOutputDir.Data());
 cJoint->SaveAs(toDoROOT);
 cJoint->SaveAs(toDoROOT);
 
 DATA_Dphi_ePlus.push_back(h->GetMean());
 DATA_err_Dphi_ePlus.push_back(h->GetRMS() /sqrt( h->GetEffectiveEntries() ));
 MC_Dphi_ePlus.push_back(hMC->GetMean());
 MC_err_Dphi_ePlus.push_back(hMC->GetRMS() /sqrt( hMC->GetEffectiveEntries() ));
 
 ///~~~~ Dphi e- ~~~~ 
 cc = (TCanvas*) fileIn->Get("All/Var/16_7_Canvas");
 nameHisto = Form ("%s_16_7_Tot_temp",nameDATA.Data());
 h = (TH1F*) cc->GetPrimitive(nameHisto);
 hMC = (TH1F*) cc->GetPrimitive("W_16_7_Tot_temp");
 cJoint->cd();
 h->Draw();
 hMC->Draw("HISTsame");
 cJoint->SetGrid();
 TLegend* leg = cJoint->BuildLegend(0.8,0.55,0.98,0.75);
 leg->SetFillColor(kWhite);
 cJoint->SetGrid(); 
 
 toDoROOT = Form("%s/plot/Dphi_EEPlus_eminus.png",nameOutputDir.Data());
 cJoint->SaveAs(toDoROOT);
 
 DATA_Dphi_eMinus.push_back(h->GetMean());
 DATA_err_Dphi_eMinus.push_back(h->GetRMS() /sqrt( h->GetEffectiveEntries() ));
 MC_Dphi_eMinus.push_back(hMC->GetMean());
 MC_err_Dphi_eMinus.push_back(hMC->GetRMS() /sqrt( hMC->GetEffectiveEntries() ));
 
 ///~~~~ Deta ~~~~
 cc = (TCanvas*) fileIn->Get("All/Var/4_6_Canvas");
 nameHisto = Form ("%s_4_6_Tot_temp",nameDATA.Data());
 h = (TH1F*) cc->GetPrimitive(nameHisto);
 hMC = (TH1F*) cc->GetPrimitive("W_4_6_Tot_temp");
 cJoint->cd();
 h->Draw();
 hMC->Draw("HISTsame");
 cJoint->SetGrid();
 TLegend* leg = cJoint->BuildLegend(0.8,0.55,0.98,0.75);
 leg->SetFillColor(kWhite);
 cJoint->SetGrid(); 
 
 toDoROOT = Form("%s/plot/Deta_EEPlus.png",nameOutputDir.Data());
 cJoint->SaveAs(toDoROOT);
 
 DATA_Deta.push_back(h->GetMean());
 DATA_err_Deta.push_back(h->GetRMS() /sqrt( h->GetEffectiveEntries() ));
 MC_Deta.push_back(hMC->GetMean());
 MC_err_Deta.push_back(hMC->GetRMS() /sqrt( hMC->GetEffectiveEntries() ));
 
 
 ///=============
 ///==== EB+ ====
 
 ///~~~~ Dphi e+ ~~~~
 cc = (TCanvas*) fileIn->Get("All/Var/9_7_Canvas");
 nameHisto = Form ("%s_9_7_Tot_temp",nameDATA.Data());
 h = (TH1F*) cc->GetPrimitive(nameHisto);
 hMC = (TH1F*) cc->GetPrimitive("W_9_7_Tot_temp");
 cJoint->cd();
 h->Draw();
 hMC->Draw("HISTsame");
 cJoint->SetGrid();
 TLegend* leg = cJoint->BuildLegend(0.8,0.55,0.98,0.75);
 leg->SetFillColor(kWhite);
 cJoint->SetGrid(); 
 
 toDoROOT = Form("%s/plot/Dphi_EBPlus_eplus.png",nameOutputDir.Data());
 cJoint->SaveAs(toDoROOT);
 
 DATA_Dphi_ePlus.push_back(h->GetMean());
 DATA_err_Dphi_ePlus.push_back(h->GetRMS() /sqrt( h->GetEffectiveEntries() ));
 MC_Dphi_ePlus.push_back(hMC->GetMean());
 MC_err_Dphi_ePlus.push_back(hMC->GetRMS() /sqrt( hMC->GetEffectiveEntries() ));
 
 ///~~~~ Dphi e- ~~~~ 
 cc = (TCanvas*) fileIn->Get("All/Var/15_7_Canvas");
 nameHisto = Form ("%s_15_7_Tot_temp",nameDATA.Data());
 h = (TH1F*) cc->GetPrimitive(nameHisto);
 hMC = (TH1F*) cc->GetPrimitive("W_15_7_Tot_temp");
 cJoint->cd();
 h->Draw();
 hMC->Draw("HISTsame");
 cJoint->SetGrid();
 TLegend* leg = cJoint->BuildLegend(0.8,0.55,0.98,0.75);
 leg->SetFillColor(kWhite);
 cJoint->SetGrid(); 
 
 toDoROOT = Form("%s/plot/Dphi_EBPlus_eminus.png",nameOutputDir.Data());
 cJoint->SaveAs(toDoROOT);
 
 DATA_Dphi_eMinus.push_back(h->GetMean());
 DATA_err_Dphi_eMinus.push_back(h->GetRMS() /sqrt( h->GetEffectiveEntries() ));
 MC_Dphi_eMinus.push_back(hMC->GetMean());
 MC_err_Dphi_eMinus.push_back(hMC->GetRMS() /sqrt( hMC->GetEffectiveEntries() ));
 
 ///~~~~ Deta ~~~~
 cc = (TCanvas*) fileIn->Get("All/Var/3_6_Canvas");
 nameHisto = Form ("%s_3_6_Tot_temp",nameDATA.Data());
 h = (TH1F*) cc->GetPrimitive(nameHisto);
 hMC = (TH1F*) cc->GetPrimitive("W_3_6_Tot_temp");
 cJoint->cd();
 h->Draw();
 hMC->Draw("HISTsame");
 cJoint->SetGrid();
 TLegend* leg = cJoint->BuildLegend(0.8,0.55,0.98,0.75);
 leg->SetFillColor(kWhite);
 cJoint->SetGrid(); 
 
 toDoROOT = Form("%s/plot/Deta_EBPlus.png",nameOutputDir.Data());
 cJoint->SaveAs(toDoROOT);
 
 DATA_Deta.push_back(h->GetMean());
 DATA_err_Deta.push_back(h->GetRMS() /sqrt( h->GetEffectiveEntries() ));
 MC_Deta.push_back(hMC->GetMean());
 MC_err_Deta.push_back(hMC->GetRMS() /sqrt( hMC->GetEffectiveEntries() ));
 
 
 ///==== EB- ====
 ///~~~~ Dphi e+ ~~~~
 cc = (TCanvas*) fileIn->Get("All/Var/11_7_Canvas");
 nameHisto = Form ("%s_11_7_Tot_temp",nameDATA.Data());
 h = (TH1F*) cc->GetPrimitive(nameHisto);
 hMC = (TH1F*) cc->GetPrimitive("W_11_7_Tot_temp");
 cJoint->cd();
 h->Draw();
 hMC->Draw("HISTsame");
 cJoint->SetGrid();
 TLegend* leg = cJoint->BuildLegend(0.8,0.55,0.98,0.75);
 leg->SetFillColor(kWhite);
 cJoint->SetGrid(); 
 
 toDoROOT = Form("%s/plot/Dphi_EBMinus_eplus.png",nameOutputDir.Data());
 cJoint->SaveAs(toDoROOT);
 
 DATA_Dphi_ePlus.push_back(h->GetMean());
 DATA_err_Dphi_ePlus.push_back(h->GetRMS() /sqrt( h->GetEffectiveEntries() ));
 MC_Dphi_ePlus.push_back(hMC->GetMean());
 MC_err_Dphi_ePlus.push_back(hMC->GetRMS() /sqrt( hMC->GetEffectiveEntries() ));
 
 ///~~~~ Dphi e- ~~~~ 
 cc = (TCanvas*) fileIn->Get("All/Var/17_7_Canvas");
 nameHisto = Form ("%s_17_7_Tot_temp",nameDATA.Data());
 h = (TH1F*) cc->GetPrimitive(nameHisto);
 hMC = (TH1F*) cc->GetPrimitive("W_17_7_Tot_temp");
 cJoint->cd();
 h->Draw();
 hMC->Draw("HISTsame");
 cJoint->SetGrid();
 TLegend* leg = cJoint->BuildLegend(0.8,0.55,0.98,0.75);
 leg->SetFillColor(kWhite);
 cJoint->SetGrid(); 
 
 toDoROOT = Form("%s/plot/Dphi_EBMinus_eminus.png",nameOutputDir.Data());
 cJoint->SaveAs(toDoROOT);
 
 DATA_Dphi_eMinus.push_back(h->GetMean());
 DATA_err_Dphi_eMinus.push_back(h->GetRMS() /sqrt( h->GetEffectiveEntries() ));
 MC_Dphi_eMinus.push_back(hMC->GetMean());
 MC_err_Dphi_eMinus.push_back(hMC->GetRMS() /sqrt( hMC->GetEffectiveEntries() ));
 
 ///~~~~ Deta ~~~~
 cc = (TCanvas*) fileIn->Get("All/Var/5_6_Canvas");
 nameHisto = Form ("%s_5_6_Tot_temp",nameDATA.Data());
 h = (TH1F*) cc->GetPrimitive(nameHisto);
 hMC = (TH1F*) cc->GetPrimitive("W_5_6_Tot_temp");
 cJoint->cd();
 h->Draw();
 hMC->Draw("HISTsame");
 cJoint->SetGrid();
 TLegend* leg = cJoint->BuildLegend(0.8,0.55,0.98,0.75);
 leg->SetFillColor(kWhite);
 cJoint->SetGrid(); 
 
 toDoROOT = Form("%s/plot/Deta_EBMinus.png",nameOutputDir.Data());
 cJoint->SaveAs(toDoROOT);
 
 DATA_Deta.push_back(h->GetMean());
 DATA_err_Deta.push_back(h->GetRMS() /sqrt( h->GetEffectiveEntries() ));
 MC_Deta.push_back(hMC->GetMean());
 MC_err_Deta.push_back(hMC->GetRMS() /sqrt( hMC->GetEffectiveEntries() ));
 
 
 ///=============
 ///==== EE- ====
 
 ///~~~~ Dphi e+ ~~~~
 cc = (TCanvas*) fileIn->Get("All/Var/12_7_Canvas");
 nameHisto = Form ("%s_12_7_Tot_temp",nameDATA.Data());
 h = (TH1F*) cc->GetPrimitive(nameHisto);
 hMC = (TH1F*) cc->GetPrimitive("W_12_7_Tot_temp");
 cJoint->cd();
 h->Draw();
 hMC->Draw("HISTsame");
 cJoint->SetGrid();
 TLegend* leg = cJoint->BuildLegend(0.8,0.55,0.98,0.75);
 leg->SetFillColor(kWhite);
 cJoint->SetGrid(); 
 
 toDoROOT = Form("%s/plot/Dphi_EEMinus_eplus.png",nameOutputDir.Data());
 cJoint->SaveAs(toDoROOT);
 
 DATA_Dphi_ePlus.push_back(h->GetMean());
 DATA_err_Dphi_ePlus.push_back(h->GetRMS() /sqrt( h->GetEffectiveEntries() ));
 MC_Dphi_ePlus.push_back(hMC->GetMean());
 MC_err_Dphi_ePlus.push_back(hMC->GetRMS() /sqrt( hMC->GetEffectiveEntries() ));
 
 ///~~~~ Dphi e- ~~~~ 
 cc = (TCanvas*) fileIn->Get("All/Var/18_7_Canvas");
 nameHisto = Form ("%s_18_7_Tot_temp",nameDATA.Data());
 h = (TH1F*) cc->GetPrimitive(nameHisto);
 hMC = (TH1F*) cc->GetPrimitive("W_18_7_Tot_temp");
 cJoint->cd();
 h->Draw();
 hMC->Draw("HISTsame");
 cJoint->SetGrid();
 TLegend* leg = cJoint->BuildLegend(0.8,0.55,0.98,0.75);
 leg->SetFillColor(kWhite);
 cJoint->SetGrid(); 
 
 toDoROOT = Form("%s/plot/Dphi_EEMinus_eminus.png",nameOutputDir.Data());
 cJoint->SaveAs(toDoROOT);
 
 DATA_Dphi_eMinus.push_back(h->GetMean());
 DATA_err_Dphi_eMinus.push_back(h->GetRMS() /sqrt( h->GetEffectiveEntries() ));
 MC_Dphi_eMinus.push_back(hMC->GetMean());
 MC_err_Dphi_eMinus.push_back(hMC->GetRMS() /sqrt( hMC->GetEffectiveEntries() ));
 
 ///~~~~ Deta ~~~~
 cc = (TCanvas*) fileIn->Get("All/Var/6_6_Canvas");
 nameHisto = Form ("%s_6_6_Tot_temp",nameDATA.Data());
 h = (TH1F*) cc->GetPrimitive(nameHisto);
 hMC = (TH1F*) cc->GetPrimitive("W_6_6_Tot_temp");
 cJoint->cd();
 h->Draw();
 hMC->Draw("HISTsame");
 cJoint->SetGrid();
 TLegend* leg = cJoint->BuildLegend(0.8,0.55,0.98,0.75);
 leg->SetFillColor(kWhite);
 cJoint->SetGrid(); 
 
 toDoROOT = Form("%s/plot/Deta_EEMinus.png",nameOutputDir.Data());
 cJoint->SaveAs(toDoROOT);
 
 DATA_Deta.push_back(h->GetMean());
 DATA_err_Deta.push_back(h->GetRMS() /sqrt( h->GetEffectiveEntries() ));
 MC_Deta.push_back(hMC->GetMean());
 MC_err_Deta.push_back(hMC->GetRMS() /sqrt( hMC->GetEffectiveEntries() ));
 
 
 ///========================
 ///==== to create HTML ====
 
 
 toDoShell = Form("cp scripts/html/template.html %s/",nameOutputDir.Data());
 system(toDoShell.Data());
 
 std::string toCreateHLTML;
 toCreateHLTML.clear();
 toCreateHLTML.insert(toCreateHLTML.size(),"cat scripts/html/template.html ");
 //  toCreateHLTML.insert(toCreateHLTML.size(),"| sed -e s%NAMEDIR%");
 //  toCreateHLTML.insert(toCreateHLTML.size(),nameOutputDir.Data());
 //  toCreateHLTML.insert(toCreateHLTML.size(),"%g ");

 std::ostringstream toCreateHLTML_strs;
 toCreateHLTML_strs.precision (3) ;
 toCreateHLTML_strs.unsetf(std::ios::scientific); 
 
 toCreateHLTML_strs << " | sed -e s%EE+DPHIE+VALUEDATA%" << DATA_Dphi_ePlus.at(0)  << "£#plusmn@£" << DATA_err_Dphi_ePlus.at(0)  << "%g ";
 toCreateHLTML_strs << " | sed -e s%EE+DPHIE-VALUEDATA%" << DATA_Dphi_eMinus.at(0) << "£#plusmn@£" << DATA_err_Dphi_eMinus.at(0) << "%g ";
 toCreateHLTML_strs << " | sed -e s%EE+DETAVALUEDATA%"   << DATA_Deta.at(0)        << "£#plusmn@£" << DATA_err_Deta.at(0)        << "%g ";

 toCreateHLTML_strs << " | sed -e s%EB+DPHIE+VALUEDATA%" << DATA_Dphi_ePlus.at(1)  << "£#plusmn@£" << DATA_err_Dphi_ePlus.at(1)  << "%g ";
 toCreateHLTML_strs << " | sed -e s%EB+DPHIE-VALUEDATA%" << DATA_Dphi_eMinus.at(1) << "£#plusmn@£" << DATA_err_Dphi_eMinus.at(1) << "%g ";
 toCreateHLTML_strs << " | sed -e s%EB+DETAVALUEDATA%"   << DATA_Deta.at(1)        << "£#plusmn@£" << DATA_err_Deta.at(1)        << "%g ";
 
 toCreateHLTML_strs << " | sed -e s%EB-DPHIE+VALUEDATA%" << DATA_Dphi_ePlus.at(2)  << "£#plusmn@£" << DATA_err_Dphi_ePlus.at(2)  << "%g ";
 toCreateHLTML_strs << " | sed -e s%EB-DPHIE-VALUEDATA%" << DATA_Dphi_eMinus.at(2) << "£#plusmn@£" << DATA_err_Dphi_eMinus.at(2) << "%g ";
 toCreateHLTML_strs << " | sed -e s%EB-DETAVALUEDATA%"   << DATA_Deta.at(2)        << "£#plusmn@£" << DATA_err_Deta.at(2)        << "%g ";
 
 toCreateHLTML_strs << " | sed -e s%EE-DPHIE+VALUEDATA%" << DATA_Dphi_ePlus.at(3)  << "£#plusmn@£" << DATA_err_Dphi_ePlus.at(3)  << "%g ";
 toCreateHLTML_strs << " | sed -e s%EE-DPHIE-VALUEDATA%" << DATA_Dphi_eMinus.at(3) << "£#plusmn@£" << DATA_err_Dphi_eMinus.at(3) << "%g ";
 toCreateHLTML_strs << " | sed -e s%EE-DETAVALUEDATA%"   << DATA_Deta.at(3)        << "£#plusmn@£" << DATA_err_Deta.at(3)        << "%g ";
 
 
 
 
 toCreateHLTML_strs << " | sed -e s%EE+DPHIE+VALUEMC%" << MC_Dphi_ePlus.at(0)  << "£#plusmn@£" << MC_err_Dphi_ePlus.at(0)  << "%g ";
 toCreateHLTML_strs << " | sed -e s%EE+DPHIE-VALUEMC%" << MC_Dphi_eMinus.at(0) << "£#plusmn@£" << MC_err_Dphi_eMinus.at(0) << "%g ";
 toCreateHLTML_strs << " | sed -e s%EE+DETAVALUEMC%"   << MC_Deta.at(0)        << "£#plusmn@£" << MC_err_Deta.at(0)        << "%g ";
 
 toCreateHLTML_strs << " | sed -e s%EB+DPHIE+VALUEMC%" << MC_Dphi_ePlus.at(1)  << "£#plusmn@£" << MC_err_Dphi_ePlus.at(1)  << "%g ";
 toCreateHLTML_strs << " | sed -e s%EB+DPHIE-VALUEMC%" << MC_Dphi_eMinus.at(1) << "£#plusmn@£" << MC_err_Dphi_eMinus.at(1) << "%g ";
 toCreateHLTML_strs << " | sed -e s%EB+DETAVALUEMC%"   << MC_Deta.at(1)        << "£#plusmn@£" << MC_err_Deta.at(1)        << "%g ";
 
 toCreateHLTML_strs << " | sed -e s%EB-DPHIE+VALUEMC%" << MC_Dphi_ePlus.at(2)  << "£#plusmn@£" << MC_err_Dphi_ePlus.at(2)  << "%g ";
 toCreateHLTML_strs << " | sed -e s%EB-DPHIE-VALUEMC%" << MC_Dphi_eMinus.at(2) << "£#plusmn@£" << MC_err_Dphi_eMinus.at(2) << "%g ";
 toCreateHLTML_strs << " | sed -e s%EB-DETAVALUEMC%"   << MC_Deta.at(2)        << "£#plusmn@£" << MC_err_Deta.at(2)        << "%g ";
 
 toCreateHLTML_strs << " | sed -e s%EE-DPHIE+VALUEMC%" << MC_Dphi_ePlus.at(3)  << "£#plusmn@£" << MC_err_Dphi_ePlus.at(3)  << "%g ";
 toCreateHLTML_strs << " | sed -e s%EE-DPHIE-VALUEMC%" << MC_Dphi_eMinus.at(3) << "£#plusmn@£" << MC_err_Dphi_eMinus.at(3) << "%g ";
 toCreateHLTML_strs << " | sed -e s%EE-DETAVALUEMC%"   << MC_Deta.at(3)        << "£#plusmn@£" << MC_err_Deta.at(3)        << "%g ";
 
 
 toCreateHLTML_strs << " | tr \"#\" \"&\" | tr \"@\" \";\" | tr \"£\" \" \"";
 
 
 
 
 
 
 toCreateHLTML.insert(toCreateHLTML.size(),toCreateHLTML_strs.str());
 
 toCreateHLTML.insert(toCreateHLTML.size()," > ");
 toCreateHLTML.insert(toCreateHLTML.size(),nameOutputDir.Data());
 toCreateHLTML.insert(toCreateHLTML.size(),"/ECALAlignment.html");
 
//  std::cout << " toCreateHLTML = " << toCreateHLTML << std::endl;
 system(toCreateHLTML.c_str());
 
//  DATA_Dphi_ePlus.push_back(h->GetMean());
//  DATA_err_Dphi_ePlus.push_back(h->GetRMS() /sqrt( h->GetEffectiveEntries() ));
//  MC_Dphi_ePlus.push_back(hMC->GetMean());
//  MC_err_Dphi_ePlus.push_back(hMC->GetRMS() /sqrt( hMC->GetEffectiveEntries() ));
//  DATA_Dphi_eMinus.push_back(h->GetMean());
//  DATA_err_Dphi_eMinus.push_back(h->GetRMS() /sqrt( h->GetEffectiveEntries() ));
//  MC_Dphi_eMinus.push_back(hMC->GetMean());
//  MC_err_Dphi_eMinus.push_back(hMC->GetRMS() /sqrt( hMC->GetEffectiveEntries() ));
//  DATA_Deta.push_back(h->GetMean());
//  DATA_err_Deta.push_back(h->GetRMS() /sqrt( h->GetEffectiveEntries() ));
//  MC_Deta.push_back(hMC->GetMean());
//  MC_err_Deta.push_back(hMC->GetRMS() /sqrt( hMC->GetEffectiveEntries() ));
 
 
 
 ///==== save in txt file the results ====
 
 std::ofstream toCreateTXT_file;
 TString nameFile = Form ("%s/result.txt",nameOutputDir.Data());
 toCreateTXT_file.open (nameFile.Data());
 
 toCreateTXT_file.precision (3) ;
 toCreateTXT_file.unsetf(std::ios::scientific); 
 
 toCreateTXT_file << " " << DATA_Dphi_ePlus.at(0)  << " " << DATA_err_Dphi_ePlus.at(0)  << std::endl;
 toCreateTXT_file << " " << DATA_Dphi_eMinus.at(0) << " " << DATA_err_Dphi_eMinus.at(0) << std::endl;
 toCreateTXT_file << " " << DATA_Deta.at(0)        << " " << DATA_err_Deta.at(0)        << std::endl;
 
 toCreateTXT_file << " " << DATA_Dphi_ePlus.at(1)  << " " << DATA_err_Dphi_ePlus.at(1)  << std::endl;
 toCreateTXT_file << " " << DATA_Dphi_eMinus.at(1) << " " << DATA_err_Dphi_eMinus.at(1) << std::endl;
 toCreateTXT_file << " " << DATA_Deta.at(1)        << " " << DATA_err_Deta.at(1)        << std::endl;
 
 toCreateTXT_file << " " << DATA_Dphi_ePlus.at(2)  << " " << DATA_err_Dphi_ePlus.at(2)  << std::endl;
 toCreateTXT_file << " " << DATA_Dphi_eMinus.at(2) << " " << DATA_err_Dphi_eMinus.at(2) << std::endl;
 toCreateTXT_file << " " << DATA_Deta.at(2)        << " " << DATA_err_Deta.at(2)        << std::endl;
 
 toCreateTXT_file << " " << DATA_Dphi_ePlus.at(3)  << " " << DATA_err_Dphi_ePlus.at(3)  << std::endl;
 toCreateTXT_file << " " << DATA_Dphi_eMinus.at(3) << " " << DATA_err_Dphi_eMinus.at(3) << std::endl;
 toCreateTXT_file << " " << DATA_Deta.at(3)        << " " << DATA_err_Deta.at(3)        << std::endl;
 
 
 
 
 toCreateTXT_file << " " << MC_Dphi_ePlus.at(0)  << " " << MC_err_Dphi_ePlus.at(0)  << std::endl;
 toCreateTXT_file << " " << MC_Dphi_eMinus.at(0) << " " << MC_err_Dphi_eMinus.at(0) << std::endl;
 toCreateTXT_file << " " << MC_Deta.at(0)        << " " << MC_err_Deta.at(0)        << std::endl;
 
 toCreateTXT_file << " " << MC_Dphi_ePlus.at(1)  << " " << MC_err_Dphi_ePlus.at(1)  << std::endl;
 toCreateTXT_file << " " << MC_Dphi_eMinus.at(1) << " " << MC_err_Dphi_eMinus.at(1) << std::endl;
 toCreateTXT_file << " " << MC_Deta.at(1)        << " " << MC_err_Deta.at(1)        << std::endl;
 
 toCreateTXT_file << " " << MC_Dphi_ePlus.at(2)  << " " << MC_err_Dphi_ePlus.at(2)  << std::endl;
 toCreateTXT_file << " " << MC_Dphi_eMinus.at(2) << " " << MC_err_Dphi_eMinus.at(2) << std::endl;
 toCreateTXT_file << " " << MC_Deta.at(2)        << " " << MC_err_Deta.at(2)        << std::endl;
 
 toCreateTXT_file << " " << MC_Dphi_ePlus.at(3)  << " " << MC_err_Dphi_ePlus.at(3)  << std::endl;
 toCreateTXT_file << " " << MC_Dphi_eMinus.at(3) << " " << MC_err_Dphi_eMinus.at(3) << std::endl;
 toCreateTXT_file << " " << MC_Deta.at(3)        << " " << MC_err_Deta.at(3)        << std::endl;
 
 toCreateTXT_file.close();
 
 
 ///==== exit ====
 gApplication->Terminate(0);
 
 
}