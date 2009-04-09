#include "NNfunction.h"
#include <cmath>

double NNfunction::Value(int index,double in0,double in1,double in2,double in3,double in4,double in5,double in6) {
   input0 = (in0 - 0)/1;
   input1 = (in1 - 0)/1;
   input2 = (in2 - 0)/1;
   input3 = (in3 - 0)/1;
   input4 = (in4 - 0)/1;
   input5 = (in5 - 0)/1;
   input6 = (in6 - 0)/1;
   switch(index) {
     case 0:
         return neuron0x91a9da0();
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
   input6 = (input[6] - 0)/1;
   switch(index) {
     case 0:
         return neuron0x91a9da0();
     default:
         return 0.;
   }
}

double NNfunction::neuron0x91a9038() {
   return input0;
}

double NNfunction::neuron0x91a91e8() {
   return input1;
}

double NNfunction::neuron0x91a93e0() {
   return input2;
}

double NNfunction::neuron0x91a95d8() {
   return input3;
}

double NNfunction::neuron0x91a97d0() {
   return input4;
}

double NNfunction::neuron0x91a99c8() {
   return input5;
}

double NNfunction::neuron0x91a9ba0() {
   return input6;
}

double NNfunction::input0x91a9ec0() {
   double input = 0.349459;
   input += synapse0x91ade40();
   input += synapse0x9132670();
   input += synapse0x91aa050();
   input += synapse0x91aa078();
   input += synapse0x91aa0a0();
   input += synapse0x91aa0c8();
   input += synapse0x91aa0f0();
   return input;
}

double NNfunction::neuron0x91a9ec0() {
   double input = input0x91a9ec0();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0x91aa118() {
   double input = 0.10585;
   input += synapse0x91aa2f0();
   input += synapse0x91aa318();
   input += synapse0x91aa340();
   input += synapse0x91aa368();
   input += synapse0x91aa390();
   input += synapse0x91aa3b8();
   input += synapse0x91aa3e0();
   return input;
}

double NNfunction::neuron0x91aa118() {
   double input = input0x91aa118();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0x91aa408() {
   double input = 0.18002;
   input += synapse0x91aa5e0();
   input += synapse0x91aa608();
   input += synapse0x91aa630();
   input += synapse0x91aa6e0();
   input += synapse0x91aa708();
   input += synapse0x91aa730();
   input += synapse0x91aa758();
   return input;
}

double NNfunction::neuron0x91aa408() {
   double input = input0x91aa408();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0x91aa780() {
   double input = 0.0532833;
   input += synapse0x91aa910();
   input += synapse0x91aa938();
   input += synapse0x91aa960();
   input += synapse0x91aa988();
   input += synapse0x91aa9b0();
   input += synapse0x91aa9d8();
   input += synapse0x91aaa00();
   return input;
}

double NNfunction::neuron0x91aa780() {
   double input = input0x91aa780();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0x91aaa28() {
   double input = 0.051002;
   input += synapse0x91aac00();
   input += synapse0x91aac28();
   input += synapse0x91aac50();
   input += synapse0x91aac78();
   input += synapse0x91aaca0();
   input += synapse0x91aa658();
   input += synapse0x91aa680();
   return input;
}

double NNfunction::neuron0x91aaa28() {
   double input = input0x91aaa28();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0x91aadd0() {
   double input = 0.0991517;
   input += synapse0x91aafc8();
   input += synapse0x91aaff0();
   input += synapse0x91ab018();
   input += synapse0x91ab040();
   input += synapse0x91ab068();
   input += synapse0x91ab090();
   input += synapse0x91ab0b8();
   return input;
}

double NNfunction::neuron0x91aadd0() {
   double input = input0x91aadd0();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0x91ab0e0() {
   double input = 0.118278;
   input += synapse0x91ab2d8();
   input += synapse0x91ab300();
   input += synapse0x91ab328();
   input += synapse0x91ab350();
   input += synapse0x91ab378();
   input += synapse0x91ab3a0();
   input += synapse0x91ab3c8();
   return input;
}

double NNfunction::neuron0x91ab0e0() {
   double input = input0x91ab0e0();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0x91ab3f0() {
   double input = 0.155996;
   input += synapse0x91ab5e8();
   input += synapse0x91ab610();
   input += synapse0x91ab638();
   input += synapse0x91ab660();
   input += synapse0x91ab688();
   input += synapse0x91ab6b0();
   input += synapse0x91ab6d8();
   return input;
}

double NNfunction::neuron0x91ab3f0() {
   double input = input0x91ab3f0();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0x91ab700() {
   double input = 0.0163352;
   input += synapse0x91ab8f8();
   input += synapse0x91ab920();
   input += synapse0x91ab948();
   input += synapse0x91ab970();
   input += synapse0x91ab998();
   input += synapse0x91ab9c0();
   input += synapse0x91ab9e8();
   return input;
}

double NNfunction::neuron0x91ab700() {
   double input = input0x91ab700();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0x91aba38() {
   double input = 0.15071;
   input += synapse0x91aba10();
   input += synapse0x91abcb8();
   input += synapse0x9132760();
   input += synapse0x91326e8();
   input += synapse0x91aa6a8();
   input += synapse0x912ac08();
   input += synapse0x918ada8();
   input += synapse0x918add0();
   input += synapse0x9132568();
   return input;
}

double NNfunction::neuron0x91aba38() {
   double input = input0x91aba38();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0x91aacc8() {
   double input = 0.273082;
   input += synapse0x91abf30();
   input += synapse0x91abf58();
   input += synapse0x91abf80();
   input += synapse0x91abfa8();
   input += synapse0x91abfd0();
   input += synapse0x91abff8();
   input += synapse0x91ac020();
   input += synapse0x91ac048();
   input += synapse0x91ac070();
   return input;
}

double NNfunction::neuron0x91aacc8() {
   double input = input0x91aacc8();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0x91ac098() {
   double input = 0.39978;
   input += synapse0x91ac290();
   input += synapse0x91ac2b8();
   input += synapse0x91ac2e0();
   input += synapse0x91ac308();
   input += synapse0x91ac330();
   input += synapse0x91ac358();
   input += synapse0x91ac380();
   input += synapse0x91ac3a8();
   input += synapse0x91ac3d0();
   return input;
}

double NNfunction::neuron0x91ac098() {
   double input = input0x91ac098();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0x91ac3f8() {
   double input = 0.107349;
   input += synapse0x91ac5f0();
   input += synapse0x91ac618();
   input += synapse0x91ac640();
   input += synapse0x91ac668();
   input += synapse0x91ac690();
   input += synapse0x91ac6b8();
   input += synapse0x91ac6e0();
   input += synapse0x91ac708();
   input += synapse0x91ac730();
   return input;
}

double NNfunction::neuron0x91ac3f8() {
   double input = input0x91ac3f8();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0x91ac758() {
   double input = -0.408188;
   input += synapse0x91ac950();
   input += synapse0x91ac978();
   input += synapse0x91ac9a0();
   input += synapse0x91ac9c8();
   input += synapse0x91ac9f0();
   input += synapse0x91aca18();
   input += synapse0x91aca40();
   input += synapse0x91aca68();
   input += synapse0x91aca90();
   return input;
}

double NNfunction::neuron0x91ac758() {
   double input = input0x91ac758();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0x91acab8() {
   double input = -0.388;
   input += synapse0x91accb0();
   input += synapse0x91accd8();
   input += synapse0x91acd00();
   input += synapse0x91acd28();
   input += synapse0x91acd50();
   input += synapse0x91acd78();
   input += synapse0x9132620();
   input += synapse0x91acda0();
   input += synapse0x91adee0();
   return input;
}

double NNfunction::neuron0x91acab8() {
   double input = input0x91acab8();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0x91adf08() {
   double input = 0.0448803;
   input += synapse0x91ae100();
   input += synapse0x91ae128();
   input += synapse0x91ae150();
   input += synapse0x91ae178();
   input += synapse0x91ae1a0();
   input += synapse0x91ae1c8();
   input += synapse0x91ae1f0();
   input += synapse0x91ae218();
   input += synapse0x91ae240();
   return input;
}

double NNfunction::neuron0x91adf08() {
   double input = input0x91adf08();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0x91ae268() {
   double input = 0.320419;
   input += synapse0x91ae460();
   input += synapse0x91ae488();
   input += synapse0x91ae4b0();
   input += synapse0x91abce0();
   input += synapse0x91abd08();
   input += synapse0x91abd30();
   input += synapse0x91abd58();
   input += synapse0x91abd80();
   input += synapse0x91abda8();
   return input;
}

double NNfunction::neuron0x91ae268() {
   double input = input0x91ae268();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0x91a9da0() {
   double input = 0.0125571;
   input += synapse0x91ae8e0();
   input += synapse0x91ae908();
   input += synapse0x91ae930();
   input += synapse0x91ae958();
   input += synapse0x91ae980();
   input += synapse0x91ae9a8();
   input += synapse0x91ae9d0();
   input += synapse0x91ae9f8();
   return input;
}

double NNfunction::neuron0x91a9da0() {
   double input = input0x91a9da0();
   return (input * 1)+0;
}

double NNfunction::synapse0x91ade40() {
   return (neuron0x91a9038()*0.177729);
}

double NNfunction::synapse0x9132670() {
   return (neuron0x91a91e8()*0.380971);
}

double NNfunction::synapse0x91aa050() {
   return (neuron0x91a93e0()*-0.153628);
}

double NNfunction::synapse0x91aa078() {
   return (neuron0x91a95d8()*-0.376992);
}

double NNfunction::synapse0x91aa0a0() {
   return (neuron0x91a97d0()*0.30821);
}

double NNfunction::synapse0x91aa0c8() {
   return (neuron0x91a99c8()*-0.0724336);
}

double NNfunction::synapse0x91aa0f0() {
   return (neuron0x91a9ba0()*0.343043);
}

double NNfunction::synapse0x91aa2f0() {
   return (neuron0x91a9038()*-0.275966);
}

double NNfunction::synapse0x91aa318() {
   return (neuron0x91a91e8()*-0.480627);
}

double NNfunction::synapse0x91aa340() {
   return (neuron0x91a93e0()*0.156723);
}

double NNfunction::synapse0x91aa368() {
   return (neuron0x91a95d8()*0.193732);
}

double NNfunction::synapse0x91aa390() {
   return (neuron0x91a97d0()*0.496689);
}

double NNfunction::synapse0x91aa3b8() {
   return (neuron0x91a99c8()*-0.384845);
}

double NNfunction::synapse0x91aa3e0() {
   return (neuron0x91a9ba0()*-0.177538);
}

double NNfunction::synapse0x91aa5e0() {
   return (neuron0x91a9038()*-0.0819731);
}

double NNfunction::synapse0x91aa608() {
   return (neuron0x91a91e8()*-0.00237839);
}

double NNfunction::synapse0x91aa630() {
   return (neuron0x91a93e0()*-0.00925701);
}

double NNfunction::synapse0x91aa6e0() {
   return (neuron0x91a95d8()*-0.374341);
}

double NNfunction::synapse0x91aa708() {
   return (neuron0x91a97d0()*0.189596);
}

double NNfunction::synapse0x91aa730() {
   return (neuron0x91a99c8()*0.458358);
}

double NNfunction::synapse0x91aa758() {
   return (neuron0x91a9ba0()*-0.00489201);
}

double NNfunction::synapse0x91aa910() {
   return (neuron0x91a9038()*-0.270405);
}

double NNfunction::synapse0x91aa938() {
   return (neuron0x91a91e8()*0.553716);
}

double NNfunction::synapse0x91aa960() {
   return (neuron0x91a93e0()*-0.517452);
}

double NNfunction::synapse0x91aa988() {
   return (neuron0x91a95d8()*-0.317153);
}

double NNfunction::synapse0x91aa9b0() {
   return (neuron0x91a97d0()*0.311318);
}

double NNfunction::synapse0x91aa9d8() {
   return (neuron0x91a99c8()*-0.0503575);
}

double NNfunction::synapse0x91aaa00() {
   return (neuron0x91a9ba0()*-0.25713);
}

double NNfunction::synapse0x91aac00() {
   return (neuron0x91a9038()*0.0739352);
}

double NNfunction::synapse0x91aac28() {
   return (neuron0x91a91e8()*0.490426);
}

double NNfunction::synapse0x91aac50() {
   return (neuron0x91a93e0()*0.00869782);
}

double NNfunction::synapse0x91aac78() {
   return (neuron0x91a95d8()*-0.0080643);
}

double NNfunction::synapse0x91aaca0() {
   return (neuron0x91a97d0()*-0.462458);
}

double NNfunction::synapse0x91aa658() {
   return (neuron0x91a99c8()*0.116618);
}

double NNfunction::synapse0x91aa680() {
   return (neuron0x91a9ba0()*0.0602938);
}

double NNfunction::synapse0x91aafc8() {
   return (neuron0x91a9038()*0.344831);
}

double NNfunction::synapse0x91aaff0() {
   return (neuron0x91a91e8()*-0.452205);
}

double NNfunction::synapse0x91ab018() {
   return (neuron0x91a93e0()*-0.337894);
}

double NNfunction::synapse0x91ab040() {
   return (neuron0x91a95d8()*-0.427133);
}

double NNfunction::synapse0x91ab068() {
   return (neuron0x91a97d0()*0.108102);
}

double NNfunction::synapse0x91ab090() {
   return (neuron0x91a99c8()*0.216578);
}

double NNfunction::synapse0x91ab0b8() {
   return (neuron0x91a9ba0()*-0.113716);
}

double NNfunction::synapse0x91ab2d8() {
   return (neuron0x91a9038()*-0.249266);
}

double NNfunction::synapse0x91ab300() {
   return (neuron0x91a91e8()*0.0646854);
}

double NNfunction::synapse0x91ab328() {
   return (neuron0x91a93e0()*-0.0980465);
}

double NNfunction::synapse0x91ab350() {
   return (neuron0x91a95d8()*-0.372506);
}

double NNfunction::synapse0x91ab378() {
   return (neuron0x91a97d0()*0.155252);
}

double NNfunction::synapse0x91ab3a0() {
   return (neuron0x91a99c8()*-0.154921);
}

double NNfunction::synapse0x91ab3c8() {
   return (neuron0x91a9ba0()*0.415007);
}

double NNfunction::synapse0x91ab5e8() {
   return (neuron0x91a9038()*0.390005);
}

double NNfunction::synapse0x91ab610() {
   return (neuron0x91a91e8()*-0.0167718);
}

double NNfunction::synapse0x91ab638() {
   return (neuron0x91a93e0()*-0.484692);
}

double NNfunction::synapse0x91ab660() {
   return (neuron0x91a95d8()*-0.47339);
}

double NNfunction::synapse0x91ab688() {
   return (neuron0x91a97d0()*-0.113485);
}

double NNfunction::synapse0x91ab6b0() {
   return (neuron0x91a99c8()*-0.452137);
}

double NNfunction::synapse0x91ab6d8() {
   return (neuron0x91a9ba0()*-0.110558);
}

double NNfunction::synapse0x91ab8f8() {
   return (neuron0x91a9038()*0.181916);
}

double NNfunction::synapse0x91ab920() {
   return (neuron0x91a91e8()*-0.240361);
}

double NNfunction::synapse0x91ab948() {
   return (neuron0x91a93e0()*0.325586);
}

double NNfunction::synapse0x91ab970() {
   return (neuron0x91a95d8()*0.147272);
}

double NNfunction::synapse0x91ab998() {
   return (neuron0x91a97d0()*0.159673);
}

double NNfunction::synapse0x91ab9c0() {
   return (neuron0x91a99c8()*0.176559);
}

double NNfunction::synapse0x91ab9e8() {
   return (neuron0x91a9ba0()*0.229002);
}

double NNfunction::synapse0x91aba10() {
   return (neuron0x91a9ec0()*-0.383991);
}

double NNfunction::synapse0x91abcb8() {
   return (neuron0x91aa118()*-0.333084);
}

double NNfunction::synapse0x9132760() {
   return (neuron0x91aa408()*0.0503488);
}

double NNfunction::synapse0x91326e8() {
   return (neuron0x91aa780()*0.201993);
}

double NNfunction::synapse0x91aa6a8() {
   return (neuron0x91aaa28()*-0.303484);
}

double NNfunction::synapse0x912ac08() {
   return (neuron0x91aadd0()*0.219565);
}

double NNfunction::synapse0x918ada8() {
   return (neuron0x91ab0e0()*-0.292833);
}

double NNfunction::synapse0x918add0() {
   return (neuron0x91ab3f0()*-0.31765);
}

double NNfunction::synapse0x9132568() {
   return (neuron0x91ab700()*-0.111717);
}

double NNfunction::synapse0x91abf30() {
   return (neuron0x91a9ec0()*-0.220361);
}

double NNfunction::synapse0x91abf58() {
   return (neuron0x91aa118()*0.0705727);
}

double NNfunction::synapse0x91abf80() {
   return (neuron0x91aa408()*-0.58008);
}

double NNfunction::synapse0x91abfa8() {
   return (neuron0x91aa780()*0.750894);
}

double NNfunction::synapse0x91abfd0() {
   return (neuron0x91aaa28()*-0.34753);
}

double NNfunction::synapse0x91abff8() {
   return (neuron0x91aadd0()*0.204554);
}

double NNfunction::synapse0x91ac020() {
   return (neuron0x91ab0e0()*0.0899098);
}

double NNfunction::synapse0x91ac048() {
   return (neuron0x91ab3f0()*0.375247);
}

double NNfunction::synapse0x91ac070() {
   return (neuron0x91ab700()*-0.23898);
}

double NNfunction::synapse0x91ac290() {
   return (neuron0x91a9ec0()*-0.00187297);
}

double NNfunction::synapse0x91ac2b8() {
   return (neuron0x91aa118()*0.121772);
}

double NNfunction::synapse0x91ac2e0() {
   return (neuron0x91aa408()*0.375419);
}

double NNfunction::synapse0x91ac308() {
   return (neuron0x91aa780()*0.258922);
}

double NNfunction::synapse0x91ac330() {
   return (neuron0x91aaa28()*0.126157);
}

double NNfunction::synapse0x91ac358() {
   return (neuron0x91aadd0()*-0.462214);
}

double NNfunction::synapse0x91ac380() {
   return (neuron0x91ab0e0()*-0.282559);
}

double NNfunction::synapse0x91ac3a8() {
   return (neuron0x91ab3f0()*-0.149928);
}

double NNfunction::synapse0x91ac3d0() {
   return (neuron0x91ab700()*0.23387);
}

double NNfunction::synapse0x91ac5f0() {
   return (neuron0x91a9ec0()*-0.345137);
}

double NNfunction::synapse0x91ac618() {
   return (neuron0x91aa118()*-0.418586);
}

double NNfunction::synapse0x91ac640() {
   return (neuron0x91aa408()*0.506194);
}

double NNfunction::synapse0x91ac668() {
   return (neuron0x91aa780()*-0.522722);
}

double NNfunction::synapse0x91ac690() {
   return (neuron0x91aaa28()*-0.112819);
}

double NNfunction::synapse0x91ac6b8() {
   return (neuron0x91aadd0()*-0.158136);
}

double NNfunction::synapse0x91ac6e0() {
   return (neuron0x91ab0e0()*-0.0285252);
}

double NNfunction::synapse0x91ac708() {
   return (neuron0x91ab3f0()*-0.442296);
}

double NNfunction::synapse0x91ac730() {
   return (neuron0x91ab700()*0.238925);
}

double NNfunction::synapse0x91ac950() {
   return (neuron0x91a9ec0()*0.0507313);
}

double NNfunction::synapse0x91ac978() {
   return (neuron0x91aa118()*-0.477618);
}

double NNfunction::synapse0x91ac9a0() {
   return (neuron0x91aa408()*-0.741174);
}

double NNfunction::synapse0x91ac9c8() {
   return (neuron0x91aa780()*0.524428);
}

double NNfunction::synapse0x91ac9f0() {
   return (neuron0x91aaa28()*-0.211224);
}

double NNfunction::synapse0x91aca18() {
   return (neuron0x91aadd0()*0.35332);
}

double NNfunction::synapse0x91aca40() {
   return (neuron0x91ab0e0()*0.313199);
}

double NNfunction::synapse0x91aca68() {
   return (neuron0x91ab3f0()*-0.120069);
}

double NNfunction::synapse0x91aca90() {
   return (neuron0x91ab700()*-0.525786);
}

double NNfunction::synapse0x91accb0() {
   return (neuron0x91a9ec0()*0.115942);
}

double NNfunction::synapse0x91accd8() {
   return (neuron0x91aa118()*0.0738978);
}

double NNfunction::synapse0x91acd00() {
   return (neuron0x91aa408()*-0.756552);
}

double NNfunction::synapse0x91acd28() {
   return (neuron0x91aa780()*0.253393);
}

double NNfunction::synapse0x91acd50() {
   return (neuron0x91aaa28()*-0.31841);
}

double NNfunction::synapse0x91acd78() {
   return (neuron0x91aadd0()*-0.363067);
}

double NNfunction::synapse0x9132620() {
   return (neuron0x91ab0e0()*0.253526);
}

double NNfunction::synapse0x91acda0() {
   return (neuron0x91ab3f0()*-0.461412);
}

double NNfunction::synapse0x91adee0() {
   return (neuron0x91ab700()*0.047193);
}

double NNfunction::synapse0x91ae100() {
   return (neuron0x91a9ec0()*0.294279);
}

double NNfunction::synapse0x91ae128() {
   return (neuron0x91aa118()*0.0979147);
}

double NNfunction::synapse0x91ae150() {
   return (neuron0x91aa408()*0.752229);
}

double NNfunction::synapse0x91ae178() {
   return (neuron0x91aa780()*-0.696816);
}

double NNfunction::synapse0x91ae1a0() {
   return (neuron0x91aaa28()*-0.0301832);
}

double NNfunction::synapse0x91ae1c8() {
   return (neuron0x91aadd0()*0.127141);
}

double NNfunction::synapse0x91ae1f0() {
   return (neuron0x91ab0e0()*-0.072834);
}

double NNfunction::synapse0x91ae218() {
   return (neuron0x91ab3f0()*-0.246954);
}

double NNfunction::synapse0x91ae240() {
   return (neuron0x91ab700()*0.616077);
}

double NNfunction::synapse0x91ae460() {
   return (neuron0x91a9ec0()*-0.24172);
}

double NNfunction::synapse0x91ae488() {
   return (neuron0x91aa118()*0.0161003);
}

double NNfunction::synapse0x91ae4b0() {
   return (neuron0x91aa408()*-0.337658);
}

double NNfunction::synapse0x91abce0() {
   return (neuron0x91aa780()*0.193418);
}

double NNfunction::synapse0x91abd08() {
   return (neuron0x91aaa28()*-0.351608);
}

double NNfunction::synapse0x91abd30() {
   return (neuron0x91aadd0()*-0.20939);
}

double NNfunction::synapse0x91abd58() {
   return (neuron0x91ab0e0()*0.0135734);
}

double NNfunction::synapse0x91abd80() {
   return (neuron0x91ab3f0()*-0.131208);
}

double NNfunction::synapse0x91abda8() {
   return (neuron0x91ab700()*-0.533359);
}

double NNfunction::synapse0x91ae8e0() {
   return (neuron0x91aba38()*0.093974);
}

double NNfunction::synapse0x91ae908() {
   return (neuron0x91aacc8()*0.887764);
}

double NNfunction::synapse0x91ae930() {
   return (neuron0x91ac098()*-0.206286);
}

double NNfunction::synapse0x91ae958() {
   return (neuron0x91ac3f8()*-0.787701);
}

double NNfunction::synapse0x91ae980() {
   return (neuron0x91ac758()*0.886335);
}

double NNfunction::synapse0x91ae9a8() {
   return (neuron0x91acab8()*0.652539);
}

double NNfunction::synapse0x91ae9d0() {
   return (neuron0x91adf08()*-1.27774);
}

double NNfunction::synapse0x91ae9f8() {
   return (neuron0x91ae268()*0.424169);
}

