all:
	g++ `pkg-config --cflags --libs glib-2.0 dbus-glib-1` main.cpp -o main