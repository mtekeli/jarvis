ROOT_DIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

.PHONY: build, config, config-rpi build, install, deploy, docker, qtc, clean

build:
	@ cd bin && make

install: build
	@ cd bin && make install

deploy:
	@ ssh ${JARVIS_HOST} 'sudo systemctl stop jarvis.service'
	@ scp bin/src/jarvis $(JARVIS_HOST):/home/pi
	@ ssh ${JARVIS_HOST} 'sudo systemctl start jarvis.service'

config: clean
	@ mkdir -p bin
	@ cd bin && cmake ../

config-rpi: clean
	@ mkdir -p bin
	@ cd bin && cmake -DCMAKE_TOOLCHAIN_FILE=toolchain-rpi.cmake ../

qtc:
	@ qtcreator $(ROOT_DIR)/CMakeLists.txt

docker:
	@ docker build -t mustafatekeli/jarvis-cross-compile .
	@ docker run --rm -it --name qt-build \
		-v /Users/mustafatekeli/dev/ws/jarvis:/root/jarvis \
		--workdir=//root/jarvis \
		--env BUILD_NAME=linux-armhf-rpi \
		--env JARVIS_HOST=$(JARVIS_HOST)
		mustafatekeli/jarvis-cross-compile

clean:
	@ rm -rf ./bin || true
