build: koko.xbm koko-mask.xbm
	gcc xkoko.cpp -I/usr/X11R6/include -I/usr/X11R6/include/X11 -L/usr/X11R6/lib -L/usr/X11R6/lib/X11 -lX11 -lXext

koko.xbm:
	convert koko.png koko.xbm

koko-mask.xbm:
	convert koko.png -alpha extract -negate koko-mask.xbm
	sed -i 's|-|_|' koko-mask.xbm
