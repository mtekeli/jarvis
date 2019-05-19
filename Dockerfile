FROM mustafatekeli/qtbuilder:amd64-rpi-cross-compile-base-5.12.3-1

ARG JARVIS_HOST
ARG QT_VERSION=5.12.3
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

RUN echo ${QT_MODULES} && mkdir ${QT_MODULES} \
	&& cd ${QT_MODULES} \
	&& git clone git://code.qt.io/qt/qtdeclarative.git -b ${QT_VERSION} \
	&& git clone git://code.qt.io/qt/qtquickcontrols.git -b ${QT_VERSION} \
	&& git clone git://code.qt.io/qt/qtquickcontrols2.git -b ${QT_VERSION} \
	&& git clone git://code.qt.io/qt/qtmultimedia.git -b ${QT_VERSION} \
	&& git clone git://code.qt.io/qt/qtsvg.git -b ${QT_VERSION} \
	&& git clone git://code.qt.io/qt/qtgraphicaleffects.git -b ${QT_VERSION}

RUN cd ${QT_MODULES}/qtdeclarative \
	&& ${HOST_BINS}/bin/qmake \
	&& make -j4 \
	&& make install

RUN cd ${QT_MODULES}/qtquickcontrols \
	&& ${HOST_BINS}/bin/qmake \
	&& make -j4 \
	&& make install

RUN cd ${QT_MODULES}/qtquickcontrols2 \
	&& ${HOST_BINS}/bin/qmake \
	&& make -j4 \
	&& make install

RUN cd ${QT_MODULES}/qtmultimedia \
	&& ${HOST_BINS}/bin/qmake \
	&& make -j4 \
	&& make install 

RUN cd ${QT_MODULES}/qtsvg \
	&& ${HOST_BINS}/bin/qmake \
	&& make -j4 \
	&& make install 

RUN cd ${QT_MODULES}/qtgraphicaleffects \
	&& ${HOST_BINS}/bin/qmake \
	&& make -j4 \
	&& make install 

# copy over the ssh profile to connect to rpi
RUN rm -rf /root/.ssh/id_rsa
COPY .ssh/id_rsa /root/.ssh/id_rsa
# make sure your domain is accepted
RUN touch /root/.ssh/known_hosts
RUN ssh-keyscan ${JARVIS_HOST} >> /root/.ssh/known_hosts
