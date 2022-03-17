PARAMS = -g -W -pedantic
all: ocma.js perolaNegra bot_A bot_B
	node ocma perolaNegra bot_A bot_B
perolaNegra: perolaNegra.c
	gcc perolaNegra.c -o perolaNegra $(PARAMS)
bot_A: bot_A.c
	gcc bot_A.c -o bot_A $(PARAMS)
bot_B: bot_B.c
	gcc bot_B.c -o bot_B $(PARAMS)