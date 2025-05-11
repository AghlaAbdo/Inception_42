FROM debian:bullseye

RUN apt-get update && apt-get upgrade -y && \
    apt-get install -y build-essential

COPY ./tools/IRC /IRC

COPY ./tools/runIRC.sh /runIRC.sh

RUN chmod +x runIRC.sh

CMD ["./runIRC.sh"]