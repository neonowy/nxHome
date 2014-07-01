#include <VirtualWire.h>
 
#define kontaktronPin 9    // 15 nóżka µC
 
const char *wiadomosc;
 
void setup() {
  vw_set_ptt_inverted(true);
  vw_setup(2000);
 
  pinMode(kontaktronPin, INPUT_PULLUP);
}
 
void loop() {
  if(digitalRead(kontaktronPin)==HIGH) {wiadomosc = "letter";}
  else                                 {wiadomosc = "empty";}
 
  vw_send((uint8_t *)wiadomosc, strlen(wiadomosc));
  vw_wait_tx();
}