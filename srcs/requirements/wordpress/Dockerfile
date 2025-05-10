FROM debian:bullseye

RUN apt-get update && apt-get upgrade -y && \
    apt-get install -y curl php php-fpm php-mysql mariadb-client

COPY ./wp_conf.sh /

RUN mkdir -p  /var/www/wordpress

RUN chmod +x /wp_conf.sh

RUN sed -i 's#listen = /run/php/php7.4-fpm.sock#listen = 0.0.0.0:9000#' /etc/php/7.4/fpm/pool.d/www.conf

ENTRYPOINt ["./wp_conf.sh"]
