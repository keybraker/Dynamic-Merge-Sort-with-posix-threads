all:
	clear
	gcc -std=c99 -pthread -o merge_sort merge_sort.c

threads:
	clear
	gcc -std=c99 -pthread -o merge_sort merge_sort.c
	./merge_sort 

nothreads:
	clear
	gcc -std=c99 -pthread -o merge_sort_no_threads merge_sort_no_threads.c
	./merge_sort_no_threads 
	
clean:
	clear
	rm merge_sort
