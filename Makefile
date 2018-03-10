all:
	clear
	gcc -std=c99 -pthread -o merge_sort merge_sort.c

test:
	clear
	gcc -std=c99 -pthread -o merge_sort merge_sort.c
	./merge_sort 
	
clean:
	clear
	rm merge_sort
