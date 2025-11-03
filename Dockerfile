FROM ubuntu:24.04

ARG DEBIAN_FRONTEND=noninteractive
ARG BUILDROOT_REF=master

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential git make cmake gcc g++ ccache \
    python3 python3-pip python3-setuptools python3-venv python3-dev \
    bzip2 cpio unzip rsync bc flex bison gettext autopoint \
    libncurses-dev libssl-dev pkg-config wget curl file sudo \
    ca-certificates gnupg nano \
    pkg-config && \
    rm -rf /var/lib/apt/lists/*

RUN git clone --depth 1 --branch ${BUILDROOT_REF} https://gitlab.com/buildroot.org/buildroot.git /opt/buildroot || \
    (git clone https://gitlab.com/buildroot.org/buildroot.git /opt/buildroot && cd /opt/buildroot && git checkout ${BUILDROOT_REF})

WORKDIR /opt/buildroot

# (опционально) место для монтирования
VOLUME [ "/br-ext-buildroot", "/opt/buildroot/output/images", "/opt/buildroot/dl", "/ccache" ]

CMD ["/bin/bash"]

