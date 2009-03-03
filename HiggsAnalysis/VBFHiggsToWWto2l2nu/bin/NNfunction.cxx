#include "NNfunction.h"
#include <cmath>

double NNfunction::Value(int index,double in0,double in1,double in2,double in3) {
   input0 = (in0 - 0)/1;
   input1 = (in1 - 0)/1;
   input2 = (in2 - 0)/1;
   input3 = (in3 - 0)/1;
   switch(index) {
     case 0:
         return neuron0xa440b48();
     default:
         return 0.;
   }
}

double NNfunction::Value(int index, double* input) {
   input0 = (input[0] - 0)/1;
   input1 = (input[1] - 0)/1;
   input2 = (input[2] - 0)/1;
   input3 = (input[3] - 0)/1;
   switch(index) {
     case 0:
         return neuron0xa440b48();
     default:
         return 0.;
   }
}

double NNfunction::neuron0xa42ec98() {
   return input0;
}

double NNfunction::neuron0xa42ee70() {
   return input1;
}

double NNfunction::neuron0xa43d340() {
   return input2;
}

double NNfunction::neuron0xa43d538() {
   return input3;
}

double NNfunction::input0xa43d850() {
   double input = -0.488702;
   input += synapse0xa42ebc0();
   input += synapse0xa43d9e0();
   input += synapse0xa43da08();
   input += synapse0xa43da30();
   return input;
}

double NNfunction::neuron0xa43d850() {
   double input = input0xa43d850();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0xa43da58() {
   double input = 0.0892427;
   input += synapse0xa43dc30();
   input += synapse0xa43dc58();
   input += synapse0xa43dc80();
   input += synapse0xa43dca8();
   return input;
}

double NNfunction::neuron0xa43da58() {
   double input = input0xa43da58();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0xa43dcd0() {
   double input = -0.231811;
   input += synapse0xa43dea8();
   input += synapse0xa43ded0();
   input += synapse0xa43def8();
   input += synapse0xa43df20();
   return input;
}

double NNfunction::neuron0xa43dcd0() {
   double input = input0xa43dcd0();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0xa43df48() {
   double input = 0.647556;
   input += synapse0xa43e120();
   input += synapse0xa43e148();
   input += synapse0xa43e170();
   input += synapse0xa43e198();
   return input;
}

double NNfunction::neuron0xa43df48() {
   double input = input0xa43df48();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0xa43e1c0() {
   double input = 0.0597326;
   input += synapse0xa43e3b8();
   input += synapse0xa43e468();
   input += synapse0xa43e490();
   input += synapse0xa43e4b8();
   return input;
}

double NNfunction::neuron0xa43e1c0() {
   double input = input0xa43e1c0();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0xa43e4e0() {
   double input = 0.151423;
   input += synapse0xa43e690();
   input += synapse0xa43e6b8();
   input += synapse0xa43e6e0();
   input += synapse0xa43e708();
   return input;
}

double NNfunction::neuron0xa43e4e0() {
   double input = input0xa43e4e0();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0xa43e730() {
   double input = -0.06658;
   input += synapse0xa43e928();
   input += synapse0xa43e950();
   input += synapse0xa43e978();
   input += synapse0xa43e9a0();
   return input;
}

double NNfunction::neuron0xa43e730() {
   double input = input0xa43e730();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0xa43e9c8() {
   double input = -0.0487014;
   input += synapse0xa43ebc0();
   input += synapse0xa43ebe8();
   input += synapse0xa43ec10();
   input += synapse0xa43ec38();
   return input;
}

double NNfunction::neuron0xa43e9c8() {
   double input = input0xa43e9c8();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0xa43ec60() {
   double input = -0.587676;
   input += synapse0xa43ee58();
   input += synapse0xa43e3e0();
   input += synapse0xa43e408();
   input += synapse0xa43e430();
   return input;
}

double NNfunction::neuron0xa43ec60() {
   double input = input0xa43ec60();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0xa43ef88() {
   double input = 0.351373;
   input += synapse0xa43f180();
   input += synapse0xa43f1a8();
   input += synapse0xa43f1d0();
   input += synapse0xa43f1f8();
   return input;
}

double NNfunction::neuron0xa43ef88() {
   double input = input0xa43ef88();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0xa43f220() {
   double input = -0.329278;
   input += synapse0xa43f420();
   input += synapse0xa43f448();
   input += synapse0xa43f470();
   input += synapse0xa43f498();
   return input;
}

double NNfunction::neuron0xa43f220() {
   double input = input0xa43f220();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0xa43f4e8() {
   double input = 0.444597;
   input += synapse0xa43f4c0();
   input += synapse0xa43f6e0();
   input += synapse0xa43f708();
   input += synapse0xa43f730();
   input += synapse0xa43f758();
   input += synapse0xa43f780();
   input += synapse0xa43f7a8();
   input += synapse0xa43f7d0();
   input += synapse0xa43f7f8();
   input += synapse0xa43f820();
   input += synapse0xa43f848();
   return input;
}

double NNfunction::neuron0xa43f4e8() {
   double input = input0xa43f4e8();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0xa43f870() {
   double input = 0.297442;
   input += synapse0xa43faf0();
   input += synapse0xa43fb18();
   input += synapse0xa43fb40();
   input += synapse0xa43fb68();
   input += synapse0xa43fb90();
   input += synapse0xa43fbb8();
   input += synapse0xa43fbe0();
   input += synapse0xa43fc08();
   input += synapse0xa43fc30();
   input += synapse0xa43fc58();
   input += synapse0xa3f82e8();
   return input;
}

double NNfunction::neuron0xa43f870() {
   double input = input0xa43f870();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0xa43ee80() {
   double input = 0.060716;
   input += synapse0xa40bfe8();
   input += synapse0xa42eb60();
   input += synapse0xa43fed0();
   input += synapse0xa43fef8();
   input += synapse0xa43ff20();
   input += synapse0xa43ff48();
   input += synapse0xa43ff70();
   input += synapse0xa43ff98();
   input += synapse0xa43ffc0();
   input += synapse0xa43ffe8();
   input += synapse0xa440010();
   return input;
}

double NNfunction::neuron0xa43ee80() {
   double input = input0xa43ee80();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0xa440038() {
   double input = -0.0488854;
   input += synapse0xa440230();
   input += synapse0xa440258();
   input += synapse0xa440280();
   input += synapse0xa4402a8();
   input += synapse0xa4402d0();
   input += synapse0xa4402f8();
   input += synapse0xa440320();
   input += synapse0xa440348();
   input += synapse0xa440370();
   input += synapse0xa440398();
   input += synapse0xa4403c0();
   return input;
}

double NNfunction::neuron0xa440038() {
   double input = input0xa440038();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0xa4403e8() {
   double input = -0.0961789;
   input += synapse0xa4405e0();
   input += synapse0xa440608();
   input += synapse0xa440630();
   input += synapse0xa440658();
   input += synapse0xa440680();
   input += synapse0xa4406a8();
   input += synapse0xa4406d0();
   input += synapse0xa4406f8();
   input += synapse0xa440720();
   input += synapse0xa440748();
   input += synapse0xa440770();
   return input;
}

double NNfunction::neuron0xa4403e8() {
   double input = input0xa4403e8();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0xa440798() {
   double input = -0.307188;
   input += synapse0xa440990();
   input += synapse0xa4409b8();
   input += synapse0xa4409e0();
   input += synapse0xa440a08();
   input += synapse0xa440a30();
   input += synapse0xa440a58();
   input += synapse0xa440a80();
   input += synapse0xa440aa8();
   input += synapse0xa440ad0();
   input += synapse0xa440af8();
   input += synapse0xa440b20();
   return input;
}

double NNfunction::neuron0xa440798() {
   double input = input0xa440798();
   return ((input < -709. ? 0. : (1/(1+exp(-input)))) * 1)+0;
}

double NNfunction::input0xa440b48() {
   double input = 0.099465;
   input += synapse0xa440c48();
   input += synapse0xa440c70();
   input += synapse0xa440c98();
   input += synapse0xa440cc0();
   input += synapse0xa440ce8();
   input += synapse0xa440d10();
   return input;
}

double NNfunction::neuron0xa440b48() {
   double input = input0xa440b48();
   return (input * 1)+0;
}

double NNfunction::synapse0xa42ebc0() {
   return (neuron0xa42ec98()*-0.434275);
}

double NNfunction::synapse0xa43d9e0() {
   return (neuron0xa42ee70()*0.259272);
}

double NNfunction::synapse0xa43da08() {
   return (neuron0xa43d340()*0.00612809);
}

double NNfunction::synapse0xa43da30() {
   return (neuron0xa43d538()*-0.41282);
}

double NNfunction::synapse0xa43dc30() {
   return (neuron0xa42ec98()*0.158752);
}

double NNfunction::synapse0xa43dc58() {
   return (neuron0xa42ee70()*0.0738154);
}

double NNfunction::synapse0xa43dc80() {
   return (neuron0xa43d340()*0.438367);
}

double NNfunction::synapse0xa43dca8() {
   return (neuron0xa43d538()*-0.0377755);
}

double NNfunction::synapse0xa43dea8() {
   return (neuron0xa42ec98()*-0.129479);
}

double NNfunction::synapse0xa43ded0() {
   return (neuron0xa42ee70()*-0.144516);
}

double NNfunction::synapse0xa43def8() {
   return (neuron0xa43d340()*0.28422);
}

double NNfunction::synapse0xa43df20() {
   return (neuron0xa43d538()*-0.20895);
}

double NNfunction::synapse0xa43e120() {
   return (neuron0xa42ec98()*0.000165728);
}

double NNfunction::synapse0xa43e148() {
   return (neuron0xa42ee70()*0.0003598);
}

double NNfunction::synapse0xa43e170() {
   return (neuron0xa43d340()*-0.000502021);
}

double NNfunction::synapse0xa43e198() {
   return (neuron0xa43d538()*-0.00866038);
}

double NNfunction::synapse0xa43e3b8() {
   return (neuron0xa42ec98()*0.12719);
}

double NNfunction::synapse0xa43e468() {
   return (neuron0xa42ee70()*0.232301);
}

double NNfunction::synapse0xa43e490() {
   return (neuron0xa43d340()*0.442538);
}

double NNfunction::synapse0xa43e4b8() {
   return (neuron0xa43d538()*0.410291);
}

double NNfunction::synapse0xa43e690() {
   return (neuron0xa42ec98()*-0.0070176);
}

double NNfunction::synapse0xa43e6b8() {
   return (neuron0xa42ee70()*0.0920279);
}

double NNfunction::synapse0xa43e6e0() {
   return (neuron0xa43d340()*-0.0360039);
}

double NNfunction::synapse0xa43e708() {
   return (neuron0xa43d538()*0.0422989);
}

double NNfunction::synapse0xa43e928() {
   return (neuron0xa42ec98()*-0.112328);
}

double NNfunction::synapse0xa43e950() {
   return (neuron0xa42ee70()*-0.184101);
}

double NNfunction::synapse0xa43e978() {
   return (neuron0xa43d340()*0.400595);
}

double NNfunction::synapse0xa43e9a0() {
   return (neuron0xa43d538()*0.365747);
}

double NNfunction::synapse0xa43ebc0() {
   return (neuron0xa42ec98()*0.00215488);
}

double NNfunction::synapse0xa43ebe8() {
   return (neuron0xa42ee70()*-0.00264435);
}

double NNfunction::synapse0xa43ec10() {
   return (neuron0xa43d340()*0.324586);
}

double NNfunction::synapse0xa43ec38() {
   return (neuron0xa43d538()*0.326119);
}

double NNfunction::synapse0xa43ee58() {
   return (neuron0xa42ec98()*-0.000258177);
}

double NNfunction::synapse0xa43e3e0() {
   return (neuron0xa42ee70()*-0.000387611);
}

double NNfunction::synapse0xa43e408() {
   return (neuron0xa43d340()*0.000483278);
}

double NNfunction::synapse0xa43e430() {
   return (neuron0xa43d538()*0.00851609);
}

double NNfunction::synapse0xa43f180() {
   return (neuron0xa42ec98()*-0.412066);
}

double NNfunction::synapse0xa43f1a8() {
   return (neuron0xa42ee70()*-0.413374);
}

double NNfunction::synapse0xa43f1d0() {
   return (neuron0xa43d340()*-0.303536);
}

double NNfunction::synapse0xa43f1f8() {
   return (neuron0xa43d538()*-0.574596);
}

double NNfunction::synapse0xa43f420() {
   return (neuron0xa42ec98()*-0.450869);
}

double NNfunction::synapse0xa43f448() {
   return (neuron0xa42ee70()*-0.320783);
}

double NNfunction::synapse0xa43f470() {
   return (neuron0xa43d340()*0.428017);
}

double NNfunction::synapse0xa43f498() {
   return (neuron0xa43d538()*-0.327166);
}

double NNfunction::synapse0xa43f4c0() {
   return (neuron0xa43d850()*-0.439056);
}

double NNfunction::synapse0xa43f6e0() {
   return (neuron0xa43da58()*-0.0823316);
}

double NNfunction::synapse0xa43f708() {
   return (neuron0xa43dcd0()*-0.012784);
}

double NNfunction::synapse0xa43f730() {
   return (neuron0xa43df48()*0.223801);
}

double NNfunction::synapse0xa43f758() {
   return (neuron0xa43e1c0()*-0.50558);
}

double NNfunction::synapse0xa43f780() {
   return (neuron0xa43e4e0()*0.0404462);
}

double NNfunction::synapse0xa43f7a8() {
   return (neuron0xa43e730()*-0.0972812);
}

double NNfunction::synapse0xa43f7d0() {
   return (neuron0xa43e9c8()*0.214342);
}

double NNfunction::synapse0xa43f7f8() {
   return (neuron0xa43ec60()*-0.104788);
}

double NNfunction::synapse0xa43f820() {
   return (neuron0xa43ef88()*0.106235);
}

double NNfunction::synapse0xa43f848() {
   return (neuron0xa43f220()*0.244468);
}

double NNfunction::synapse0xa43faf0() {
   return (neuron0xa43d850()*-0.102315);
}

double NNfunction::synapse0xa43fb18() {
   return (neuron0xa43da58()*0.211177);
}

double NNfunction::synapse0xa43fb40() {
   return (neuron0xa43dcd0()*0.14407);
}

double NNfunction::synapse0xa43fb68() {
   return (neuron0xa43df48()*1.55442);
}

double NNfunction::synapse0xa43fb90() {
   return (neuron0xa43e1c0()*-0.313222);
}

double NNfunction::synapse0xa43fbb8() {
   return (neuron0xa43e4e0()*0.339402);
}

double NNfunction::synapse0xa43fbe0() {
   return (neuron0xa43e730()*-0.09391);
}

double NNfunction::synapse0xa43fc08() {
   return (neuron0xa43e9c8()*-0.601086);
}

double NNfunction::synapse0xa43fc30() {
   return (neuron0xa43ec60()*-1.05974);
}

double NNfunction::synapse0xa43fc58() {
   return (neuron0xa43ef88()*0.335509);
}

double NNfunction::synapse0xa3f82e8() {
   return (neuron0xa43f220()*0.402334);
}

double NNfunction::synapse0xa40bfe8() {
   return (neuron0xa43d850()*-0.303568);
}

double NNfunction::synapse0xa42eb60() {
   return (neuron0xa43da58()*0.402533);
}

double NNfunction::synapse0xa43fed0() {
   return (neuron0xa43dcd0()*-0.015878);
}

double NNfunction::synapse0xa43fef8() {
   return (neuron0xa43df48()*0.799869);
}

double NNfunction::synapse0xa43ff20() {
   return (neuron0xa43e1c0()*0.410143);
}

double NNfunction::synapse0xa43ff48() {
   return (neuron0xa43e4e0()*-0.418323);
}

double NNfunction::synapse0xa43ff70() {
   return (neuron0xa43e730()*-0.501301);
}

double NNfunction::synapse0xa43ff98() {
   return (neuron0xa43e9c8()*-0.432059);
}

double NNfunction::synapse0xa43ffc0() {
   return (neuron0xa43ec60()*-0.331032);
}

double NNfunction::synapse0xa43ffe8() {
   return (neuron0xa43ef88()*-0.0902234);
}

double NNfunction::synapse0xa440010() {
   return (neuron0xa43f220()*-0.224523);
}

double NNfunction::synapse0xa440230() {
   return (neuron0xa43d850()*0.0870333);
}

double NNfunction::synapse0xa440258() {
   return (neuron0xa43da58()*-0.405871);
}

double NNfunction::synapse0xa440280() {
   return (neuron0xa43dcd0()*0.441849);
}

double NNfunction::synapse0xa4402a8() {
   return (neuron0xa43df48()*0.109542);
}

double NNfunction::synapse0xa4402d0() {
   return (neuron0xa43e1c0()*-0.167053);
}

double NNfunction::synapse0xa4402f8() {
   return (neuron0xa43e4e0()*0.281742);
}

double NNfunction::synapse0xa440320() {
   return (neuron0xa43e730()*0.441245);
}

double NNfunction::synapse0xa440348() {
   return (neuron0xa43e9c8()*0.452539);
}

double NNfunction::synapse0xa440370() {
   return (neuron0xa43ec60()*-0.0237023);
}

double NNfunction::synapse0xa440398() {
   return (neuron0xa43ef88()*0.134543);
}

double NNfunction::synapse0xa4403c0() {
   return (neuron0xa43f220()*-0.380718);
}

double NNfunction::synapse0xa4405e0() {
   return (neuron0xa43d850()*-0.505823);
}

double NNfunction::synapse0xa440608() {
   return (neuron0xa43da58()*-0.45246);
}

double NNfunction::synapse0xa440630() {
   return (neuron0xa43dcd0()*0.423014);
}

double NNfunction::synapse0xa440658() {
   return (neuron0xa43df48()*-1.06686);
}

double NNfunction::synapse0xa440680() {
   return (neuron0xa43e1c0()*0.083738);
}

double NNfunction::synapse0xa4406a8() {
   return (neuron0xa43e4e0()*-0.11761);
}

double NNfunction::synapse0xa4406d0() {
   return (neuron0xa43e730()*-0.229755);
}

double NNfunction::synapse0xa4406f8() {
   return (neuron0xa43e9c8()*0.206015);
}

double NNfunction::synapse0xa440720() {
   return (neuron0xa43ec60()*0.675996);
}

double NNfunction::synapse0xa440748() {
   return (neuron0xa43ef88()*0.35553);
}

double NNfunction::synapse0xa440770() {
   return (neuron0xa43f220()*0.2328);
}

double NNfunction::synapse0xa440990() {
   return (neuron0xa43d850()*0.0677177);
}

double NNfunction::synapse0xa4409b8() {
   return (neuron0xa43da58()*0.227081);
}

double NNfunction::synapse0xa4409e0() {
   return (neuron0xa43dcd0()*-0.443897);
}

double NNfunction::synapse0xa440a08() {
   return (neuron0xa43df48()*-0.326026);
}

double NNfunction::synapse0xa440a30() {
   return (neuron0xa43e1c0()*-0.0766752);
}

double NNfunction::synapse0xa440a58() {
   return (neuron0xa43e4e0()*0.20245);
}

double NNfunction::synapse0xa440a80() {
   return (neuron0xa43e730()*0.247048);
}

double NNfunction::synapse0xa440aa8() {
   return (neuron0xa43e9c8()*0.388035);
}

double NNfunction::synapse0xa440ad0() {
   return (neuron0xa43ec60()*0.367886);
}

double NNfunction::synapse0xa440af8() {
   return (neuron0xa43ef88()*-0.205376);
}

double NNfunction::synapse0xa440b20() {
   return (neuron0xa43f220()*-0.387547);
}

double NNfunction::synapse0xa440c48() {
   return (neuron0xa43f4e8()*-0.138094);
}

double NNfunction::synapse0xa440c70() {
   return (neuron0xa43f870()*-1.92059);
}

double NNfunction::synapse0xa440c98() {
   return (neuron0xa43ee80()*-0.764099);
}

double NNfunction::synapse0xa440cc0() {
   return (neuron0xa440038()*0.00520833);
}

double NNfunction::synapse0xa440ce8() {
   return (neuron0xa4403e8()*1.21582);
}

double NNfunction::synapse0xa440d10() {
   return (neuron0xa440798()*0.429813);
}

