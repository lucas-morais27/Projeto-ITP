PARAMS = -g -W -pedantic
all: ocma.js perolaNegra bot_teste
	node ocma perolaNegra bot_teste
perolaNegra: perolaNegra.c
	gcc perolaNegra.c -o perolaNegra $(PARAMS)
bot_teste: bot_teste.c
	gcc bot_teste.c -o bot_teste $(PARAMS)