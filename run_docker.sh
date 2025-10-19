#!/bin/bash
set -e
	
IMAGE_NAME="my-ubuntu-buildroot:latest"

if [[ "$1" == "--rebuild" ]]; then
    echo "Rebuilding Docker image..."
    docker build --no-cache -t "$IMAGE_NAME" .
else
    if ! docker image inspect "$IMAGE_NAME" > /dev/null 2>&1; then
	echo "Building Docker image..."
        docker build -t "$IMAGE_NAME" .
    else
        echo "Image $IMAGE_NAME already"
    fi
fi

mkdir -p out
mkdir -p out/images

echo "Starting Buildroot container..."
docker run -it --rm \
  -v "$(pwd)/out/images":/opt/buildroot/output/images \
  -w /opt/buildroot \
  $IMAGE_NAME \

