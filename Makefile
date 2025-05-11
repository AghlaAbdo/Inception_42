


up:
	mkdir -p /home/aghla/data/wordpress
	mkdir -p /home/aghla/data/mariadb
	docker compose -f ./srcs/docker-compose.yml up --build
down:
	docker compose -f ./srcs/docker-compose.yml down

# exec:
