#include <stdio.h>
#include <wayland-server.h>
#include "xdg_shell.h"
#include "waymux.h"

// TODO: add destructor to free surface and deref buffer

// surface
void surface_destroy(struct wl_client* wl_client, struct wl_resource* wl_surface) {
	(void) wl_client;
	wl_resource_destroy(wl_surface);
}

void surface_set_opaque_region(struct wl_client* wl_client, struct wl_resource* wl_surface, struct wl_resource* wl_region) {
	(void) wl_client; (void) wl_surface; (void) wl_region;
}

void surface_commit(struct wl_client* wl_client, struct wl_resource* wl_surface) {
	(void) wl_client; (void) wl_surface;
}

void surface_set_input_region(struct wl_client* wl_client, struct wl_resource* wl_surface, struct wl_resource* wl_region) {
	(void) wl_client; (void) wl_surface; (void) wl_region;
}

void surface_attach(struct wl_client* wl_client, struct wl_resource* wl_surface, struct wl_resource* wl_buffer, int32_t x, int32_t y) {
	(void) wl_client; (void) wl_surface; (void) wl_buffer; (void) x; (void) y;
	Surface* surface = wl_resource_get_user_data(wl_surface);
	if (surface->wl_buffer != NULL) {
		wl_shm_buffer_unref(wl_shm_buffer_get(surface->wl_buffer));
	}
	surface->wl_buffer = wl_buffer;
	if (wl_buffer != NULL) {
		wl_shm_buffer_ref(wl_shm_buffer_get(wl_buffer));
	}
}

void surface_set_buffer_scale(struct wl_client* wl_client, struct wl_resource* wl_surface, int32_t buffer_scale) {
	(void) wl_client; (void) wl_surface; (void) buffer_scale;
}

void surface_damage_buffer(struct wl_client* wl_client, struct wl_resource* wl_surface, int32_t x, int32_t y, int32_t width, int32_t height) {
	(void) wl_client;
	Surface* surface = wl_resource_get_user_data(wl_surface);
	if (surface == NULL) {
		printf("error: wl_surface user data is null\n");
		return;
	}
	struct wl_shm_buffer* wl_buffer = wl_shm_buffer_get(surface->wl_buffer);
	if (wl_buffer == NULL) {
		printf("error: surface buffer is null\n");
		return;
	}
	uint8_t* data = wl_shm_buffer_get_data(wl_buffer);
	if (data == NULL) {
		printf("error: buffer data is null\n");
		return;
	}
	int32_t stride = wl_shm_buffer_get_stride(wl_buffer);
	wl_shm_buffer_begin_access(wl_buffer);
	for (int i=0; i<height; i++) {
		for (int j=0; j<width; j++) {
			int offset = stride * (y+i) + 4*(x+j);
			putchar(data[offset] ? ' ' : '.');
		}
		putchar('\n');
	}
	wl_shm_buffer_end_access(wl_buffer);
}

const struct wl_surface_interface surface_implementation = {
	.destroy = surface_destroy,
	.set_opaque_region = surface_set_opaque_region,
	.commit = surface_commit,
	.set_input_region = surface_set_input_region,
	.attach = surface_attach,
	.set_buffer_scale = surface_set_buffer_scale,
	.damage_buffer = surface_damage_buffer
};

// xdg surface
void xdg_surface_destroy(struct wl_client* wl_client, struct wl_resource* xdg_surface) {
	(void) wl_client;
	wl_resource_destroy(xdg_surface);
}

void xdg_surface_get_toplevel(struct wl_client* wl_client, struct wl_resource* xdg_surface, uint32_t id) {
	struct wl_resource* xdg_toplevel = wl_resource_create(wl_client, &xdg_toplevel_interface, xdg_toplevel_interface.version, id);
	Surface* surface = wl_resource_get_user_data(xdg_surface);
	surface->xdg_toplevel = xdg_toplevel;
	wl_resource_set_implementation(xdg_toplevel, &xdg_toplevel_implementation, surface, NULL);
	struct wl_array capabilities;
	wl_array_init(&capabilities);
	xdg_toplevel_send_wm_capabilities(surface->xdg_toplevel, &capabilities);
	wl_array_release(&capabilities);
	xdg_toplevel_send_configure_bounds(surface->xdg_toplevel, WIDTH, HEIGHT);
	struct wl_array states;
	wl_array_init(&states);
	wl_array_add(&states, 4 * 2);
	uint32_t* data = states.data;
	data[0] = XDG_TOPLEVEL_STATE_MAXIMIZED;
	data[1] = XDG_TOPLEVEL_STATE_ACTIVATED;
	xdg_toplevel_send_configure(surface->xdg_toplevel, WIDTH, HEIGHT, &states);
	wl_array_release(&states);
	xdg_surface_send_configure(surface->xdg_surface, wl_display_get_serial(surface->display->wl_display));
}

void xdg_surface_ack_configure(struct wl_client* wl_client, struct wl_resource* xdg_surface, uint32_t serial) {
	(void) wl_client; (void) serial; (void) xdg_surface;
}

void xdg_surface_set_window_geometry(struct wl_client* wl_client, struct wl_resource* xdg_surface, int32_t x, int32_t y, int32_t width, int32_t height) {
	(void) wl_client; (void) xdg_surface; (void) x; (void) y; (void) width; (void) height;
}

const struct xdg_surface_interface xdg_surface_implementation = {
	.destroy = xdg_surface_destroy,
	.get_toplevel = xdg_surface_get_toplevel,
	.ack_configure = xdg_surface_ack_configure,
	.set_window_geometry = xdg_surface_set_window_geometry
};

// subsurface
void subsurface_destroy(struct wl_client* wl_client, struct wl_resource* wl_subsurface) {
	(void) wl_client;
	wl_resource_destroy(wl_subsurface);
}

void subsurface_set_sync(struct wl_client* wl_client, struct wl_resource* wl_subsurface) {
	(void) wl_client; (void) wl_subsurface;
}

void subsurface_set_position(struct wl_client* wl_client, struct wl_resource* wl_subsurface, int32_t x, int32_t y) {
	(void) wl_client; (void) wl_subsurface; (void) x; (void) y;
}

const struct wl_subsurface_interface subsurface_implementation = {
	.destroy = subsurface_destroy,
	.set_position = subsurface_set_position,
	.set_sync = subsurface_set_sync
};

// xdg toplevel
void xdg_toplevel_destroy(struct wl_client* wl_client, struct wl_resource* xdg_toplevel) {
	(void) wl_client;
	wl_resource_destroy(xdg_toplevel);
}

void xdg_toplevel_set_app_id(struct wl_client* wl_client, struct wl_resource* xdg_toplevel, const char* app_id) {
	(void) wl_client; (void) xdg_toplevel; (void) app_id;
}

void xdg_toplevel_set_title(struct wl_client* wl_client, struct wl_resource* xdg_toplevel, const char* title) {
	(void) wl_client; (void) xdg_toplevel; (void) title;
}

void xdg_toplevel_set_min_size(struct wl_client* wl_client, struct wl_resource* xdg_toplevel, int32_t x, int32_t y) {
	(void) wl_client; (void) xdg_toplevel; (void) x; (void) y;
}

void xdg_toplevel_set_window_geometry(struct wl_client* wl_client, struct wl_resource* xdg_toplevel, int32_t x, int32_t y, int32_t width, int32_t height) {
	(void) wl_client; (void) xdg_toplevel; (void) x; (void) y; (void) width; (void) height;
}

const struct xdg_toplevel_interface xdg_toplevel_implementation = {
	.destroy = xdg_toplevel_destroy,
	.set_app_id = xdg_toplevel_set_app_id,
	.set_title = xdg_toplevel_set_title,
	.set_min_size = xdg_toplevel_set_min_size,
};

void Surface_init(Surface* surface, struct wl_resource* wl_surface, Display* display) {
	surface->wl_surface = wl_surface;
	surface->xdg_surface = NULL;
	surface->xdg_toplevel = NULL;
	surface->display = display;
	surface->wl_buffer = NULL;
}
