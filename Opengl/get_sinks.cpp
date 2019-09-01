#include <pulse/pulseaudio.h>

pa_mainloop *pa_ml;
pa_mainloop_api *pa_mlapi;
pa_context *c;
pa_operation *op;

void get_sink_info_callback(pa_context *c, const pa_sink_info *i, int is_last, void *userdata){


}

int main(int argc, char* args[])
{
    pa_ml = pa_mainloop_new();
    pa_mlapi = pa_mainloop_get_api(pa_ml);
    c = pa_context_new(pa_mlapi, "test");

    pa_context_connect(c, NULL, PA_CONTEXT_NOAUTOSPAWN, NULL);

    pa_context_disconnect(c);

    //op = pa_context_get_sink_info_list(c, get_sink_info_callback, NULL);
    return 0;
}
