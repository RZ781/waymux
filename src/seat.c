#include <wayland-server.h>
#include "waymux.h"

void seat_release(struct wl_client* wl_client, struct wl_resource* wl_seat) {
	(void) wl_client;
	wl_resource_destroy(wl_seat);
}

const struct wl_seat_interface seat_implementation = {
	.release = seat_release
};

void bind_seat(struct wl_client* wl_client, void* data, uint32_t version, uint32_t id) {
	(void) data; (void) version;
	struct wl_resource* wl_seat = wl_resource_create(wl_client, &wl_seat_interface, wl_seat_interface.version, id);
	wl_resource_set_implementation(wl_seat, &seat_implementation, NULL, NULL);
	//wl_seat_send_capabilities(resource, WL_SEAT_CAPABILITY_KEYBOARD);
	wl_seat_send_capabilities(wl_seat, 0);
	wl_seat_send_name(wl_seat, "John Seat");
}
