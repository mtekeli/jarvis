ROOT_DIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

.PHONY: build, config, config-rpi build, fmt, install, deploy, docker, qtc, clean

build:
	@ cd bin && make

install: build
	@ cd bin && make install

deploy: install
	@ ssh ${JARVIS_DOMAIN} 'sudo systemctl stop jarvis.service'
	@ scp output/bin/jarvis $(JARVIS_DOMAIN):/home/pi
	@ ssh ${JARVIS_DOMAIN} 'sudo systemctl start jarvis.service'

config: clean
	@ mkdir -p bin
	@ cd bin && cmake ${CMAKE_OPTIONS} ../

config-rpi: clean
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
		mustafatekeli/jarvis-cross-compile

fmt: config
	@ cd bin && make fmt

clean:
	@ rm -rf ./bin && rm -rf ./output || true
