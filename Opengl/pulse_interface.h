#ifndef MUSICVISUALIZER_OPENGL_PULSE_INTERFACE_H
#define MUSICVISUALIZER_OPENGL_PULSE_INTERFACE_H

#include <string>

namespace pulse_interface{
	void init_context();
	void deinit_context();
	int get_sinks();
	std::string get_active_sink_name();
}

#endif //MUSICVISUALIZER_OPENGL_PULSE_INTERFACE_H