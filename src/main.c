#include <wayland-server.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "xdg_shell.h"
#include "waymux.h"

#include <linux/fb.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/ioctl.h>

// TODO: client destructor
// TODO: change ints to int32_ts

void Display_init(Display* display) {
	// initialise attributes
	struct wl_display* wl_display = wl_display_create();
	if (wl_display == NULL) {
		perror("failed to create wayland display");
		exit(-1);
	}
	display->wl_display = wl_display;
	display->y = 0;
	wl_list_init(&display->clients);
	wl_display_init_shm(wl_display);
	// create global singletons
	wl_global_create(wl_display, &wl_compositor_interface, wl_compositor_interface.version, display, bind_compositor);
	wl_global_create(wl_display, &wl_subcompositor_interface, wl_subcompositor_interface.version, display, bind_subcompositor);
	wl_global_create(wl_display, &xdg_wm_base_interface, xdg_wm_base_interface.version, display, bind_wm_base);
	wl_global_create(wl_display, &wl_data_device_manager_interface, wl_data_device_manager_interface.version, display, bind_data_device_manager);
	wl_global_create(wl_display, &wl_seat_interface, wl_seat_interface.version, display, bind_seat);
	wl_global_create(wl_display, &wl_output_interface, wl_output_interface.version, display, bind_output);
	// add socket
	char path[512];
	snprintf(path, sizeof(path), "%s/waymux", getenv("XDG_RUNTIME_DIR"));
	unlink(path);
	wl_display_add_socket(wl_display, path);
}

Client* Display_get_client(Display* display, struct wl_client* wl_client) {
	Client* client;
	wl_list_for_each(client, &display->clients, link) {
		if (client->wl_client == wl_client) {
			return client;
		}
	}
	// client not found, create a new one
	client = malloc(sizeof(Client));
	Client_init(client, wl_client);
	wl_list_insert(&display->clients, &client->link);
	return client;
}

void Client_init(Client* client, struct wl_client* wl_client) {
	client->wl_client = wl_client;
	client->wl_output = NULL;
	wl_list_init(&client->link);
}

int main() {
	int fbfd = open("/dev/fb0", O_RDWR);
	if (fbfd == -1) {
		perror("failed to open framebuffer");
		exit(-1);
	}
	struct fb_var_screeninfo vinfo;
	ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo);
	Display display;
	Display_init(&display);
	display.fb_width = vinfo.xres;
	display.fb_height = vinfo.yres;
	display.fb_bytes = vinfo.bits_per_pixel / 8;
	display.framebuffer = mmap(0, display.fb_width * display.fb_height * display.fb_bytes, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	wl_display_run(display.wl_display);
	wl_display_destroy(display.wl_display);
}
