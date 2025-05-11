FROM debian:bullseye

RUN apt-get update && apt-get upgrade -y && \
    apt-get install -y nginx openssl

RUN mkdir -p /etc/nginx/ssl

RUN openssl req -x509 -nodes -days 365 -newkey rsa:2048 \
    -out /etc/nginx/ssl/certificate.crt \
    -keyout /etc/nginx/ssl/priv_key.key \
    -subj "/C=MA/ST=Khouribga/O=1337/OU=1337/CN=aaghla.42.fr/UID=thedon"

COPY ./conf/nginx.conf /etc/nginx/nginx.conf

RUN mkdir -p /var/www/wordpress

RUN chown -R www-data:www-data /var/www/wordpress


CMD ["nginx", "-g", "daemon off;"]


 