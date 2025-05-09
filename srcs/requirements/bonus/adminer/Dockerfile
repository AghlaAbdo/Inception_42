FROM debian:bullseye

RUN apt-get update && apt-get upgrade -y && \
    apt-get install -y php wget

RUN mkdir -p /var/www/adminer

WORKDIR /var/www/adminer

RUN wget -O index.php https://github.com/vrana/adminer/releases/download/v5.2.1/adminer-5.2.1.php && \
    chmod 775 -R .

CMD ["php", "-S", "0.0.0.0:6060", "-t", "/var/www/adminer"]