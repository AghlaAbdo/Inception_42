#!/bin/bash

if [ -z "$FTP_PASS" ]; then
    echo "❌ Missing environment variable: FTP_PASS"
    exit 1
fi

if ! id -u aghla > /dev/null 2>/dev/null; then
    useradd -m -d /var/www/wordpress -s /bin/bash aghla && \
    echo "aghla:$FTP_PASS" | chpasswd && \
    usermod -aG www-data aghla
fi

vsftpd /etc/vsftpd.conf