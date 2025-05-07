#!/bin/bash

apt-get install php-xml -y
curl -O https://raw.githubusercontent.com/wp-cli/builds/gh-pages/phar/wp-cli.phar
chmod +x wp-cli.phar
mv wp-cli.phar /usr/local/bin/wp
cd /var/www/wordpress




wp core download --allow-root && \
wp config create --dbhost=mariadb:3306 --dbname="$DB_NAME" --dbuser="$DB_USER" --dbpass="$DB_PASS" --allow-root && \
wp config set WP_REDIS_PATH '/var/run/redis/redis.sock' --add --allow-root && \
wp config set WP_REDIS_SCHEME 'unix' --add --allow-root && \
wp core install --url="$DOMAIN_NAME" --title="Expensive" --admin_user="$WP_ADMIN" --admin_password="$WP_ADMIN_PASS" --admin_email="admin@gmail.com" --allow-root && \
wp db check --allow-root && \

chmod 775  -R /var/www/*
chown -R www-data:www-data /var/www/wordpress/*

apt-get install redis-server -y && \
sed -i '/^bind 127\.0\.0\.1 ::1$/{
c\
unixsocket /var/run/redis/redis.sock\
unixsocketperm 777
}' /etc/redis/redis.conf && \
sed -i -e '$a\save ""' /etc/redis/redis.conf && \
service redis-server restart && \
usermod -aG redis www-data && \

wp plugin install redis-cache --activate --allow-root && \
wp redis enable --allow-root

apt-get install wget -y && \
mkdir -p adminer && \
cd adminer && \
wget -O index.php https://github.com/vrana/adminer/releases/download/v5.2.1/adminer-5.2.1.php
chmod 775 -R .


wp user create "$WP_USR_NAME" "$WP_USR_EMAIL" --user_pass="$WP_USR_PASS" --role="$WP_USR_ROLE" --allow-root

chown -R www-data:www-data /var/www/wordpress/*

mkdir -p /run/php

/usr/sbin/php-fpm7.4 -F
