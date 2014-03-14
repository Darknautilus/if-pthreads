# ======================
# INSA Lyon, Département Informatique
# TP 3IF : Programmation concurrente
# Binome : B3229
# ======================

MODE=DEBUG
CC=gcc
CFLAGS=
LDFLAGS= -lm -lpthread
PATHEXEC=bin/
EXEC=ifpthreads generator
TARGET=$(addprefix $(PATHEXEC), $(EXEC))

ifeq ($(MODE),DEBUG)
	CFLAGS:=$(CFLAGS) -g
endif

SRC=generator.c ifpthreads.c
OBJ=$(SRC:.cpp=.o)

all: $(TARGET)

$(PATHEXEC)generator: generator.o
	@mkdir -p $(PATHEXEC)
	$(CC) -o $@ $^ $(LDFLAGS)

$(PATHEXEC)ifpthreads: ifpthreads.o
	@mkdir -p $(PATHEXEC)
	$(CC) -o $@ $^ $(LDFLAGS)

#$(TARGET): $(OBJ)
#	@mkdir -p $(PATHEXEC)
#	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

#run: $(TARGET)
#	@./$(TARGET)

#test: $(TARGET)

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf $(PATHEXEC)
