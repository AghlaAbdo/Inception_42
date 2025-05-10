FROM debian:bullseye

RUN apt-get update && apt-get upgrade -y && \
    apt-get install -y redis


CMD ["redis-server", "--bind", "0.0.0.0", "--port", "7070", "--protected-mode", "no"]