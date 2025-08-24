# Inception 🚀

This repository contains my solution for the **Inception** project at 42 School, a comprehensive dive into Docker, virtualization, and system administration. The goal of this project is to set up a small infrastructure composed of different services using Docker Compose.

The entire project is containerized, demonstrating a robust understanding of microservices architecture and deployment.

---

## Core Project Overview

The main part of the Inception project involves creating a multi-service Docker environment. This typically includes:

- **Nginx:** As a web server and reverse proxy.
- **WordPress:** A popular content management system.
- **MariaDB:** A robust relational database management system to support WordPress.

All services are meticulously configured to communicate within their respective Docker containers, showcasing secure and efficient network management.

---

## Bonus Features ✨

Beyond the core requirements, this project includes several exciting bonus services, demonstrating a deeper exploration into various aspects of web development and system administration:

1. **Static Page: Age Calculator App 🎂**  
   A simple static web page hosted within its own Docker container, providing an interactive Age Calculator application. This demonstrates the ability to serve custom front-end applications alongside the main services.

2. **Adminer 🗄️**  
   Adminer is a full-featured database management tool accessible via a web interface. It's deployed as a separate service, allowing for easy management and interaction with the MariaDB database used by WordPress and other potential services.

3. **FTP Server (vsftpd) 📁**  
   An FTP (File Transfer Protocol) server is included, enabling secure file transfers to and from the WordPress volume. This is crucial for content management, backups, and direct access to the website's files. The vsftpd (Very Secure FTP Daemon) is used for its emphasis on security and performance.

4. **Redis (In-Memory Data Structure Store) ⚡**  
   Redis is integrated as an in-memory data structure store, often used for caching and session management to improve the performance of web applications like WordPress. This demonstrates an understanding of optimizing application responsiveness and handling transient data efficiently.

5. **Extra Service: IRC Server (Custom C++98 Implementation) 💬**  
   A significant bonus feature, this project includes a custom-built IRC (Internet Relay Chat) server. This server is not based on existing solutions but is developed from scratch in C++98, showcasing strong foundational programming skills and an in-depth understanding of network protocols. This part was also a separate project within the 42 curriculum, highlighting its complexity and pedagogical value. It provides a real-time communication platform integrated into the Docker ecosystem.

---

## Technologies Used 🛠️

- **Docker**: Containerization platform.
- **Docker Compose**: Tool for defining and running multi-container Docker applications.
- **Nginx**: Web server and reverse proxy.
- **WordPress**: Content Management System.
- **MariaDB**: Relational Database Management System.
- **Adminer**: Database management tool.
- **vsftpd**: FTP Server.
- **Redis**: In-memory data store.
- **C++98**: For the custom IRC server implementation.

---

## How to Use 🚀

1. **Clone the repository:**

    ```bash
    git clone https://github.com/AghlaAbdo/Inception_42
    cd Inception_42
    ```

2. **Build and run the containers:**

   Make sure you have `docker` and `docker compose` installed, and simly run:

    ```bash
    make
    ```

3. **Access the services:**

    - **Wordpress:**  `https://localhost:443`

    - **Adminer:** You need to add the following line in your `/etc/hosts` file:
        ```bash
        127.0.0.1  adminer.aaghla.42.fr
        ```
        Then access it through `adminer.aaghla.42.fr`.

    - **Age Calculator:** You need to add the following line as well in your `/etc/hosts` file:
        ```bash
        127.0.0.1  app.aaghla.42.fr
        ```
        Then access it through `app.aaghla.42.fr`.

    - **FTP Serverv:** Connect via an FTP client to `localhost` specifing the password `FTP_PASS` in `./src/.env` file.
    
    - **IRC Server:** Connect via an IRC client to `localhost` port `6667`, password: `IRC_PASS` in `./src/.env`.

4. **To stop and remove containers:**

   Simply run:
   ```bash
   make fclean
   ```
