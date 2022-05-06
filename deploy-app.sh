#!/bin/bash

docker rm iot-app -f
docker image rm iotdotmatrix_app:latest

docker-compose up -d

echo 'y' | docker image prune