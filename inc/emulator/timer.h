#ifndef TIMER_H
#define TIMER_H

#include "common.h"

class MMU;

class Timer {
public:
    Timer();
    void M_tick();
    void write(uint16 addr, uint8 val);
    uint8 read(uint16 addr) const;
    void setMMU(MMU* mmu) { this->mmu = mmu; }
private:
    enum class State {
        None,
        Overflow,
        Reload
    };
    MMU* mmu = nullptr;
    void initialize();
    void reset_div();
    void write_tima(uint8 val);
    void write_tma(uint8 val);
    void write_tac(uint8 val);
    uint8 read_div() const;
    uint8 read_tima() const;
    uint8 read_tma() const;
    uint8 read_tac() const;
    void tima_tick();
    void falling_edge();
    uint16 div;
    uint8 tima;
    uint8 tma;
    uint8 tac;
    bool prev_bit;
    State state;
};


#endif // TIMER_H