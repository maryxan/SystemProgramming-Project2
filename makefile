OBJS = main.o namedpipes.o list.o avl.o preprocessing.o commandsfunctions.o date.o 
EXEC = diseaseAggregator
GCCC = gcc -c
GCCO = gcc -o
LIBS = -lm 

$(EXEC): $(OBJS)
	$(GCCO) $(EXEC) $(OBJS) $(LIBS) -g

main.o: main.c
	$(GCCC) main.c

namedpipes.o: namedpipes.c
	$(GCCC) namedpipes.c

list.o: list.c
	$(GCCC) list.c	

avl.o: avl.c
	$(GCCC) avl.c

preprocessing.o: preprocessing.c
	$(GCCC) preprocessing.c

commandsfunctions.o: commandsfunctions.c
	$(GCCC) commandsfunctions.c

date.o: date.c
	$(GCCC) date.c	

clean:
	rm -rf $(OBJS) $(EXEC)