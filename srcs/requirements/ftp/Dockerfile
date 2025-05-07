FROM debian:bullseye

RUN apt-get update && apt-get upgrade -y

RUN apt-get install vsftpd -y

RUN sed -i 's/^#write_enable=YES/write_enable=YES/' /etc/vsftpd.conf

RUN echo "pasv_enable=YES" >> /etc/vsftpd.conf && \
    echo "pasv_min_port=30000" >> /etc/vsftpd.conf && \
    echo "pasv_max_port=30100" >> /etc/vsftpd.conf && \
    echo "pasv_address=165.227.65.15" >> /etc/vsftpd.conf && \
    echo "chroot_local_user=YES" >> /etc/vsftpd.conf && \
    echo "allow_writeable_chroot=YES" >> /etc/vsftpd.conf

RUN useradd -m -d /var/www/wordpress -s /bin/bash aghla && \
echo "aghla:passit" | chpasswd && \
usermod -aG www-data aghla

RUN service vsftpd start

CMD ["vsftpd", "/etc/vsftpd.conf"]

# CMD ["bash"]