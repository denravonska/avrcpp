#pragma once

#include <stdint.h>

namespace avrcpp {

   volatile uint8_t& io_memory(const uint16_t loc) {
      return *reinterpret_cast<uint8_t*>(loc);
   }

   template<uint8_t PortAddr>
   struct IOPort {
      void enablePin(uint8_t pin) {
         io_memory(PortAddr) |= 1 << pin;
      }
   };

   template<template<typename PortAddr> struct IOPort, uint8_t Pin>
   struct IOPin {
      void enable() {
         Port.enablePin(Pin);
      }
   };

   IOPort<0x05 + 0x20> PortB;
   IOPort<0x0E + 0x20> PortE;
}

