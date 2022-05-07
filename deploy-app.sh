#!/bin/bash

docker rm iot-app -f
docker image rm iotdotmatrix_app:latest

docker-compose up -d --no-deps --build app

echo 'y' | docker image prune