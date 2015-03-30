
CCFLAGS = -g -wall

binBud: binBud.c blockList.c block.h
	gcc $(CCFLAGS) -o binBud binBud.c blockList.c block.h
	
blockList.c: blockList.h block.h
	gcc $(CCFLAGS)