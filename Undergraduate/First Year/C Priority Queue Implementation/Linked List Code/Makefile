CC=gcc
CFLAGS=-I.
DEPS = linked_list.h
OBJ = Client_app.o linked_list.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

Linked_List: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)
