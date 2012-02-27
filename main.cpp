#include <glib.h>
#include <dbus/dbus-glib.h>
#include "marshal.h"

static void media_key_pressed(DBusGProxy *proxy, const char *value1, const char *value2, gpointer user_data) {
    g_print("mediakey: %s\n",value2);
}

int
main (int argc, char **argv)
{
    DBusGConnection *conn;
    DBusGProxy *proxy;
    GError *error;
    GMainLoop *loop;

    g_type_init();
    error = NULL;

    conn = dbus_g_bus_get (DBUS_BUS_SESSION, &error);
    if (!conn) {
        g_printerr("Failed to connect to the D-BUS daemon: %s\n", error->message);
        g_error_free(error);

        return 1;
    }

    loop = g_main_loop_new (NULL, FALSE);
    if(!loop) {
        g_printerr("Could not create mainloop\n");

        return 1;
    }

    proxy = dbus_g_proxy_new_for_name(conn,
                                    "org.gnome.SettingsDaemon",
                                    "/org/gnome/SettingsDaemon/MediaKeys",
                                    "org.gnome.SettingsDaemon.MediaKeys");

    if(!proxy) {
        g_printerr("Could not create proxy object\n");
    }

    error = NULL;
    if(!dbus_g_proxy_call(proxy,
                      "GrabMediaPlayerKeys", &error,
                      G_TYPE_STRING, "WebMediaKeys",
                      G_TYPE_UINT, 0,
                      G_TYPE_INVALID,
                      G_TYPE_INVALID)) {
        g_printerr("Could not grab media player keys: %s\n", error->message);
    }

    dbus_g_object_register_marshaller (
            g_cclosure_user_marshal_VOID__STRING_STRING,
            G_TYPE_NONE,
            G_TYPE_STRING,
            G_TYPE_STRING,
            G_TYPE_INVALID);

    dbus_g_proxy_add_signal(proxy,
                          "MediaPlayerKeyPressed",
                          G_TYPE_STRING,
                          G_TYPE_STRING,
                          G_TYPE_INVALID);

    dbus_g_proxy_connect_signal(proxy,
                              "MediaPlayerKeyPressed",
                              G_CALLBACK(media_key_pressed),
                              NULL,
                              NULL);

    g_print("Starting media key listener\n");
    g_main_loop_run (loop);

    return 0;
}

