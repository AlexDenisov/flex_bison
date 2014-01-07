all: symbol_table

symbol_table: lexer
	clang lexer.c main.c -o symbol_table

lexer: 
	flex -o lexer.c lexer.l

clean:
	rm -f lexer.c symbol_table

