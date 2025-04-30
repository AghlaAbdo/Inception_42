#!/bin/bash

curl -O https://raw.githubusercontent.com/wp-cli/builds/gh-pages/phar/wp-cli.phar
chmod +x wp-cli.phar
mv wp-cli.phar /usr/local/bin/wp
cd /var/www/wordpress

#chmod 755  -R /var/www/wordpress
#chown -R www-data:www-data /var/www/wordpress

wp core download --allow-root
wp core config --dbhost=mariadb:3306 --dbname="$DB_NAME" --dbuser="$DB_USER" --dbpass="$DB_PASS" --allow-root
wp core install --url="$DOMAIN_NAME" --title="Expensive" --admin_user="$WP_ADMIN" --admin_password="$WP_ADMIN_PASS" --admin_email="admin@gmail.com" --allow-root

chmod 755  -R /var/www/wordpress
chown -R www-data:www-data /var/www/wordpress

apt-get install php-xml -y

wp user create "$WP_USR_NAME" "$WP_USR_EMAIL" --user_pass="$WP_USR_PASS" --role="$WP_USR_ROLE" --allow-root

wp db check --allow-root

mkdir -p /run/php

/usr/sbin/php-fpm7.4 -F
