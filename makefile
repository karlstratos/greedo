files = $(subst .cpp,.o, $(shell /bin/ls *.cpp) $(shell /bin/ls util/*.cpp))

main: $(files)
	g++ -I util/eigen-eigen-ffa86ffb5570 -Wall -g -O3 -o greedo $(files)

%.o: %.cpp
	g++ -I util/eigen-eigen-ffa86ffb5570 -Wall -g -O3 -o $@ -c $<

clean:
	rm -rf util/*.o *.o greedo *.debug
