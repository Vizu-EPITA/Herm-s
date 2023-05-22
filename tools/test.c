#include "hash_table.h"
#include <stdio.h> 
#include <stdlib.h>

int main()
{
	HashTable *Ht = create_table(30);

	ht_insert(Ht, "https://en.wikipedia.org/1", 10);
	ht_insert(Ht, "https://en.wikipedia.org/2", 11);
	ht_insert(Ht, "https://en.wikipedia.org/3", 12);
	ht_insert(Ht, "https://en.wikipedia.org/4", 13);
	ht_insert(Ht, "https://en.wikipedia.org/5", 14);
	ht_insert(Ht, "https://en.wikipedia.org/6", 15);
	ht_insert(Ht, "https://en.wikipedia.org/7", 16);
	ht_insert(Ht, "https://en.wikipedia.org/8", 17);
	ht_insert(Ht, "https://en.wikipedia.org/9", 18);
	ht_insert(Ht, "https://en.wikipedia.org/10", 19);
	ht_insert(Ht, "https://en.wikipedia.org/11", 20);
	ht_insert(Ht, "https://en.wikipedia.org/12", 21);
	ht_insert(Ht, "https://en.wikipedia.org/13", 22);
	ht_insert(Ht, "https://en.wikipedia.org/14", 23);
	ht_insert(Ht, "https://en.wikipedia.org/15", 24);
	ht_insert(Ht, "https://en.wikipedia.org/16", 25);
	ht_insert(Ht, "https://en.wikipedia.org/17", 26);
	ht_insert(Ht, "https://en.wikipedia.org/18", 27);
	ht_insert(Ht, "https://en.wikipedia.org/19", 28);
	ht_insert(Ht, "https://en.wikipedia.org/20", 29);
	ht_insert(Ht, "https://en.wikipedia.org/21", 30);
	ht_insert(Ht, "https://en.wikipedia.org/22", 31);
	ht_insert(Ht, "https://en.wikipedia.org/23", 32);
	ht_insert(Ht, "https://en.wikipedia.org/24", 33);
	ht_insert(Ht, "https://en.wikipedia.org/25", 34);
	ht_insert(Ht, "https://en.wikipedia.org/26", 35);
	ht_insert(Ht, "https://en.wikipedia.org/27", 36);
	ht_insert(Ht, "https://en.wikipedia.org/28", 37);
	ht_insert(Ht, "https://en.wikipedia.org/29", 38);
	ht_insert(Ht, "https://en.wikipedia.org/30", 39);
	ht_insert(Ht, "https://en.wikipedia.org/31", 40);
	ht_insert(Ht, "https://en.wikipedia.org/32", 41);
	ht_insert(Ht, "https://en.wikipedia.org/33", 42);
	ht_insert(Ht, "https://en.wikipedia.org/34", 43);
	ht_insert(Ht, "https://en.wikipedia.org/35", 44);
	ht_insert(Ht, "https://en.wikipedia.org/36", 45);
	ht_insert(Ht, "https://en.wikipedia.org/37", 46);
	ht_insert(Ht, "https://en.wikipedia.org/38", 47);
	ht_insert(Ht, "https://en.wikipedia.org/39", 48);
	ht_insert(Ht, "https://en.wikipedia.org/40", 49);
	ht_insert(Ht, "https://en.wikipedia.org/41", 50);
	ht_insert(Ht, "https://en.wikipedia.org/42", 51);
	ht_insert(Ht, "https://en.wikipedia.org/43", 52);
	ht_insert(Ht, "https://en.wikipedia.org/44", 53);
	ht_insert(Ht, "https://en.wikipedia.org/45", 54);
	ht_insert(Ht, "https://en.wikipedia.org/46", 55);
	ht_insert(Ht, "https://en.wikipedia.org/47", 56);
	ht_insert(Ht, "https://en.wikipedia.org/48", 57);
	ht_insert(Ht, "https://en.wikipedia.org/49", 58);
	ht_insert(Ht, "https://en.wikipedia.org/50", 59);
	ht_insert(Ht, "https://en.wikipedia.org/51", 60);
	ht_insert(Ht, "https://en.wikipedia.org/52", 61);
	
	print_table(Ht);

	save_table(Ht, "./pastec");

	HashTable *loaded = load_table("./pastec");

	print_table(loaded);

	printf("%d\n", ht_search(loaded, "https://en.wikipedia.org/52"));
	printf("%d\n", ht_search(loaded, "https://en.wikipedia.org/29"));
	printf("%d\n", ht_search(loaded, "https://en.wikipedia.org/7"));
	
	return 0;
}
