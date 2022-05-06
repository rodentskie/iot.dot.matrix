#!/bin/bash

docker rm iot-api -f
docker image rm iotdotmatrix_api:latest

docker-compose up -d

echo 'y' | docker image prune