ROOT_DIR:=${shell dirname ${realpath ${lastword ${MAKEFILE_LIST}}}}

.PHONY: build, config, config-rpi, build, fmt, install, deploy, docker, qtc, clean

build:
	@ cd ${BUILD_NAME} && make

install: build
	@ cd ${BUILD_NAME} && make install

deploy: install
	@ scp ${BUILD_NAME}/output/bin/jarvis ${JARVIS_DOMAIN}:/tmp
	@ ssh ${JARVIS_DOMAIN} 'sh /home/pi/update.sh'

config:
	@ mkdir -p ${BUILD_NAME}
	@ cd ${BUILD_NAME} && cmake ${CMAKE_OPTIONS} -DCMAKE_BUILD_TYPE=Release ../

config-rpi:
	@ mkdir -p ${BUILD_NAME}
	@ cd ${BUILD_NAME} && cmake ${CMAKE_OPTIONS} -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=toolchain-rpi.cmake ../

qtc:
	@ qtcreator ${ROOT_DIR}/CMakeLists.txt

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
		--env IPDATA_API_KEY=${IPDATA_API_KEY} \
		--env OPEN_WEATHER_API_KEY=${OPEN_WEATHER_API_KEY} \
		--env FIXER_EXCHANGES_API_KEY=${FIXER_EXCHANGES_API_KEY} \
		mustafatekeli/jarvis-cross-compile

fmt: config
	@ cd ${BUILD_NAME} && make fmt

clean:
	@ rm -rf ./$${BUILD_NAME:?} || true
