{
  
  //Electrons
  float r1_ele = 1.87842;
  float err_r1_ele = 0.381863;
  
  float r2_ele = 2.585;
  float err_r2_ele = 1.06903;
  
  //Muons
  float r1_mu = 3.43;
  float err_r1_mu = 0.37;
  
  float r2_mu = 6.9;
  float err_r2_mu = 1.54;

  //Get Double ratios
  float r1_muOverEle = r1_mu/r1_ele;
  float r2_muOverEle = r2_mu/r2_ele;
  
  //Get Double ratios errors
  float err_r1_muOverEle = r1_muOverEle * 
      sqrt ( (err_r1_mu/r1_mu)*(err_r1_mu/r1_mu) + (err_r1_ele/r1_ele)*(err_r1_ele/r1_ele) );
  
  float err_r2_muOverEle = r2_muOverEle * 
      sqrt ( (err_r2_mu/r2_mu)*(err_r2_mu/r2_mu) + (err_r2_ele/r2_ele)*(err_r2_ele/r2_ele) );
  
  std::cout << "r1_muOverEle " <<  r1_muOverEle << " +- " << err_r1_muOverEle << std::endl;
  std::cout << "r2_muOverEle " <<  r2_muOverEle << " +- " << err_r2_muOverEle << std::endl;

}