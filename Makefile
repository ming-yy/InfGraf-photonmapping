# Definir el compilador y las opciones de compilación
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++20

# Definir el ejecutable
TARGET = main

# Buscar todos los archivos .cpp en el directorio actual
SRCS = $(wildcard *.cpp)

# Generar los nombres de los archivos .o correspondientes
OBJS = $(SRCS:.cpp=.o)

# Regla por defecto que se ejecuta cuando solo escribes `make`
all: $(TARGET)

# Regla para construir el ejecutable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Regla para compilar cada archivo .cpp a .o
%.o: %.cpp matriz.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regla para compilar cada archivo .cpp a .o
main.o: main.cpp matriz.h
	$(CXX) $(CXXFLAGS) -c $< -o $@
	
# Regla para limpiar los archivos generados
clean:
	del /Q *.o $(TARGET).exe
