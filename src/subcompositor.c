#include <wayland-server.h>
#include "waymux.h"

void subcompositor_destroy(struct wl_client* wl_client, struct wl_resource* wl_subcompositor) {
	(void) wl_client;
	wl_resource_destroy(wl_subcompositor);
}

void subcompositor_get_subsurface(struct wl_client* wl_client, struct wl_resource* wl_subcompositor, uint32_t id, struct wl_resource* wl_surface, struct wl_resource* parent) {
	(void) wl_subcompositor; (void) wl_surface; (void) parent;
	struct wl_resource* wl_subsurface = wl_resource_create(wl_client, &wl_subsurface_interface, wl_subsurface_interface.version, id);
	Surface* surface = wl_resource_get_user_data(wl_surface);
	surface->parent = wl_resource_get_user_data(parent);
	surface->x = surface->parent->x - surface->parent->state.geo_x;
	surface->y = surface->parent->y - surface->parent->state.geo_y;
	wl_resource_set_implementation(wl_subsurface, &subsurface_implementation, surface, NULL);
}

const struct wl_subcompositor_interface subcompositor_implementation = {
	.destroy = subcompositor_destroy,
	.get_subsurface = subcompositor_get_subsurface
};

void bind_subcompositor(struct wl_client* wl_client, void* data, uint32_t version, uint32_t id) {
	(void) data; (void) version;
	struct wl_resource* wl_subcompositor = wl_resource_create(wl_client, &wl_subcompositor_interface, wl_subcompositor_interface.version, id);
	wl_resource_set_implementation(wl_subcompositor, &subcompositor_implementation, NULL, NULL);
}
