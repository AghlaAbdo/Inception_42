#!/bin/bash

REQUIRED_VARS=(DB_NAME DB_USER DB_PASS DB_ROOT_PASS DOMAIN_NAME WP_BLOG_TITLE WP_ADMIN WP_ADMIN_PASS WP_ADMIN_EMAIL WP_USR_NAME WP_USR_EMAIL WP_USR_PASS WP_USR_ROLE)

for var in "${REQUIRED_VARS[@]}"; do
  if [ -z "${!var}" ]; then
    echo "❌ Missing environment variable: $var"
    exit 1
  fi
done

apt-get install php-xml -y
curl -O https://raw.githubusercontent.com/wp-cli/builds/gh-pages/phar/wp-cli.phar
chmod +x wp-cli.phar
mv wp-cli.phar /usr/local/bin/wp
cd /var/www/wordpress
chown -R www-data:www-data /var/www/wordpress

wp core download --allow-root && \
wp config create --dbhost=mariadb:3306 --dbname="$DB_NAME" --dbuser="$DB_USER" --dbpass="$DB_PASS" --allow-root && \
wp config set WP_REDIS_HOST 'redis' --add --allow-root && \
wp config set WP_REDIS_PORT '7070' --add --allow-root && \
wp core install --url="$DOMAIN_NAME" --title=$WP_BLOG_TITLE --admin_user="$WP_ADMIN" --admin_password="$WP_ADMIN_PASS" --admin_email=$WP_ADMIN_EMAIL --allow-root && \
wp db check --allow-root && \
wp plugin install redis-cache --activate --allow-root && \
wp user create "$WP_USR_NAME" "$WP_USR_EMAIL" --user_pass="$WP_USR_PASS" --role="$WP_USR_ROLE" --allow-root && \
wp redis enable --allow-root

chmod 775  -R /var/www/*
chown -R www-data:www-data /var/www/wordpress/*

mkdir -p /run/php

/usr/sbin/php-fpm7.4 -F
