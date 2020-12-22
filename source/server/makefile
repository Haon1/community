TARGET=$(wildcard *.c)
OBJ=$(patsubst %.c,%.o,$(TARGET))
DEST=server
LIB=-lpthread
CC=gcc

$(DEST):$(OBJ)
	$(CC) $(TARGET) -o $(DEST) $(LIB)
	$(RM) $(OBJ)
