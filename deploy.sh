#!/bin/bash

docker-compose up -d

echo 'y' | docker image prune