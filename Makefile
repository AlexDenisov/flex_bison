all: parser lexer
	CC lexer.c parser.c

lexer:
	flex -o lexer.c lexer.l

parser:
	bison -d -o parser.c parser.y

clean:
	rm -f parser.[h,c]
	rm -f lexer.c
