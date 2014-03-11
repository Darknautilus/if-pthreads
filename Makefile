# ======================
# INSA Lyon, Département Informatique
# TP 3IF : Programmation concurrente
# Binome : B3229
# ======================

MODE=DEBUG
CC=gcc
CFLAGS=
LDFLAGS=
PATHEXEC=bin/
EXEC=ifpthreads
TARGET=$(addprefix $(PATHEXEC), $(EXEC))

ifeq ($(MODE),DEBUG)
	CFLAGS:=$(CFLAGS) -g
endif

SRC=generator.c
OBJ=$(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(PATHEXEC)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

run: $(TARGET)
	@./$(TARGET)

#test: $(TARGET)

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf $(PATHEXEC)
