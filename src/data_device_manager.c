#include <wayland-server.h>
#include "waymux.h"

void data_device_manager_get_data_device(struct wl_client* wl_client, struct wl_resource* wl_data_device_manager, uint32_t id, struct wl_resource* wl_seat) {
	(void) wl_data_device_manager; (void) wl_seat;
	struct wl_resource* wl_data_device = wl_resource_create(wl_client, &wl_data_device_interface, wl_data_device_interface.version, id);
	wl_resource_set_implementation(wl_data_device, &data_device_implementation, NULL, NULL);
}

const struct wl_data_device_manager_interface data_device_manager_implementation = {
	.get_data_device = data_device_manager_get_data_device
};

void bind_data_device_manager(struct wl_client* wl_client, void* data, uint32_t version, uint32_t id) {
	(void) data; (void) version;
	struct wl_resource* wl_data_device_manager = wl_resource_create(wl_client, &wl_data_device_manager_interface, wl_data_device_manager_interface.version, id);
	wl_resource_set_implementation(wl_data_device_manager, &data_device_manager_implementation, NULL, NULL);
}
