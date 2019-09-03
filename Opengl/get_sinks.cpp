#include <pulse/pulseaudio.h>
#include <iostream>

pa_mainloop *pa_ml;
pa_mainloop_api *pa_mlapi;
pa_context *c;
pa_operation *op;
pa_context_state_t state = PA_CONTEXT_UNCONNECTED;

struct pa_device{
    pa_sink_state_t state;
    std::string name;
    pa_device(){
        state = PA_SINK_INVALID_STATE;
        name = "";
    }
};

void get_sink_info_callback(pa_context *c, const pa_sink_info *i, int is_last, void *userdata){
    pa_device *d = reinterpret_cast<pa_device*>(userdata);

    if(is_last>0){
        return;
    }

    if(i->state == PA_SINK_RUNNING){
        std::cout << i->name << std::endl;
    }
}

void pa_state_cb(pa_context *c, void *userdata) {
    pa_context_state_t *s = reinterpret_cast<pa_context_state_t*>(userdata);
    *s = pa_context_get_state(c);
}

int main(int argc, char* args[])
{
    pa_device list[8];

    pa_ml = pa_mainloop_new();
    pa_mlapi = pa_mainloop_get_api(pa_ml);
    c = pa_context_new(pa_mlapi, "test");
    pa_context_connect(c, NULL, PA_CONTEXT_NOAUTOSPAWN, NULL);
    pa_context_set_state_callback(c, pa_state_cb, &state);

    while(state != PA_CONTEXT_READY){
        switch (state) {
            case PA_CONTEXT_FAILED:
            case PA_CONTEXT_TERMINATED:
                pa_context_disconnect(c);
                pa_context_unref(c);
                pa_mainloop_free(pa_ml);
                return -1;
            default:
                pa_mainloop_iterate(pa_ml, 1, NULL);
                break;
        }
    }

    std::cout << "starting op" << std::endl;
    op = pa_context_get_sink_info_list(c, get_sink_info_callback, list);


    while(pa_operation_get_state(op) != PA_OPERATION_DONE){
        pa_mainloop_iterate(pa_ml, 1, NULL);
    }

    pa_context_disconnect(c);
    pa_context_unref(c);
    pa_mainloop_free(pa_ml);
    return 0;
}
