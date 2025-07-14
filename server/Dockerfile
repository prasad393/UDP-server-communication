FROM debian:latest

RUN apt-get update && apt-get install -y g++ cmake

WORKDIR /app

COPY ../ .  

RUN cmake . && make

CMD ["./udp_receiver"]
