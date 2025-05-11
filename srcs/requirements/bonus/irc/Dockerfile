FROM debian:bullseye

RUN apt-get update && apt-get upgrade -y && \
    apt-get install -y build-essential

COPY ./tools/irc_src /IRCSERV

COPY ./tools/runIRC.sh /runIRC.sh

RUN chmod +x runIRC.sh

CMD ["./runIRC.sh"]