FROM mustafatekeli/qtbuilder:qt5.12.1-amd64-rpi-cross-compile

RUN apt-get update && \
	apt-get install -y \
	build-essential \
	curl \
	git \
	cmake \
	python \
	rsync \
	wget \ 
	&& apt-get clean \
	&& rm -rf /var/lib/apt/lists/*

