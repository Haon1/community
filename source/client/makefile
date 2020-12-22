TARGET=$(wildcard *.c)
OBJ=$(patsubst %.c,%.o,$(TARGET))
DEST=client
LIB=-L./ -lfont -lm -lpthread
CC=arm-linux-gcc

$(DEST):$(OBJ)
	$(CC) $(TARGET) -o $(DEST) $(LIB)
	$(RM) $(OBJ)
