#include "pulse_interface.h"

#include <pulse/pulseaudio.h>

namespace pulse_interface{

	namespace {
		// Structs
		struct pa_device{
		    pa_sink_state_t state;
		    std::string name;
		    pa_device(){
		        state = PA_SINK_INVALID_STATE;
		        name = "";
		    }
		};

		// Variables
		const int NUM_MAX_SINKS = 8;
		int num_sinks_found = 0;
    	pa_device sink_list[NUM_MAX_SINKS];
		pa_mainloop *pa_ml;
		pa_mainloop_api *pa_mlapi;
		pa_context *c;
		pa_operation *op;
		pa_context_state_t state = PA_CONTEXT_UNCONNECTED;

		// Callback Functions
		void pa_sink_info_cb(pa_context *c, const pa_sink_info *i, int is_last, void *userdata){
		    if(is_last>0 || num_sinks_found==NUM_MAX_SINKS){
		        return;
		    }

		    sink_list[num_sinks_found].state = i->state;
		    sink_list[num_sinks_found].name = i->name;
		    num_sinks_found++;
		}

		void pa_state_cb(pa_context *c, void *userdata) {
		    state = pa_context_get_state(c);
		}

	} // namespace

	void init_context(){
		pa_ml = pa_mainloop_new();
	    pa_mlapi = pa_mainloop_get_api(pa_ml);
	    c = pa_context_new(pa_mlapi, "test");
	    pa_context_connect(c, NULL, PA_CONTEXT_NOAUTOSPAWN, NULL);
	    pa_context_set_state_callback(c, pa_state_cb, NULL);

	    while(state != PA_CONTEXT_READY){
	        switch (state) {
	            case PA_CONTEXT_FAILED:
	            case PA_CONTEXT_TERMINATED:
	                deinit_context();
	                return;
	            default:
	                pa_mainloop_iterate(pa_ml, 1, NULL);
	                break;
	        }
	    }
	}

	void deinit_context(){
		pa_context_disconnect(c);
	    pa_context_unref(c);
	    pa_mainloop_free(pa_ml);
	}

	int get_sinks(){
	    op = pa_context_get_sink_info_list(c, pa_sink_info_cb, NULL);

	    while(pa_operation_get_state(op) != PA_OPERATION_DONE){
	        pa_mainloop_iterate(pa_ml, 1, NULL);
	    }

	    return num_sinks_found;
	}

	std::string get_active_sink_name(){
		for(int i=0; i<num_sinks_found; i++){
			if(sink_list[i].state == PA_SINK_RUNNING){
				return sink_list[i].name;
			}
		}
		return "";
	}

} // pulse namespace
