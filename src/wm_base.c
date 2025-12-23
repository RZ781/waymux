#include <wayland-server.h>
#include "waymux.h"

void wm_base_destroy(struct wl_client* wl_client, struct wl_resource* xdg_wm_base) {
	(void) wl_client;
	wl_resource_destroy(xdg_wm_base);
}

void wm_base_get_xdg_surface(struct wl_client* wl_client, struct wl_resource* xdg_wm_base, uint32_t id, struct wl_resource* wl_surface) {
	(void) xdg_wm_base; (void) wl_surface;
	struct wl_resource* xdg_surface = wl_resource_create(wl_client, &xdg_surface_interface, xdg_surface_interface.version, id);
	Surface* surface = wl_resource_get_user_data(wl_surface);
	surface->xdg_surface = xdg_surface;
	wl_resource_set_implementation(xdg_surface, &xdg_surface_implementation, surface, NULL);
}

const struct xdg_wm_base_interface wm_base_implementation = {
	.destroy = wm_base_destroy,
	.get_xdg_surface = wm_base_get_xdg_surface
};

void bind_wm_base(struct wl_client* wl_client, void* data, uint32_t version, uint32_t id) {
	(void) data; (void) version;
	struct wl_resource* xdg_wm_base = wl_resource_create(wl_client, &xdg_wm_base_interface, xdg_wm_base_interface.version, id);
	wl_resource_set_implementation(xdg_wm_base, &wm_base_implementation, NULL, NULL);
}
