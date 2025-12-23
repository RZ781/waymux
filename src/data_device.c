#include <wayland-server.h>
#include "waymux.h"

void data_device_release(struct wl_client* wl_client, struct wl_resource* wl_data_device) {
	(void) wl_client;
	wl_resource_destroy(wl_data_device);
}

const struct wl_data_device_interface data_device_implementation = {
	.release = data_device_release
};
