#Our compiler.
CC = gcc
CFLAGS = -g 
LDFLAGS = -lreadline
#Default rule.

default: xfs-interface

xfs-interface: *.c *.h
	$(CC) $(CFLAGS) *.c -o xfs-interface $(LDFLAGS)
#xfs-interface: labels.o diskUtility.o inode.o interface.o fileSystem.o exception.o
#	$(CC) $(CFLAGS) -o xfs-interface  labels.o diskUtility.o inode.o interface.o fileSystem.o exception.o $(LDFLAGS)

#diskUtility.o: diskUtility.c diskUtility.h exception.h
#	$(CC) $(CFLAGS) -c diskUtility.c

#exception.o: exception.c exception.h
#	$(CC) $(CFLAGS) -c exception.c

#fileSystem.o: fileSystem.c fileSystem.h memOrg.h inode.h diskUtility.h constants.h
#	$(CC) $(CFLAGS) -c fileSystem.c

#inode.o: inode.c inode.h
#	$(CC) $(CFLAGS) -c inode.c

#interface.o: interface.c interface.h inode.h diskUtility.h fileSystem.h 
#	$(CC) $(CFLAGS) -c interface.c

#labels.o: labels.c labels.h
#	$(CC) $(CFLAGS) -c labels.c

clean:
	$(RM) xfs-interface *.o 