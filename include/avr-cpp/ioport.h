#pragma once

#include <stdint.h>

namespace avrcpp {

    enum PinDirection {
        INPUT,
        OUTPUT,
        INPUT_PULL_UP
    };

    volatile uint8_t& io_memory(const uint16_t loc) {
        return *reinterpret_cast<uint8_t*>(loc);
    }

    void set_bits(uint8_t addr, uint8_t mask) {
        io_memory(addr) |= mask;
    }

    uint8_t get_bits(uint8_t addr, uint8_t mask) {
        return io_memory(addr);
    }

    void clear_bits(uint8_t addr, uint8_t mask) {
        io_memory(addr) &= ~mask;
    }

    template<
            const uint8_t PortAddr,
            const uint8_t DdrAddr,
            const uint8_t PinAddr>
    struct IOPort {
        static void setDdr(uint8_t mask) {
            set_bits(DdrAddr, mask);
        }

        static void clearDdr(uint8_t mask) {
            clear_bits(DdrAddr, mask);
        }

        static void setPin(uint8_t mask) {
            set_bits(PinAddr, mask);
        }

        static void clearPin(uint8_t mask) {
            clear_bits(PinAddr, mask);
        }

        static int readPin(uint8_t mask) {
            return get_bits(PinAddr, mask);
        }
    };

    template<typename Port, uint8_t Pin>
    struct IOPin {
        void setDirection(const PinDirection direction) {
            switch(direction) {
                case INPUT:
                    Port::clearDdr(1 << Pin);
                    Port::clearPin(1 << Pin);
                    break;
                case INPUT_PULL_UP:
                    Port::clearDdr(1 << Pin);
                    Port::setPin(1 << Pin);
                    break;
                case OUTPUT:
                    Port::setDdr(1 << Pin);
                    break;
            }
        }

        void set() {
            Port::setPin(1 << Pin);
        }

        void clear() {
            Port::clearPin(1 << Pin);
        }

        bool read() {
            return Port::readPin(1 << Pin);
        }
    };

    const uint8_t BASE_OFFSET = 0x20;
    const uint8_t PIN_OFFSET  = 0x00;
    const uint8_t DDR_OFFSET  = 0x01;
    const uint8_t PORT_OFFSET = 0x02;
    const uint8_t IOA         = 0x00 + BASE_OFFSET;
    const uint8_t IOB         = 0x03 + BASE_OFFSET;
    const uint8_t IOC         = 0x06 + BASE_OFFSET;
    const uint8_t IOD         = 0x09 + BASE_OFFSET;
    const uint8_t IOE         = 0x0C + BASE_OFFSET;

    using PortA = IOPort<IOA + PORT_OFFSET, IOA + DDR_OFFSET, IOA + PIN_OFFSET>;
    using PortB = IOPort<IOB + PORT_OFFSET, IOB + DDR_OFFSET, IOB + PIN_OFFSET>;
    using PortC = IOPort<IOC + PORT_OFFSET, IOC + DDR_OFFSET, IOC + PIN_OFFSET>;
    using PortD = IOPort<IOD + PORT_OFFSET, IOD + DDR_OFFSET, IOD + PIN_OFFSET>;
    using PortE = IOPort<IOE + PORT_OFFSET, IOE + DDR_OFFSET, IOE + PIN_OFFSET>;
}

