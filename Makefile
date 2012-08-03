# compile without optimization
#

CC = gcc
CFLAGS = -std=gnu99 -I/bin/R/R-2.15.0/include -DNDEBUG  -I/usr/local/include -fpic  -g -O2 
OBJS = \
 R_all_finite.o\
 dc_spa.o\
 maxdist.o\
 R_dc_spa.o\
 R_sc_arith.o\
 R_spa.o\
 R_sparseConstraints.o\
 sc_arith.o\
 spa.o\
 sparseConstraints.o

rspa : $(OBJS)
	gcc -std=gnu99 -shared -L/usr/local/lib -o rspa.so $(OBJS)


all_finite.o            :     all_finite.c
	$(CC) $(CFLAGS) -c R_all_finite.c -o R_all_finite.o

dc_spa.o                :     dc_spa.c              
	$(CC) $(CFLAGS) -c dc_spa.c -o dc_spa.o

maxdist.o               :     maxdist.c             
	$(CC) $(CFLAGS) -c maxdist.c -o maxdist.o

R_dc_spa.o              :     R_dc_spa.c            
	$(CC) $(CFLAGS) -c R_dc_spa.c -o R_dc_spa.o

R_sc_arith.o            :     R_sc_arith.c          
	$(CC) $(CFLAGS) -c R_sc_arith.c -o R_sc_arith.o

R_spa.o                 :     R_spa.c               
	$(CC) $(CFLAGS) -c R_spa.c -o R_spa.o

R_sparseConstraints.o   :     R_sparseConstraints.c 
	$(CC) $(CFLAGS) -c R_sparseConstraints.c -o R_sparseConstraints.o

sc_arith.o              :     sc_arith.c            
	$(CC) $(CFLAGS) -c sc_arith.c -o sc_arith.o

spa.o                   :     spa.c                 
	$(CC) $(CFLAGS) -c spa.c -o spa.o

sparseConstraints.o     :     sparseConstraints.c   
	$(CC) $(CFLAGS) -c sparseConstraints.c -o sparseConstraints.o






#gcc -std=gnu99 -I/bin/R/R-2.15.0/include -DNDEBUG  -I/usr/local/include    -fpic  -g -O2  -c dc_spa.c -o dc_spa.o
#gcc -std=gnu99 -I/bin/R/R-2.15.0/include -DNDEBUG  -I/usr/local/include    -fpic  -g -O2  -c dc_spa.c -o dc_spa.o
#gcc -std=gnu99 -I/bin/R/R-2.15.0/include -DNDEBUG  -I/usr/local/include    -fpic  -g -O2  -c maxdist.c -o maxdist.o
#gcc -std=gnu99 -I/bin/R/R-2.15.0/include -DNDEBUG  -I/usr/local/include    -fpic  -g -O2  -c R_dc_spa.c -o R_dc_spa.o
#gcc -std=gnu99 -I/bin/R/R-2.15.0/include -DNDEBUG  -I/usr/local/include    -fpic  -g -O2  -c R_sc_arith.c -o R_sc_arith.o
#gcc -std=gnu99 -I/bin/R/R-2.15.0/include -DNDEBUG  -I/usr/local/include    -fpic  -g -O2  -c R_spa.c -o R_spa.o
#gcc -std=gnu99 -I/bin/R/R-2.15.0/include -DNDEBUG  -I/usr/local/include    -fpic  -g -O2  -c R_sparseConstraints.c -o R_sparseConstraints.o
#gcc -std=gnu99 -I/bin/R/R-2.15.0/include -DNDEBUG  -I/usr/local/include    -fpic  -g -O2  -c sc_arith.c -o sc_arith.o
#gcc -std=gnu99 -I/bin/R/R-2.15.0/include -DNDEBUG  -I/usr/local/include    -fpic  -g -O2  -c spa.c -o spa.o
#gcc -std=gnu99 -I/bin/R/R-2.15.0/include -DNDEBUG  -I/usr/local/include    -fpic  -g -O2  -c sparseConstraints.c -o sparseConstraints.o



