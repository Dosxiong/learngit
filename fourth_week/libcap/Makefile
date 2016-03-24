#****************************************************************************

# Copyright        :   EmbedWay 恒为科技武汉研发中心 By:Sugarguo

# Author           :   sugarguo

# Date             :   2016-03-01

# Version          :   Link静态链接库

# Description 	   :   DoubleLinkTestMakefile

#

#****************************************************************************/

CC = gcc

AR = ar rcs

RM = rm -fr *.o *.a *.out

ip_recombination : pcap.o libDoubleLink.a 
	$(CC)	-o	ip_recombination pcap.o libDoubleLink.a -lpcap -lpthread

pcap.o : pcap.c DoubleLink.h 
	$(CC)	-c	pcap.c

libDoubleLink.a : DoubleLink.o
	$(AR)		libDoubleLink.a DoubleLink.o

DoubleLink.o :  DoubleLink.c
	$(CC)	-c	DoubleLink.c


.PHONY: clean
clean :
	$(RM)		ip_recombination
