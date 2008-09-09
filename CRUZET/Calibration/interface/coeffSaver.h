#ifndef coeffSaver_h 
#define coeffSaver_h   
#include <iostream> 
#include <fstream>   
#include <map>   

class coeffSaver 
{  
  public :  
  coeffSaver(){};  
  ~coeffSaver(){};    
  int save (std::string nomeFile, const std::map<int, double> coeffs)   
        {  
        std::ofstream fout (nomeFile.c_str()) ;  
     //Cos'è?   fout << str <<endl;  
        for (std::map<int,double>::const_iterator mapIt = coeffs.begin () ;  mapIt != coeffs.end () ;  ++mapIt)  
             {  
	     fout << mapIt->first << "\t" << mapIt->second <<"\n" ;  
	     }  


        fout.close();     
        return 0 ;  

        }
   
} ;   








#endif
