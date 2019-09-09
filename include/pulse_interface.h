#ifndef MUSICVISUALIZER_PULSE_INTERFACE_H
#define MUSICVISUALIZER_PULSE_INTERFACE_H

#include <string>

namespace pulse_interface{
<<<<<<< HEAD
	void init_context();
	void init_stream(std::string dev);
	void deinit_context();
	void deinit_stream();
	int get_sinks();
	std::string get_active_sink_name();
=======
    void init_context();
    void deinit_context();
    int get_sinks();
    std::string get_active_sink_name();
>>>>>>> master
}

#endif //MUSICVISUALIZER_PULSE_INTERFACE_H