#include "../include/pulse_interface.h"

#include <pulse/pulseaudio.h>

namespace pulse_interface{

    namespace {
        // Structs

        /*
        ** Struct for storing sink device info
        */
        struct pa_device{
            pa_sink_state_t state;
            std::string name;
            pa_device(){
                state = PA_SINK_INVALID_STATE;
                name = "";
            }
        };


        // Variables

        /*
        ** Device list variables
        */
        const int NUM_MAX_SINKS = 8;
        int num_sinks_found = 0;
        pa_device sink_list[NUM_MAX_SINKS];

        /*
        ** PA Async variables
        */
        pa_mainloop *pa_ml;
        pa_mainloop_api *pa_mlapi;
        pa_context *pa_c;
        pa_operation *pa_op;
        pa_context_state_t state = PA_CONTEXT_UNCONNECTED;


        // Callback Functions

        /*
        ** Callback for getting the sink device information
        ** Stores the information in sink_list
        */
        void pa_sink_info_cb(pa_context *pa_c, const pa_sink_info *i, int is_last, void *userdata){
            if(is_last>0 || num_sinks_found==NUM_MAX_SINKS){
                return;
            }

            sink_list[num_sinks_found].state = i->state;
            sink_list[num_sinks_found].name = i->name;
            num_sinks_found++;
        }

        /*
        ** Callback for when the state changes
        */
        void pa_state_cb(pa_context *pa_c, void *userdata) {
            state = pa_context_get_state(pa_c);
        }
    } // namespace

    /*
    ** Function for initializing the PA variables
    ** Blocks until the mainloop is ready for operations
    */
    void init_context(){
        pa_ml = pa_mainloop_new();
        pa_mlapi = pa_mainloop_get_api(pa_ml);
        pa_c = pa_context_new(pa_mlapi, "test");
        pa_context_connect(pa_c, NULL, PA_CONTEXT_NOAUTOSPAWN, NULL);
        pa_context_set_state_callback(pa_c, pa_state_cb, NULL);

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

    /*
    ** Function for freeing variables
    */
    void deinit_context(){
        pa_context_disconnect(pa_c);
        pa_context_unref(pa_c);
        pa_mainloop_free(pa_ml);
    }

    /*
    ** Device list variables
    */
    int get_sinks(){
        pa_op = pa_context_get_sink_info_list(pa_c, pa_sink_info_cb, NULL);

        while(pa_operation_get_state(pa_op) != PA_OPERATION_DONE){
            pa_mainloop_iterate(pa_ml, 1, NULL);
        }

        return num_sinks_found;
    }

    /*
    ** Function that returns the string of the currently
    ** running sink or an empty string if none exist
    **
    ** NOTE: Assumes only one active sink
    */
    std::string get_active_sink_name(){
        for(int i=0; i<num_sinks_found; i++){
            if(sink_list[i].state == PA_SINK_RUNNING){
                return sink_list[i].name;
            }
        }
        return "";
    }

} // pulse namespace
