#include <wayland-server.h>
#include "waymux.h"

void region_destroy(struct wl_client* wl_client, struct wl_resource* wl_region) {
	(void) wl_client;
	wl_resource_destroy(wl_region);
}

void region_add(struct wl_client* wl_client, struct wl_resource* wl_region, int32_t x, int32_t y, int32_t width, int32_t height) {
	(void) wl_client; (void) wl_region; (void) x; (void) y; (void) width; (void) height;
}

const struct wl_region_interface region_implementation = {
	.destroy = region_destroy,
	.add = region_add
};
