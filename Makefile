NOM=projet1
OUTFILE=${NOM}
SOURCES=src/ads1115_rpi.c src/${NOM}.c
FLAGS=-Wall -lpigpio

all: release

release: src/${NOM}.c
	gcc ${SOURCES} ${FLAGS} -o ${OUTFILE}

debug: src/${NOM}.c
	gcc -g ${SOURCES} ${FLAGS} 

clean: 
	rm ${OUTFILE} 
