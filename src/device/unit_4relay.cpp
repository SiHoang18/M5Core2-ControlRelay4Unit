#include "unit_4relay.h"
UNIT_4RELAY relay;
void init_relay(){
    relay.begin();
    relay.Init(true);
}
