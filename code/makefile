all:hello generic string

hello: hello.cc
	g++ $^ -o $@ -lpthread /usr/local/lib/libredis++.a /usr/lib/x86_64-linux-gnu/libhiredis.a

generic:generic.cc
	g++ $^ -o $@ -lpthread /usr/local/lib/libredis++.a /usr/lib/x86_64-linux-gnu/libhiredis.a

string:string.cc
	g++ $^ -o $@ -lpthread /usr/local/lib/libredis++.a /usr/lib/x86_64-linux-gnu/libhiredis.a

.PHONY: clean
clean:
	rm -rf hello