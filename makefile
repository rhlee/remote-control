all:
	g++ main.cpp -o main `pkg-config --cflags --libs glib-2.0 dbus-glib-1`

marshall:
	glib-genmarshal --header --body mediaplayerkeypressed.list > marshal.h