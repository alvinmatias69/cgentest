lexer.o: lexer.c lexer.h token.h
	@gcc -o lexer.o -c lexer.c

trie.o: trie.c trie.h token.h
	@gcc -o trie.o -c trie.c

util.o: util.c util.h trie.o
	@gcc -o util.o -c util.c

extractor: lexer.o trie.o util.o main.c
	@gcc -o extractor lexer.o trie.o util.o main.c

trie_driver: trie.c trie_driver.c
	@gcc -o trie_driver trie_driver.c
