LEX=flex
YACC=bison
CC=g++
OBJECT=main

$(OBJECT): lex.yy.o yacc.tab.o
	$(CC) lex.yy.o yacc.tab.o -o $(OBJECT)
	@./$(OBJECT)    # 编译后立即运行

lex.yy.o: lex.yy.c yacc.tab.h main.h
	$(CC) -c lex.yy.c

yacc.tab.o: yacc.tab.h main.h
	$(CC) -c yacc.tab.c

yacc.tab.c yacc.tab.h: yacc.y
# bison使用-d参数编译.y文件
	$(YACC) -d yacc.y

lex.yy.c: lex.l
	$(LEX) lex.l

clean:
	@rm -f $(OBJECT) *.o