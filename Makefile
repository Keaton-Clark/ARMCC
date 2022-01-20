src = $(wildcard ./src/*.c)
obj = $(src:.cpp=.o)

CS219-Project-Part-1: $(obj)
	    gcc -o $@ $^

clean:
	    rm -f $(obj) cpp_repo
