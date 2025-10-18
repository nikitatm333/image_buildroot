# Dockerfile — Ubuntu с Buildroot под root
FROM ubuntu:24.04

ARG DEBIAN_FRONTEND=noninteractive
ARG BUILDROOT_REF=master

# Устанавливаем необходимые пакеты
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential git make cmake gcc g++ ccache \
    python3 python3-pip python3-setuptools python3-venv python3-dev \
    bzip2 cpio unzip rsync bc flex bison gettext autopoint \
    libncurses-dev libssl-dev pkg-config wget curl file sudo \
    ca-certificates gnupg \
    pkg-config && \
    rm -rf /var/lib/apt/lists/*

# Клонируем Buildroot
RUN git clone --depth 1 --branch ${BUILDROOT_REF} https://gitlab.com/buildroot.org/buildroot.git /opt/buildroot || \
    (git clone https://gitlab.com/buildroot.org/buildroot.git /opt/buildroot && cd /opt/buildroot && git checkout ${BUILDROOT_REF})

# Делаем рабочую директорию
WORKDIR /root

# Запускаем контейнер с bash
CMD ["/bin/bash"]

