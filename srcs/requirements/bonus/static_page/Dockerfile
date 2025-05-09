FROM debian:bullseye

RUN apt-get update && apt-get upgrade -y && \
    apt-get install -y nodejs

RUN  mkdir -p /var/www/App

COPY ./src /var/www/App

CMD ["node", "/var/www/App/backend/index.js"]