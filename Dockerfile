FROM debian:12.9-slim

RUN apt-get update \
    && apt install -y cmake g++

WORKDIR /home/avito
 
COPY . /home/avito/

RUN cmake -Bbuild \
    && cmake --build build

ENTRYPOINT [ "bash" ]
