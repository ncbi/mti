CC=gcc
CFLAGS= -O -D_FILE_OFFSET_BITS=64
AR = ar

# Berkeley DB Information

DB_DIR = /nfsvol/nls/tools/berkeley_db/Linux-x86_64/db-4.8.24
DBLIBS = ${DB_DIR}/lib/libdb.a -lpthread
LLIBS = ./btree_cotrel_query.a

INC = -I. -I${DB_DIR}/include

OBJS = btree_cotrel_query.a MTI.Linux createBtree

all:	${OBJS}

clean:
	rm -f *.o *.a *.qof *.so *% *.bck core ${OBJS}

btree_cotrel_query.a:	btree_query.c
		${CC} ${CFLAGS} -c btree_query.c  ${INC}
		/usr/bin/ar ru btree_cotrel_query.a btree_query.o

createBtree:	createBtree.c
	${CC} ${CFLAGS} ${INC} -o createBtree createBtree.c ${DBLIBS}

MTI.Linux:	MTI.o btree_cotrel_query.a validate.o cluster.o display.o \
		medium_filtering.o params.o parseMARC.o \
		finalFiltering.o context.o sentences.o
	${CC} ${CFLAGS} ${INC} MTI.o validate.o cluster.o display.o \
	medium_filtering.o params.o parseMARC.o context.o sentences.o \
	finalFiltering.o ${LLIBS} ${DBLIBS} -o MTI.Linux

MTI.o:	MTI.c  defaults.h M_terms.h newMain.h myLocal.h \
	 badLeafList.h activeJournal.h ageLookFor.h SA_Labels.h PTs.h sents.h
	${CC} -c ${INC} MTI.c

validate.o:	validate.c M_terms.h newList.h
	${CC} -c ${INC} validate.c

cluster.o:	cluster.c M_terms.h
	${CC} -c ${INC} cluster.c

finalFiltering.o:	finalFiltering.c M_terms.h gen_spec_list.h
	${CC} -c ${INC} finalFiltering.c

display.o:	display.c finalFiltering.c M_terms.h ageLookFor.h newMain.h
	${CC} -c ${INC} display.c

sentences.o:	sentences.c finalFiltering.c M_terms.h ageLookFor.h newMain.h sents.h PTs.h
	${CC} -c ${INC} sentences.c

medium_filtering.o:	medium_filtering.c M_terms.h
	${CC} -c ${INC} medium_filtering.c

params.o:	params.c M_terms.h
	${CC} -c ${INC} params.c

parseMARC.o:	parseMARC.c M_terms.h
	${CC} -c ${INC} parseMARC.c

context.o:	context.c defaults.h
	${CC} -c ${INC} context.c

install:	${OBJS}
	cp ${OBJS} ../bin
