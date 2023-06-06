CC = gcc
TARGET = coda
SOURCE = main.c
DEST ?= .

all:
ifndef DEST
	$(CC) $(SOURCE) -o $(TARGET)
else
	@if [ -d $(DEST) ]; then \
		$(CC) $(SOURCE) -o $(DEST)/$(TARGET); \
	else \
		echo "?ERROR" >& 2; \
	fi
endif
