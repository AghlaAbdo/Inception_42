#!/bin/bash

REQUIRED_VARS=(DB_NAME DB_USER DB_PASS DB_ROOT_PASS)

for var in "${REQUIRED_VARS[@]}"; do
  if [ -z "${!var}" ]; then
    echo "❌ Missing environment variable: $var"
    exit 1
  fi
done

service mariadb start
sleep 3


mariadb -e "CREATE DATABASE IF NOT EXISTS $DB_NAME;"
mariadb -e "CREATE USER IF NOT EXISTS $DB_USER@'%' IDENTIFIED BY '$DB_PASS';"
mariadb -e "GRANT ALL PRIVILEGES ON $DB_NAME.* TO $DB_USER@'%';"
mariadb -e "FLUSH PRIVILEGES;"
#mariadb -e "ALTER USER 'root'@'localhost' IDENTIFIED BY '${DB_ROOT_PASS}';"

mariadb-admin -u root -p"$DB_ROOT_PASS" shutdown
mariadbd-safe  --port=3306 --bind-address=0.0.0.0 --datadir='/var/lib/mysql'
