#!/bin/bash

docker rm iot-app -f
docker rm iot-api -f
docker rm mongo -f

docker image rm iotdotmatrix_api:latest
docker image rm iotdotmatrix_app:latest

docker-compose up -d

echo 'y' | docker image prune