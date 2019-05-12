ROOT_DIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

.PHONY: build, config, config-rpi build, fmt, install, deploy, docker, qtc, clean

build:
	@ cd bin && make

install: build
	@ cd bin && make install

deploy: install
	@ scp output/bin/jarvis ${JARVIS_DOMAIN}:/tmp
	@ ssh ${JARVIS_DOMAIN} 'sh /home/pi/update.sh'

config:
	@ mkdir -p bin
	@ cd bin && cmake ${CMAKE_OPTIONS} ../

config-rpi:
	@ mkdir -p bin
	@ cd bin && cmake ${CMAKE_OPTIONS} -DCMAKE_TOOLCHAIN_FILE=toolchain-rpi.cmake ../

qtc:
	@ qtcreator $(ROOT_DIR)/CMakeLists.txt

docker:
	@ mkdir -p .ssh && cp ${SSH_KEY_FILE} .ssh/id_rsa
	@ docker build -t mustafatekeli/jarvis-cross-compile \
		--build-arg JARVIS_HOST=${JARVIS_HOST} \
		.
	@ docker run --rm -it --name qt-build \
		-v /Users/mustafatekeli/dev/ws/jarvis:/root/jarvis \
		--workdir=//root/jarvis \
		--env BUILD_NAME=linux-armhf-rpi \
		--env JARVIS_HOST=${JARVIS_HOST} \
		--env JARVIS_DOMAIN=${JARVIS_DOMAIN} \
		--env IPDATA_API_KEY=$(IPDATA_API_KEY) \
		mustafatekeli/jarvis-cross-compile

fmt: config
	@ cd bin && make fmt

clean:
	@ rm -rf ./bin && rm -rf ./output || true
