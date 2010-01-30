
#ifndef looper_01_h
#define looper_01_h

#include "smallHBaseClass.h"

class looper_01 : public smallHBaseClass
{

  public :

    looper_01 (TTree * tree): smallHBaseClass (tree) {} ;

    virtual void Begin () ;
    virtual void Loop () ;
    virtual void Terminate () ;


  public :
  
    //PG put your histos here

} ;

#endif
