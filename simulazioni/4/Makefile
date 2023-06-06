NAME = SignalProxy

all:
ifdef FILE
	@if [[ ! -f $(FILE) ]]; then \
		echo "start" > $(FILE); \
	fi
endif
	@gcc main.c -o $(NAME)

clean:
	@if [[ -f $(FILE) ]]; then \
		rm $(FILE); \
	fi
	@if [[ -f $(NAME) ]]; then \
		rm $(NAME); \
	fi
