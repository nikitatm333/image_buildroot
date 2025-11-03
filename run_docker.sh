#!/bin/bash
set -e

IMAGE_NAME="buildroot-1"

if [[ "$1" == "--rebuild" ]]; then
    echo "Rebuilding Docker image..."
    docker build --no-cache -t "$IMAGE_NAME" .
else
    if ! docker image inspect "$IMAGE_NAME" > /dev/null 2>&1; then
        echo "Building Docker image..."
        docker build -t "$IMAGE_NAME" .
    else
        echo "Image $IMAGE_NAME already exists"
    fi
fi

# Создаём dir для артефактов и кешей
mkdir -p out/images
mkdir -p out/dl
mkdir -p out/ccache

echo "Starting Buildroot container..."

docker run -it --rm --name buildroot-1 \
  --mount type=bind,source="$(pwd)",target=/br-ext-buildroot \
  --mount type=bind,source="$(pwd)/out/images",target=/buildroot/output/images \
  --mount type=bind,source="$(pwd)/out/dl",target=/buildroot/dl \
  --mount type=bind,source="$(pwd)/out/ccache",target=/ccache \
  --env FORCE_UNSAFE_CONFIGURE=1 \
  --env CCACHE_DIR=/ccache \
  -w /buildroot \
  $IMAGE_NAME \
  bash

