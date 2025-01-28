FROM debian:12.9-slim

RUN apt-get update \
    && apt install -y cmake g++

WORKDIR /home/avito
 
COPY . /home/avito/

RUN cmake -Bbuild \
    && cmake --build build

# Uncomment if needed
# COPY input.txt /home/avito/build/bin
# COPY output.txt /home/avito/build/bin

ENTRYPOINT [ "bash" ]
