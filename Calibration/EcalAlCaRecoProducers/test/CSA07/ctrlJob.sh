#! /bin/sh
echo "---- jobs monitor --------- "
echo "running : "
bjobs | grep RUN | wc | awk '{print $1}'
echo "pending : "
bjobs | grep PEND | wc | awk '{print $1}' 
echo "suspended : "
bjobs | grep SUSP | wc | awk '{print $1}' 
echo "total   : "
bjobs | grep -v FROM_HOST | wc | awk '{print $1}' 
echo "---- files monitor -------- "
echo "data   : "
rfdir /castor/cern.ch/cms/store/cmscaf/alca/calibration/IMA/data | grep root | wc | awk '{print $1}' 
echo "logs   : "
rfdir /castor/cern.ch/cms/store/cmscaf/alca/calibration/IMA/logs | grep log | wc | awk '{print $1}' 
