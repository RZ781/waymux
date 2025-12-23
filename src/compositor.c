#include <stdlib.h>
#include <wayland-server.h>
#include "waymux.h"

// TODO: add destructor for compositor

void compositor_create_surface(struct wl_client* wl_client, struct wl_resource* wl_compositor, uint32_t id) {
	Surface* surface = malloc(sizeof(Surface));
	Compositor* compositor = wl_resource_get_user_data(wl_compositor);
	Client* client = Display_get_client(compositor->display, wl_client);
	struct wl_resource* wl_surface = wl_resource_create(wl_client, &wl_surface_interface, wl_surface_interface.version, id);
	Surface_init(surface, wl_surface, compositor->display);
	wl_resource_set_implementation(wl_surface, &surface_implementation, surface, NULL);
	if (client->wl_output) {
		wl_surface_send_enter(wl_surface, client->wl_output);
	}
}

void compositor_create_region(struct wl_client* wl_client, struct wl_resource* wl_compositor, uint32_t id) {
	(void) wl_compositor;
	struct wl_resource* wl_region = wl_resource_create(wl_client, &wl_region_interface, 1, id);
	wl_resource_set_implementation(wl_region, &region_implementation, NULL, NULL);
}

const struct wl_compositor_interface compositor_implementation = {
	.create_surface = compositor_create_surface,
	.create_region = compositor_create_region
};

void bind_compositor(struct wl_client* wl_client, void* data, uint32_t version, uint32_t id) {
	(void) version;
	Display* display = data;
	struct wl_resource* wl_compositor = wl_resource_create(wl_client, &wl_compositor_interface, wl_compositor_interface.version, id);
	Compositor* compositor = malloc(sizeof(Compositor));
	compositor->wl_compositor = wl_compositor;
	compositor->display = display;
	wl_resource_set_implementation(wl_compositor, &compositor_implementation, compositor, NULL);
}
