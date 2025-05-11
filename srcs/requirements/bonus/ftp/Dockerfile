FROM debian:bullseye

RUN apt-get update && apt-get upgrade -y

RUN apt-get install vsftpd -y

RUN sed -i 's/^#write_enable=YES/write_enable=YES/' /etc/vsftpd.conf

RUN echo "pasv_enable=YES" >> /etc/vsftpd.conf && \
    echo "pasv_min_port=30000" >> /etc/vsftpd.conf && \
    echo "pasv_max_port=30100" >> /etc/vsftpd.conf && \
    echo "chroot_local_user=YES" >> /etc/vsftpd.conf && \
    echo "allow_writeable_chroot=YES" >> /etc/vsftpd.conf
    # echo "pasv_address=165.227.65.15" >> /etc/vsftpd.conf && \

RUN mkdir -p /var/run/vsftpd/empty && \
    chown root:root /var/run/vsftpd/empty && \
    chmod 755 /var/run/vsftpd/empty

COPY ./tools/ftp_conf.sh /ftp_conf.sh

RUN chmod +x  /ftp_conf.sh

CMD ["./ftp_conf.sh"]