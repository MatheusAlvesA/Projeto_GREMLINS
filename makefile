All:
	@echo "Compilando GREMLINS..."
	@g++ -Wall -std=c++11 src/main.cpp -I include -o bin/GREMLINS
clean:
	@echo "Limpando..."
	@rm bin/GREMLINS
