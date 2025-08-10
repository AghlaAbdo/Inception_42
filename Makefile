
up:
	@mkdir -p $(HOME)/data/wordpress
	@mkdir -p $(HOME)/data/mariadb
	@docker compose -f ./srcs/docker-compose.yml up --build
down:
	@docker compose -f ./srcs/docker-compose.yml down

fclean: down
	@docker rmi $$(docker images -q) 2>/dev/null || true
	@docker volume rm $$(docker volume ls -q) 2>/dev/null || true
	@docker network rm $$(docker network ls -q) 2>/dev/null || true
	@rm -rf /home/aaghla/data

re: down fclean up

# exec:
