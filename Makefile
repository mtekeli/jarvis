ROOT_DIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

.PHONY: docker, qtc

qtc:
	@ qtcreator $(ROOT_DIR)/CMakeLists.txt

docker:
	@ docker build -t mustafatekeli/jarvis-cross-compile .
	@ docker run --rm -it --name qt-build \
		-v /Users/mustafatekeli/dev/ws/jarvis:/root/jarvis \
		--workdir=//root/jarvis \
		--env BUILD_NAME=linux-armhf-rpi \
		mustafatekeli/jarvis-cross-compile