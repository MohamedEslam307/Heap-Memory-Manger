# Targets
all: static dynamic

static:
	gcc -c ./HMM/hmm.c ./DoubleLinkedList/DoubleLinkedList.c 
	ar rcs libhmm.a hmm.o
	@echo "Static library libhmm.a created."

dynamic:
	gcc -o libhmm.so -fPIC -shared ./HMM/hmm.c ./DoubleLinkedList/DoubleLinkedList.c
	@echo "Dynamic library libhmm.so created."

clean:
	rm -f hmm.o libhmm.a libhmm.so

.PHONY: all static dynamic clean
