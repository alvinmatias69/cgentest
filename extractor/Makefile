lexer.o: lexer.c lexer.h
	@gcc -o lexer.o -c lexer.c

trie.o: trie.c trie.h
	@gcc -o trie.o -c trie.c

util.o: util.c util.h trie.o
	@gcc -o util.o -c util.c
