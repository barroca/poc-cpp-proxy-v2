FROM ubuntu:xenial

ADD resources/docker/sources.list /etc/apt/sources.list

RUN apt-get update
RUN apt-get install -y libmicrohttpd-dev libboost-dev build-essential libboost-system-dev libboost-thread-dev
