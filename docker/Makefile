.PHONY: all build run clean fclean

all: build

build:
	docker compose -f ./docker-compose.yml up --build -d

clean:
	docker compose -f ./docker-compose.yml down -v

fclean: clean
	docker system prune -a -f 2>/dev/null

restart: clean build
