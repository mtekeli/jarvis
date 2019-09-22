ARG QT_VERSION=5.12.5
ARG NUM_PARALLEL_JOBS=4

FROM mustafatekeli/qtbuilder:amd64-rpi-cross-compile-base-${QT_VERSION}-1

ARG JARVIS_HOST
ARG QT_VERSION
ARG NUM_PARALLEL_JOBS
ARG RPI_DIR=/root/raspi
ARG DEV_BINS=${RPI_DIR}/qt5pi
ARG HOST_BINS=${RPI_DIR}/qt5
ARG QT_MODULES=${HOST_BINS}/modules

ENV CMAKE_PREFIX_PATH ${DEV_BINS}

RUN apt-get update && \
	apt-get install -y \
	build-essential \
	libgles2-mesa-dev \
	curl \
	git \
	cmake \
	python \
	wget \ 
	&& apt-get clean \
	&& rm -rf /var/lib/apt/lists/*

COPY /sh/qt-install-module.sh ${RPI_DIR}/qt-install-module.sh

RUN echo ${QT_MODULES} && mkdir ${QT_MODULES} \
	&& cd ${RPI_DIR} \
	&& sh qt-install-module.sh ${QT_MODULES} ${QT_VERSION} ${HOST_BINS}/bin/qmake ${NUM_PARALLEL_JOBS} "qtdeclarative" \
	&& sh qt-install-module.sh ${QT_MODULES} ${QT_VERSION} ${HOST_BINS}/bin/qmake ${NUM_PARALLEL_JOBS} "qtquickcontrols" \
	&& sh qt-install-module.sh ${QT_MODULES} ${QT_VERSION} ${HOST_BINS}/bin/qmake ${NUM_PARALLEL_JOBS} "qtquickcontrols2" \
	&& sh qt-install-module.sh ${QT_MODULES} ${QT_VERSION} ${HOST_BINS}/bin/qmake ${NUM_PARALLEL_JOBS} "qtmultimedia" \
	&& sh qt-install-module.sh ${QT_MODULES} ${QT_VERSION} ${HOST_BINS}/bin/qmake ${NUM_PARALLEL_JOBS} "qtsvg" \
	&& sh qt-install-module.sh ${QT_MODULES} ${QT_VERSION} ${HOST_BINS}/bin/qmake ${NUM_PARALLEL_JOBS} "qtgraphicaleffects"

# copy over the ssh profile to connect to rpi
RUN rm -rf /root/.ssh/id_rsa
COPY .ssh/id_rsa /root/.ssh/id_rsa
# make sure your domain is accepted
RUN touch /root/.ssh/known_hosts
RUN ssh-keyscan ${JARVIS_HOST} >> /root/.ssh/known_hosts
