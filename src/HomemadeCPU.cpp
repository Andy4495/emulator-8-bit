/* Homemade CPU Emulator 
   Copyright 2025 Andreas Taylor
   https://github.com/Andy4495/emulator-8-bit
   MIT License

   See https://github.com/Andy4495/Homemade-CPU for the CPU this emulates

   0.1  08-Oct-2025  Andy4495  Initial Creation


*/

#include "HomemadeCPU.h"
#include <iostream>
using std::cout;


// public: 
    HomemadeCPU::HomemadeCPU(uint16_t ramstart, uint16_t ramend) {

    }

    void HomemadeCPU::load_memory(const char* fname) {

    }

    void HomemadeCPU::dump_memory_to_file(const char* fname) {

    }
    
    void HomemadeCPU::cold_reset() {

    }
    
    void HomemadeCPU::warm_reset() {

    }
    
    void HomemadeCPU::run_from_address(uint16_t addr) {

    }
    
    void HomemadeCPU::fetch_and_decode() {

    }
    
    void HomemadeCPU::execute() {

    }
    
    void HomemadeCPU::print_fetched_instruction() {

    }
    
    void HomemadeCPU::print_registers() {

    }
    
    void HomemadeCPU::print_flags() {

    }
    
    void HomemadeCPU::print_memory(uint16_t start, uint16_t end) {

    }
    
    uint8_t HomemadeCPU::get_memory(uint16_t loc) {
        return 0;
    }

    void HomemadeCPU::set_memory(uint16_t loc, uint8_t val) {
        cout << "Not supported by Homemade CPU";
    }
    

    uint8_t HomemadeCPU::get_input_port(uint8_t loc) {
        cout << "Not supported by Homemade CPU";
        return 0;
    }
    

    void HomemadeCPU::set_input_port(uint8_t loc, uint8_t val) {
        cout << "Not supported by Homemade CPU";
    }
    
   uint8_t HomemadeCPU::get_output_port(uint8_t loc) {
        cout << "Not supported by Homemade CPU";
        return 0;
    }
    
    void HomemadeCPU::set_register() {

    }
    
    uint8_t HomemadeCPU::halt_opcode() {
        return 0;
    }
    
    // If true, the CPU is halted
    bool HomemadeCPU::halted() {  
        return true;
    }
    
    uint16_t HomemadeCPU::getPC() {
        return 0;
    }
    
    void HomemadeCPU::print_assembly() {

    }
    

// private:
    void HomemadeCPU::update_C(uint8_t val1, uint8_t val2) {

    }
    
    void HomemadeCPU::update_V(uint8_t val1, uint8_t val2) {

    }
    
    void HomemadeCPU::update_Z(uint8_t val) {

    }
    
    void HomemadeCPU::update_S(uint8_t val) {

    }
    
    uint16_t HomemadeCPU::getSP() {
        return 0;
    }
    
    // Jump Address: 16-bit equivalent of JH, JL
    uint16_t HomemadeCPU::getJA() {  
        return 0;
    }
    
    void HomemadeCPU::setSP(uint16_t v) {

    }
    
    void HomemadeCPU::setPC(uint8_t msb, uint8_t lsb) {

    }
    
    void HomemadeCPU::init_registers() {

    }
    
    void HomemadeCPU::setFlag(FLAG_BITS f) {

    }
    
    void HomemadeCPU::clearFlag(FLAG_BITS f) {

    }
    
    void HomemadeCPU::decode_instruction() {

    }
    
    void HomemadeCPU::execute_instruction() {

    }
    