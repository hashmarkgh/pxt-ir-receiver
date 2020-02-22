#include "pxt.h"
#include <map>
#include <vector>
#include "ReceiverIR.h"
using namespace pxt;
typedef vector<Action> vA;

enum class Pins{
  P0=  3,
  P1=  2,
  P2=  1,
  P3=  4,
  P4=  5,
  P5=  17,
  P6=  12,
  P7=  11,
  P8=  18,
  P9=  10,
  P10= 6,
  P11= 26,
  P12= 20,
  P13= 23,
  P14= 22,
  P15= 21,
  P16= 16,
  P19= 0,
  P20= 30
};

//% color=50 weight=80
//% icon="\uf1eb"
namespace IR { 
  vA action;
  uint32_t lastact;
  uint8_t lastCode = 0xff;
  Timer tsb; 
  uint8_t buf[32];
  uint32_t now;
  ReceiverIR *rx;
  RemoteIR::Format fmt = RemoteIR::UNKNOWN;

  void cA(vA runner){
	auto code = fromInt(buf[2]);
	for(int i=0;i<runner.size();i++) {
		runAction1(runner[i], code);
	}
  }

  void onReceivable(){
    int x = rx->getData(&fmt, buf, 32 * 8);
    now = tsb.read_ms();
    if((buf[2] == lastCode) && (now - lastact) < 100) return;
    lastCode = buf[2];
    lastact = now;
    cA(action);
  }

  void monitorIR(){
    while(1){
      while(rx->getState() != ReceiverIR::Received){ 
        uBit.sleep(50);
      }
      onReceivable();
    }
  }

  //%
  void init(Pins pin){
    rx = new ReceiverIR((PinName)pin);
    tsb.start(); //interrupt timer for debounce
    create_fiber(monitorIR);
  }

  //% 
  void onPressEvent(Action body) {
    action.push_back(body);
  }
}
