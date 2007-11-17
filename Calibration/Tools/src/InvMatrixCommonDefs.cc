/**   
    $Date: 2006/12/15 16:12:28 $
    $Revision: 1.1 $
    $Id: InvMatrixCommonDefs.cc,v 1.1 2006/12/15 16:12:28 govoni Exp $ 
    \author $Author: govoni $
*/

#include "Calibration/Tools/interface/InvMatrixCommonDefs.h"

int
ecalIM::uniqueIndex (int eta, int phi)
{
  return eta * SCMaxPhi + phi ;
}
