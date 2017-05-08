all:
	svn up
	cd Caravel && make
	cd Command && make

clean:
	cd Caravel && make clean
	cd Command && make clean

main:
	cd Main && ./TestMain
