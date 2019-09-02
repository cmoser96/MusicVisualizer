#include <pulse/pulseaudio.h>
#include <iostream>

pa_mainloop *pa_ml;
pa_mainloop_api *pa_mlapi;
pa_context *c;
pa_operation *op;

void get_sink_info_callback(pa_context *c, const pa_sink_info *i, int is_last, void *userdata){
    if(is_last>0){
        return;
    }
    if(i->state == PA_SINK_RUNNING){
        std::cout << i->name << std::endl;
    }
}

void pa_state_cb(pa_context *c, void *userdata) {
        pa_context_state_t state;
        int *pa_ready = (int*) userdata;

        state = pa_context_get_state(c);

        switch  (state) {
                // There are just here for reference
                case PA_CONTEXT_UNCONNECTED:
                    std::cout << "PA_CONTEXT_UNCONNECTED" << std::endl;
                    break;
                case PA_CONTEXT_CONNECTING:
                    std::cout << "PA_CONTEXT_CONNECTING" << std::endl;
                    break;
                case PA_CONTEXT_AUTHORIZING:
                    std::cout << "PA_CONTEXT_AUTHORIZING" << std::endl;
                    break;
                case PA_CONTEXT_SETTING_NAME:
                    std::cout << "PA_CONTEXT_SETTING_NAME" << std::endl;
                    break;
                default:
                        break;
                case PA_CONTEXT_FAILED:
                    std::cout << "PA_CONTEXT_FAILED" << std::endl;
                    break;
                case PA_CONTEXT_TERMINATED:
                        std::cout << "PA_CONTEXT_TERMINATED" << std::endl;
                        *pa_ready = 2;
                        break;
                case PA_CONTEXT_READY:
                        *pa_ready = 1;
                        std::cout << "PA_CONTEXT_READY" << std::endl;
                        break;
        }
}

int main(int argc, char* args[])
{
    pa_ml = pa_mainloop_new();
    pa_mlapi = pa_mainloop_get_api(pa_ml);
    c = pa_context_new(pa_mlapi, "test");
    int pa_ready = 0;
    pa_context_connect(c, NULL, PA_CONTEXT_NOAUTOSPAWN, NULL);
    pa_context_set_state_callback(c, pa_state_cb, &pa_ready);

    for(;;){
        if (pa_ready == 0) {
            pa_mainloop_iterate(pa_ml, 1, NULL);
            continue;
        }
        if (pa_ready == 2) {
            pa_context_disconnect(c);
            pa_context_unref(c);
            pa_mainloop_free(pa_ml);
            return -1;
        }
        if (pa_ready == 1) {
            break;
        }
    }
    std::cout << "starting op" << std::endl;

    op = pa_context_get_sink_info_list(c, get_sink_info_callback, NULL);


    while(pa_operation_get_state(op) != PA_OPERATION_DONE){
        if (pa_ready == 0) {
            pa_mainloop_iterate(pa_ml, 1, NULL);
            continue;
        }

        pa_mainloop_iterate(pa_ml, 1, NULL);
    }

    pa_context_disconnect(c);
    pa_context_unref(c);
    pa_mainloop_free(pa_ml);
    return 0;
}
