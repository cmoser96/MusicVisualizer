#include "../include/pulse_interface.h"

#include <iostream>
#include <string>

int main(int argc, char* args[]){
    pulse_interface::init_context();
    std::cout << pulse_interface::get_sinks() << std::endl;
    std::string dev = pulse_interface::get_active_sink_name();
    std::cout << dev << std::endl;
    pulse_interface::init_stream(dev);
    pulse_interface::deinit_stream();
    pulse_interface::deinit_context();
    return 0;
}
