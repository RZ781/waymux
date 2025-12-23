#ifndef WAYMUX_H
#define WAYMUX_H

#include <wayland-server.h>
#include "xdg_shell.h"

#define WIDTH 100
#define HEIGHT 80

typedef struct {
	struct wl_client* wl_client;
	struct wl_resource* wl_output;
	struct wl_list link;
} Client;

typedef struct {
	struct wl_display* wl_display;
	struct wl_list clients;
} Display;

typedef struct {
	Display* display;
	struct wl_resource* wl_compositor;
} Compositor;

typedef struct {
	struct wl_resource* wl_surface;
	struct wl_resource* xdg_surface;
	struct wl_resource* xdg_toplevel;
	Display* display;
	struct wl_resource* wl_buffer;
} Surface;

extern const struct wl_surface_interface surface_implementation;
extern const struct xdg_surface_interface xdg_surface_implementation;
extern const struct xdg_toplevel_interface xdg_toplevel_implementation;
extern const struct wl_region_interface region_implementation;
extern const struct wl_subsurface_interface subsurface_implementation;
extern const struct wl_data_device_interface data_device_implementation;

void bind_compositor(struct wl_client* wl_client, void* data, uint32_t version, uint32_t id);
void bind_subcompositor(struct wl_client* wl_client, void* data, uint32_t version, uint32_t id);
void bind_output(struct wl_client* wl_client, void* data, uint32_t version, uint32_t id);
void bind_seat(struct wl_client* wl_client, void* data, uint32_t version, uint32_t id);
void bind_data_device_manager(struct wl_client* wl_client, void* data, uint32_t version, uint32_t id);
void bind_wm_base(struct wl_client* wl_client, void* data, uint32_t version, uint32_t id);

void Display_init(Display* display);
Client* Display_get_client(Display* display, struct wl_client* wl_client);
void Client_init(Client* client, struct wl_client* wl_client);
void Surface_init(Surface* surface, struct wl_resource* wl_surface, Display* display);

#endif
