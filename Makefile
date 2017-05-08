all:
	svn up
	cd Caravel && make
	cd Command && make
	cd Server && make

clean:
	cd Caravel && make clean
	cd Command && make clean
	cd Server && make clean

main:
	cd Main && ./TestMain
