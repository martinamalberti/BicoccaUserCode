#include "NNfunction_SinglePair_JetCorrection_NoMjj_NoDeta_ptSum.h"
#include <cmath>

double NNfunction::Value(int index,double in0,double in1,double in2,double in3,double in4,double in5) {
   input0 = (in0 - 0)/1;
   input1 = (in1 - 0)/1;
   input2 = (in2 - 0)/1;
   input3 = (in3 - 0)/1;
   input4 = (in4 - 0)/1;
   input5 = (in5 - 0)/1;
   switch(index) {
     case 0:
         return neuron0x90138f0();
     default:
         return 0.;
   }
}

double NNfunction::Value(int index, double* input) {
   input0 = (input[0] - 0)/1;
   input1 = (input[1] - 0)/1;
   input2 = (input[2] - 0)/1;
   input3 = (input[3] - 0)/1;
   input4 = (input[4] - 0)/1;
   input5 = (input[5] - 0)/1;
   switch(index) {
     case 0:
         return neuron0x90138f0();
     default:
         return 0.;
   }
}

double NNfunction::neuron0x9002270() {
   return input0;
}

double NNfunction::neuron0x90108a8() {
   return input1;
}

double NNfunction::neuron0x90109a8() {
   return input2;
}

double NNfunction::neuron0x9010b38() {
   return input3;
}

double NNfunction::neuron0x9010d30() {
   return input4;
}

double NNfunction::neuron0x9010f30() {
   return input5;
}

double NNfunction::input0x9011250() {
   double input = -0.1494;
   input += synapse0x9002038();
   input += synapse0x8fa9958();
   input += synapse0x8fa9a88();
   input += synapse0x90113e0();
   input += synapse0x9011408();
   input += synapse0x9011430();
   return input;
}

double NNfunction::neuron0x9011250() {
   double input = input0x9011250();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0x9011458() {
   double input = 0.288312;
   input += synapse0x9011630();
   input += synapse0x9011658();
   input += synapse0x9011680();
   input += synapse0x90116a8();
   input += synapse0x90116d0();
   input += synapse0x90116f8();
   return input;
}

double NNfunction::neuron0x9011458() {
   double input = input0x9011458();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0x9011720() {
   double input = -0.186566;
   input += synapse0x90118f8();
   input += synapse0x9011920();
   input += synapse0x9011948();
   input += synapse0x9011970();
   input += synapse0x9011998();
   input += synapse0x9011a48();
   return input;
}

double NNfunction::neuron0x9011720() {
   double input = input0x9011720();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0x9011a70() {
   double input = -0.154019;
   input += synapse0x9011c20();
   input += synapse0x9011c48();
   input += synapse0x9011c70();
   input += synapse0x9011c98();
   input += synapse0x9011cc0();
   input += synapse0x9011ce8();
   return input;
}

double NNfunction::neuron0x9011a70() {
   double input = input0x9011a70();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0x9011d10() {
   double input = -0.467907;
   input += synapse0x9011f08();
   input += synapse0x9011f30();
   input += synapse0x9011f58();
   input += synapse0x9011f80();
   input += synapse0x9011fa8();
   input += synapse0x9011fd0();
   return input;
}

double NNfunction::neuron0x9011d10() {
   double input = input0x9011d10();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0x9011ff8() {
   double input = -0.385373;
   input += synapse0x90121f0();
   input += synapse0x9012218();
   input += synapse0x9012240();
   input += synapse0x90119c0();
   input += synapse0x90119e8();
   input += synapse0x9011a10();
   return input;
}

double NNfunction::neuron0x9011ff8() {
   double input = input0x9011ff8();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0x9012370() {
   double input = 0.0629558;
   input += synapse0x9012568();
   input += synapse0x9012590();
   input += synapse0x90125b8();
   input += synapse0x90125e0();
   input += synapse0x9012608();
   input += synapse0x9012630();
   return input;
}

double NNfunction::neuron0x9012370() {
   double input = input0x9012370();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0x9012680() {
   double input = 0.155885;
   input += synapse0x9012658();
   input += synapse0x9012878();
   input += synapse0x90128a0();
   input += synapse0x90128c8();
   input += synapse0x90128f0();
   input += synapse0x9012918();
   input += synapse0x9012940();
   return input;
}

double NNfunction::neuron0x9012680() {
   double input = input0x9012680();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0x9012968() {
   double input = 0.131242;
   input += synapse0x9012b60();
   input += synapse0x9012b88();
   input += synapse0x9012bb0();
   input += synapse0x9012bd8();
   input += synapse0x9012c00();
   input += synapse0x9012c28();
   input += synapse0x9012c50();
   return input;
}

double NNfunction::neuron0x9012968() {
   double input = input0x9012968();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0x9012c78() {
   double input = -0.163281;
   input += synapse0x9012e70();
   input += synapse0x9012e98();
   input += synapse0x9012ec0();
   input += synapse0x9012ee8();
   input += synapse0x9012f10();
   input += synapse0x9012f38();
   input += synapse0x9012f60();
   return input;
}

double NNfunction::neuron0x9012c78() {
   double input = input0x9012c78();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0x9012f88() {
   double input = 0.0330343;
   input += synapse0x9013208();
   input += synapse0x9013230();
   input += synapse0x8fa9ab0();
   input += synapse0x8fa99c0();
   input += synapse0x8fa1f00();
   input += synapse0x90020d8();
   input += synapse0x9002100();
   return input;
}

double NNfunction::neuron0x9012f88() {
   double input = input0x9012f88();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0x9012268() {
   double input = 0.38685;
   input += synapse0x90021c0();
   input += synapse0x90134f0();
   input += synapse0x9013518();
   input += synapse0x9013540();
   input += synapse0x9013568();
   input += synapse0x9013590();
   input += synapse0x90135b8();
   return input;
}

double NNfunction::neuron0x9012268() {
   double input = input0x9012268();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0x90135e0() {
   double input = 0.13811;
   input += synapse0x90137d8();
   input += synapse0x9013800();
   input += synapse0x9013828();
   input += synapse0x9013850();
   input += synapse0x9013878();
   input += synapse0x90138a0();
   input += synapse0x90138c8();
   return input;
}

double NNfunction::neuron0x90135e0() {
   double input = input0x90135e0();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0x90138f0() {
   double input = 0.342552;
   input += synapse0x90139f0();
   input += synapse0x9013a18();
   input += synapse0x9013a40();
   input += synapse0x9013a68();
   input += synapse0x9013a90();
   input += synapse0x9013ab8();
   return input;
}

double NNfunction::neuron0x90138f0() {
   double input = input0x90138f0();
   return (input * 1)+0;
}

double NNfunction::synapse0x9002038() {
   return (neuron0x9002270()*0.360263);
}

double NNfunction::synapse0x8fa9958() {
   return (neuron0x90108a8()*0.44639);
}

double NNfunction::synapse0x8fa9a88() {
   return (neuron0x90109a8()*0.137045);
}

double NNfunction::synapse0x90113e0() {
   return (neuron0x9010b38()*-0.0452906);
}

double NNfunction::synapse0x9011408() {
   return (neuron0x9010d30()*-0.230365);
}

double NNfunction::synapse0x9011430() {
   return (neuron0x9010f30()*-0.0497044);
}

double NNfunction::synapse0x9011630() {
   return (neuron0x9002270()*0.302979);
}

double NNfunction::synapse0x9011658() {
   return (neuron0x90108a8()*0.409064);
}

double NNfunction::synapse0x9011680() {
   return (neuron0x90109a8()*-0.332913);
}

double NNfunction::synapse0x90116a8() {
   return (neuron0x9010b38()*0.127809);
}

double NNfunction::synapse0x90116d0() {
   return (neuron0x9010d30()*0.31788);
}

double NNfunction::synapse0x90116f8() {
   return (neuron0x9010f30()*0.22083);
}

double NNfunction::synapse0x90118f8() {
   return (neuron0x9002270()*-0.048512);
}

double NNfunction::synapse0x9011920() {
   return (neuron0x90108a8()*-0.0506493);
}

double NNfunction::synapse0x9011948() {
   return (neuron0x90109a8()*-0.129942);
}

double NNfunction::synapse0x9011970() {
   return (neuron0x9010b38()*-0.484978);
}

double NNfunction::synapse0x9011998() {
   return (neuron0x9010d30()*0.35646);
}

double NNfunction::synapse0x9011a48() {
   return (neuron0x9010f30()*-0.317667);
}

double NNfunction::synapse0x9011c20() {
   return (neuron0x9002270()*-0.26207);
}

double NNfunction::synapse0x9011c48() {
   return (neuron0x90108a8()*0.0636826);
}

double NNfunction::synapse0x9011c70() {
   return (neuron0x90109a8()*0.279251);
}

double NNfunction::synapse0x9011c98() {
   return (neuron0x9010b38()*0.489767);
}

double NNfunction::synapse0x9011cc0() {
   return (neuron0x9010d30()*-0.441218);
}

double NNfunction::synapse0x9011ce8() {
   return (neuron0x9010f30()*-0.156516);
}

double NNfunction::synapse0x9011f08() {
   return (neuron0x9002270()*-0.299576);
}

double NNfunction::synapse0x9011f30() {
   return (neuron0x90108a8()*0.308097);
}

double NNfunction::synapse0x9011f58() {
   return (neuron0x90109a8()*0.00535744);
}

double NNfunction::synapse0x9011f80() {
   return (neuron0x9010b38()*0.361075);
}

double NNfunction::synapse0x9011fa8() {
   return (neuron0x9010d30()*0.495057);
}

double NNfunction::synapse0x9011fd0() {
   return (neuron0x9010f30()*0.441791);
}

double NNfunction::synapse0x90121f0() {
   return (neuron0x9002270()*0.344824);
}

double NNfunction::synapse0x9012218() {
   return (neuron0x90108a8()*-0.230669);
}

double NNfunction::synapse0x9012240() {
   return (neuron0x90109a8()*0.5347);
}

double NNfunction::synapse0x90119c0() {
   return (neuron0x9010b38()*0.0955198);
}

double NNfunction::synapse0x90119e8() {
   return (neuron0x9010d30()*-0.26917);
}

double NNfunction::synapse0x9011a10() {
   return (neuron0x9010f30()*-0.11162);
}

double NNfunction::synapse0x9012568() {
   return (neuron0x9002270()*0.0794451);
}

double NNfunction::synapse0x9012590() {
   return (neuron0x90108a8()*-0.0759038);
}

double NNfunction::synapse0x90125b8() {
   return (neuron0x90109a8()*-0.614751);
}

double NNfunction::synapse0x90125e0() {
   return (neuron0x9010b38()*-0.0296527);
}

double NNfunction::synapse0x9012608() {
   return (neuron0x9010d30()*0.0649674);
}

double NNfunction::synapse0x9012630() {
   return (neuron0x9010f30()*0.0152083);
}

double NNfunction::synapse0x9012658() {
   return (neuron0x9011250()*-0.264399);
}

double NNfunction::synapse0x9012878() {
   return (neuron0x9011458()*-0.249006);
}

double NNfunction::synapse0x90128a0() {
   return (neuron0x9011720()*-0.311438);
}

double NNfunction::synapse0x90128c8() {
   return (neuron0x9011a70()*-0.174031);
}

double NNfunction::synapse0x90128f0() {
   return (neuron0x9011d10()*-0.164152);
}

double NNfunction::synapse0x9012918() {
   return (neuron0x9011ff8()*0.290763);
}

double NNfunction::synapse0x9012940() {
   return (neuron0x9012370()*-0.239868);
}

double NNfunction::synapse0x9012b60() {
   return (neuron0x9011250()*-0.244145);
}

double NNfunction::synapse0x9012b88() {
   return (neuron0x9011458()*-0.180781);
}

double NNfunction::synapse0x9012bb0() {
   return (neuron0x9011720()*-0.196073);
}

double NNfunction::synapse0x9012bd8() {
   return (neuron0x9011a70()*0.0097474);
}

double NNfunction::synapse0x9012c00() {
   return (neuron0x9011d10()*0.385194);
}

double NNfunction::synapse0x9012c28() {
   return (neuron0x9011ff8()*-0.524412);
}

double NNfunction::synapse0x9012c50() {
   return (neuron0x9012370()*0.327374);
}

double NNfunction::synapse0x9012e70() {
   return (neuron0x9011250()*-0.294178);
}

double NNfunction::synapse0x9012e98() {
   return (neuron0x9011458()*-0.148496);
}

double NNfunction::synapse0x9012ec0() {
   return (neuron0x9011720()*-0.160787);
}

double NNfunction::synapse0x9012ee8() {
   return (neuron0x9011a70()*-0.436321);
}

double NNfunction::synapse0x9012f10() {
   return (neuron0x9011d10()*0.479877);
}

double NNfunction::synapse0x9012f38() {
   return (neuron0x9011ff8()*-0.609302);
}

double NNfunction::synapse0x9012f60() {
   return (neuron0x9012370()*-0.102164);
}

double NNfunction::synapse0x9013208() {
   return (neuron0x9011250()*-0.404309);
}

double NNfunction::synapse0x9013230() {
   return (neuron0x9011458()*0.283685);
}

double NNfunction::synapse0x8fa9ab0() {
   return (neuron0x9011720()*0.114294);
}

double NNfunction::synapse0x8fa99c0() {
   return (neuron0x9011a70()*-0.151698);
}

double NNfunction::synapse0x8fa1f00() {
   return (neuron0x9011d10()*0.14278);
}

double NNfunction::synapse0x90020d8() {
   return (neuron0x9011ff8()*-0.30735);
}

double NNfunction::synapse0x9002100() {
   return (neuron0x9012370()*0.516484);
}

double NNfunction::synapse0x90021c0() {
   return (neuron0x9011250()*-0.0315547);
}

double NNfunction::synapse0x90134f0() {
   return (neuron0x9011458()*0.243708);
}

double NNfunction::synapse0x9013518() {
   return (neuron0x9011720()*0.402163);
}

double NNfunction::synapse0x9013540() {
   return (neuron0x9011a70()*0.123132);
}

double NNfunction::synapse0x9013568() {
   return (neuron0x9011d10()*-0.123708);
}

double NNfunction::synapse0x9013590() {
   return (neuron0x9011ff8()*-0.682263);
}

double NNfunction::synapse0x90135b8() {
   return (neuron0x9012370()*0.669913);
}

double NNfunction::synapse0x90137d8() {
   return (neuron0x9011250()*-0.222452);
}

double NNfunction::synapse0x9013800() {
   return (neuron0x9011458()*-0.209504);
}

double NNfunction::synapse0x9013828() {
   return (neuron0x9011720()*-0.265455);
}

double NNfunction::synapse0x9013850() {
   return (neuron0x9011a70()*0.0689342);
}

double NNfunction::synapse0x9013878() {
   return (neuron0x9011d10()*0.377918);
}

double NNfunction::synapse0x90138a0() {
   return (neuron0x9011ff8()*0.448819);
}

double NNfunction::synapse0x90138c8() {
   return (neuron0x9012370()*-0.519832);
}

double NNfunction::synapse0x90139f0() {
   return (neuron0x9012680()*0.186642);
}

double NNfunction::synapse0x9013a18() {
   return (neuron0x9012968()*-0.580554);
}

double NNfunction::synapse0x9013a40() {
   return (neuron0x9012c78()*-0.708363);
}

double NNfunction::synapse0x9013a68() {
   return (neuron0x9012f88()*-0.430821);
}

double NNfunction::synapse0x9013a90() {
   return (neuron0x9012268()*-0.743571);
}

double NNfunction::synapse0x9013ab8() {
   return (neuron0x90135e0()*0.560726);
}

