#ifndef MUSICVISUALIZER_PULSE_INTERFACE_H
#define MUSICVISUALIZER_PULSE_INTERFACE_H

#include <string>

namespace pulse_interface{
	void init_context();
	void init_stream(std::string dev);
	void deinit_context();
	void deinit_stream();
	int get_sinks();
	std::string get_active_sink_name();
}

#endif //MUSICVISUALIZER_PULSE_INTERFACE_H
