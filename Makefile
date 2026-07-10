ds4-led: ds4-led.c
	gcc ds4-led.c -o ds4-led

.PHONY: install
install: ds4-led
	install -Dm755 ds4-led ${DESTDIR}${PREFIX}/bin/ds4-led
	install -Dm755 update-ds4-leds ${DESTDIR}${PREFIX}/bin/update-ds4-leds
	install -Dm644 50-ds4-led.rules ${DESTDIR}${PREFIX}/lib/udev/rules.d/50-ds4-led.rules
	install -Dm644 update-ds4-leds.service ${DESTDIR}${PREFIX}/lib/systemd/user/update-ds4-leds.service
