# build stage
FROM node:14.17.5-alpine as build-stage
WORKDIR /usr/src/app
COPY package*.json ./

RUN apk update
RUN apk add --no-cache git \
    python \
    make \
    g++

RUN npm ci
COPY . .
RUN npm run build
EXPOSE 4000
ENV NUXT_HOST=0.0.0.0
CMD [ "npm", "start" ]