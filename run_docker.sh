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

echo "Starting Buildroot container..."
docker run -it --rm \
  -v "$(pwd)/out":/opt/buildroot/output \
  -w /opt/buildroot \
  my-ubuntu-buildroot:latest

