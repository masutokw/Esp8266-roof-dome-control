#include "config.h"
#include "ir_control.h"
#ifdef IR_CONTROL
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

;
const uint16_t kRecvPin =IR_INPUT;

int obj = 0;
int n = 0;
double ra, dec;
IRrecv irrecv(kRecvPin);
decode_results results;
extern int target;
uint32_t last, lastpresstime, lasti, truecode;
char ir_state = 0;
void ir_init(void)
{
  String s;
  irrecv.enableIRIn();  // Start the receiver
  pinMode(kRecvPin, INPUT_PULLUP);
  }



void ir_read(void)
{
  uint32_t  code;
  if (irrecv.decode(&results))
  {
    if (results.value == 0xFFFFFFFFFFFFFFFF) code = 0xFFFF;
    else
    { truecode = results.command;

      lasti =last = code = truecode;

    }
    switch (code)
    {
      // print() & println() can't handle printing long longs. (uint64_t)
      case EAST:
       
        lastpresstime = millis();
        ir_state = 1;
        break;
      case WEST:
       
        lastpresstime = millis();
        ir_state = 1;
        break;
      case OK:
        soft_stop(15);
        lastpresstime = millis();
        ir_state = 1;
        break;
      case CLOSE:
        target=0;
        lastpresstime = millis();
        ir_state = 1;
        break; ;
      case OPEN:
        target=maxcounter;
        lastpresstime = millis();
        ir_state = 1;
        break;
       
      case B_1 :
        n = n * 10 + 1;
        break;
      case B_2 :
        n = n * 10 + 2;
        break;
      case B_3 :
        n = n * 10 + 3;
        break;
      case B_4 :
        n = n * 10 + 4;
        break;
      case B_5 :
        n = n * 10 + 5;
        break;
      case B_6 :
        n = n * 10 + 6;
        break;
      case B_7 :
        n = n * 10 + 7;
        break;
      case B_8 :
        n = n * 10 + 8;
        break;
      case B_9 :
        n = n * 10 + 9;
        break;
      case B_0 :
        n = n * 10;
        break;
      case GO_TO:
        if ((n < 111) && (n > 0))
        {

         
        }
        break;
      case CLEAR :
        hard_stop();
        break;
      case 0xFFFF:
        lastpresstime = millis();
        ir_state = 1;
        break; //mount_move(telescope, 's');
      default:
        ir_state = 0;
        break;
    }
    // last = code;
    irrecv.resume();  // Receive the next value
  }
  else if ((millis() - lastpresstime > 200) && (ir_state))
  {
    ir_state = 0;
    switch (last)
    {
      case EAST:
      case WEST:
      // mount_stop(telescope, 'w');
        break;
      case OPEN:
      case CLOSE:
      //  mount_stop(telescope, 's');
        break;
     
     
      default:
        break;

    }
    last = 0xFFFF;
  }
}
 #endif 
