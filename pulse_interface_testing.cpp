#include "pulse_interface.h"
#include <iostream>

int main(int argc, char* args[]){
    pulse_interface::init_context();
    std::cout << pulse_interface::get_sinks() << std::endl;
    std::cout << pulse_interface::get_active_sink_name() << std::endl;
    pulse_interface::deinit_context();
    return 0;
}
