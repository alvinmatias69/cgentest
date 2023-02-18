##
# CgenTest
#
# @file
# @version 0.1

cgentest: main.c cli.o logger.o cgentest.o readtags.o ctags_helper.o mapper.o mustach.o mustach-wrap.o mustach-cjson.o util.o
	@gcc -o cgentest cli.o logger.o cgentest.o readtags.o ctags_helper.o mapper.o mustach.o mustach-wrap.o mustach-cjson.o util.o main.c -lcjson -Wall -ggdb3

cli.o: cli.h cli.c
	@gcc -o cli.o -c cli.c

logger.o: logger.h logger.c
	@gcc -o logger.o -c logger.c

cgentest.o: cgentest.h cgentest.c
	@gcc -o cgentest.o -c cgentest.c

ctags_helper.o: ctags_helper.h ctags_helper.c
	@gcc -o ctags_helper.o -c ctags_helper.c

readtags.o: ./libs/libreadtags/readtags.h ./libs/libreadtags/readtags.c
	@gcc -o readtags.o -c ./libs/libreadtags/readtags.c

mustach.o: ./libs/mustach/mustach.h ./libs/mustach/mustach.c
	@gcc -o mustach.o -c ./libs/mustach/mustach.c

mustach-wrap.o: ./libs/mustach/mustach-wrap.h ./libs/mustach/mustach-wrap.c
	@gcc -o mustach-wrap.o -c ./libs/mustach/mustach-wrap.c

mustach-cjson.o: ./libs/mustach/mustach-cjson.h ./libs/mustach/mustach-cjson.c
	@gcc -o mustach-cjson.o -c ./libs/mustach/mustach-cjson.c

mapper.o: mapper.h mapper.c
	@gcc -o mapper.o -c mapper.c -lcjson

util.o: util.h util.c
	@gcc -o util.o -c util.c

# end
