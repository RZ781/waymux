#include <wayland-server.h>
#include "waymux.h"

void output_release(struct wl_client* wl_client, struct wl_resource* wl_output) {
	(void) wl_client;
	wl_resource_destroy(wl_output);
}

const struct wl_output_interface output_implementation = {
	.release = output_release
};

void bind_output(struct wl_client* wl_client, void* data, uint32_t version, uint32_t id) {
	(void) version;
	Display* display = data;
	Client* client = Display_get_client(display, wl_client);
	struct wl_resource* wl_output = wl_resource_create(wl_client, &wl_output_interface, wl_output_interface.version, id);
	client->wl_output = wl_output;
	wl_resource_set_implementation(wl_output, &output_implementation, NULL, NULL);
	wl_output_send_geometry(wl_output, 0, 0, 1000, 1000, WL_OUTPUT_SUBPIXEL_UNKNOWN, "John Make", "John Model", WL_OUTPUT_TRANSFORM_NORMAL);
	wl_output_send_mode(wl_output, WL_OUTPUT_MODE_CURRENT | WL_OUTPUT_MODE_PREFERRED, display->fb_width, display->fb_height, 60);
	wl_output_send_done(wl_output);
}
