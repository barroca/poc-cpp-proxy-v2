build: build-docker build-b2f-cpp2

build-docker:
	docker-compose build base

build-b2f-cpp2:
	docker-compose run --rm --workdir=/app/src shell make

clean:
	(cd src/ && make clean)

down:
	docker-compose down

shell:
	@docker-compose run -e SEARCH_HOST=poc-search.test.com.br --rm shell

up:
	docker-compose run -e SEARCH_HOST=poc-search.test.com.br --rm --service-ports web
