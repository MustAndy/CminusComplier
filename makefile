CC = gcc

CFLAGS = -Wall

OBJS = driver.o scanner.o parse_util.o parser.o util.o symbtab.o rd_parser.o s_analyzer.o analyzer.o

main: $(OBJS)
	$(CC) $(CFLAGS) -o main $(OBJS)

driver.o: libs.h analyzer.h util.h analyzer.h parse_util.h s_analyzer.h globals.h rd_parser.h
	$(CC) $(CFLAGS) -c driver.c

scanner.o: libs.h scan.h globals.h
	$(CC) $(CFLAGS) -c scanner.c
	
parser.o: libs.h parse.h scan.h util.h globals.h parse_util.h
	$(CC) $(CFLAGS) -c parser.c
	
symbtab.o: libs.h globals.h util.h parse.h symbol_table.h
	$(CC) $(CFLAGS) -c symbtab.c
	
util.o: util.h globals.h libs.h
	$(CC) $(CFLAGS) -c util.c
		
parse_util.o: parse_util.h libs.h parse.h util.h globals.h
	$(CC) $(CFLAGS) -c parse_util.c

analyzer.o: libs.h util.h globals.h scan.h parse.h analyzer.h parse_util.h symbol_table.h 
	$(CC) $(CFLAGS) -c analyzer.c

s_analyzer.o: globals.h analyzer.h s_analyzer.h
	$(CC) $(CFLAGS) -c s_analyzer.c

rd_parser.o: parse_util.h scan.h parse.h globals.h libs.h parse_util.h
	$(CC) $(CFLAGS) -c rd_parser.c

clean:
	-rm main $(OBJS)
