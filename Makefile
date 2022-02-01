
default: build

build:

	mkdir -p bin
	go build -o bin/loadoutput main.go

install: build

	cp bin/loadoutput /usr/local/bin/loadoutput
	cp loadoutput.service /lib/systemd/system/loadoutput.service
	systemctl enable loadoutput.service

uninstall:

	rm -f /usr/local/bin/loadoutput
	rm -f /lib/systemd/system/loadoutput.service

run: install

	systemctl restart loadoutput.service
