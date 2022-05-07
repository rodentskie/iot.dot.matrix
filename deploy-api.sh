#!/bin/bash

docker rm iot-api -f
docker image rm iotdotmatrix_api:latest

docker-compose up -d --no-deps --build api

echo 'y' | docker image prune