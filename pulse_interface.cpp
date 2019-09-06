#include "pulse_interface.h"

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
        pa_threaded_mainloop *pa_ml;
        pa_mainloop_api *pa_mlapi;
        pa_context *pa_c;
        pa_stream *pa_s;
        pa_operation *pa_op;
        pa_context_state_t state;
        bool ml_locked = false;


        // Callback Functions

        /*
        ** Callback for getting the sink device information
        ** Stores the information in sink_list
        */
        void pa_sink_info_cb(pa_context *context, const pa_sink_info *i, int is_last, void *userdata){
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
        void context_state_cb(pa_context *context, void *mainloop) {
            pa_threaded_mainloop *m = reinterpret_cast<pa_threaded_mainloop*>(mainloop);
            pa_threaded_mainloop_signal(m, 0);
        }

        /*
        ** Callback for when the state changes
        */
        void stream_state_cb(pa_stream *stream, void *mainloop) {
            pa_threaded_mainloop *m = reinterpret_cast<pa_threaded_mainloop*>(mainloop);
            pa_threaded_mainloop_signal(m, 0);
        }

        void stream_read_cb(pa_stream *stream, size_t nbytes, void *userdata) {
            // TODO
        }

    } // namespace

    /*
    ** Function for initializing the PA variables
    ** Blocks until the mainloop is ready for operations
    */
    void init_context(){
        pa_ml = pa_threaded_mainloop_new();
        pa_mlapi = pa_threaded_mainloop_get_api(pa_ml);

        pa_c = pa_context_new(pa_mlapi, "musicvisualizer");

        pa_context_set_state_callback(pa_c, &context_state_cb, pa_ml);

        pa_threaded_mainloop_lock(pa_ml);
        ml_locked = true;

        pa_threaded_mainloop_start(pa_ml);
        pa_context_connect(pa_c, NULL, PA_CONTEXT_NOAUTOSPAWN, NULL);

        for(;;){
            state = pa_context_get_state(pa_c);
            if(!PA_CONTEXT_IS_GOOD(state)){
                deinit_context();
                return;
            }
            if(state == PA_CONTEXT_READY){
                break;
            }
            pa_threaded_mainloop_wait(pa_ml);
        }

        pa_threaded_mainloop_unlock(pa_ml);
        ml_locked = false;
    }

    /*
    ** Function for initializing the PA recording stream
    */
    void init_stream(){
        pa_sample_spec sample_spec;
        sample_spec.format = PA_SAMPLE_U8;
        sample_spec.rate = 44100;
        sample_spec.channels = 1;

        pa_channel_map map;
        pa_channel_map_init_mono(&map);

        pa_s = pa_stream_new(pa_c, "Recording", &sample_spec, &map);
        pa_stream_set_state_callback(pa_s, stream_state_cb, pa_ml);
        pa_stream_set_read_callback(pa_s, stream_read_cb, pa_ml);
    }

    void deinit_stream(){
        pa_stream_disconnect(pa_s);
        pa_stream_unref(pa_s);
    }

    /*
    ** Function for freeing variables
    */
    void deinit_context(){
        pa_context_disconnect(pa_c);
        pa_context_unref(pa_c);
        if(ml_locked){
            pa_threaded_mainloop_unlock(pa_ml);
        }
        pa_threaded_mainloop_stop(pa_ml);
        pa_threaded_mainloop_free(pa_ml);
    }

    /*
    ** Device list variables
    */
    int get_sinks(){
        pa_op = pa_context_get_sink_info_list(pa_c, pa_sink_info_cb, NULL);

        while(pa_operation_get_state(pa_op) != PA_OPERATION_DONE){
            pa_threaded_mainloop_wait(pa_ml);
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
