#include "ioport.h"



int main() {
   avrcpp::PortB.enablePin(4);

   while(1);
}

