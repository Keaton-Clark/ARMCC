src = $(wildcard ./src/*.c)
obj = $(src:.cpp=.o)

CS219-Project: $(obj)
	    gcc -o $@ $^

