FROM debian:bullseye

RUN apt-get update && apt-get upgrade -y && \
    apt-get install -y mariadb-server mariadb-client netcat

COPY ./conf/mdb-conf.sh /mdb-conf.sh

RUN chmod +x /mdb-conf.sh

ENTRYPOINT ["./mdb-conf.sh"]
