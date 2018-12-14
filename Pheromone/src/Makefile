include Mk/local.Mk
SUBDIRS+=common
SUBDIRS+=gui
SUBDIRS+=main

OBJS=$(wildcard ../obj/*.o)
LXXLIBS+=-L/usr/local/lib
LXXLIBS+=-lSDL
LXXLIBS+=-lSDL_ttf
LXXLIBS+=-lpthread -g `pkg-config opencv --libs` 

COPY_ETC_CMD=../etc/copy.sh
COPY_ETC_SRC=../etc
COPY_ETC_DST=../bin

phero: all obj
	$(CXX) $(CXXDEFINE) -o ../bin/$@  $(OBJS)  $(CXXFLAGS) $(LXXLIBS) 

all:
	echo $(OPSYS)
	@for i in $(SUBDIRS) ;\
	do \
	echo "making" all "in $(CURRENT_DIR)/$$i..."; \
	$(MAKE) -C $$i all; \
	done

obj: all
	echo "Copy objs"
	@for i in $(SUBDIRS) ;\
	do \
	echo "coping all in $(CURRENT_DIR)/$$i..."; \
	cp $$i/*.o ../obj; \
	done


etc: ../bin/.etc

	#cp -r ../etc/$$i ../bin; \
	#
../bin/.etc:
	echo "Copy dirs from etc"
	$(COPY_ETC_CMD) $(COPY_ETC_SRC) $(COPY_ETC_DST)
	touch ../bin/.etc

cleanetc:
	rm -f ../bin/.etc	

	#cp -r ../etc/$$i ../bin; 
forceetc:
	echo "Copy dirs from etc"
	$(COPY_ETC_CMD) $(COPY_ETC_SRC) $(COPY_ETC_DST)
	touch ../bin/.etc

clean:
	echo $(OPSYS)
	@for i in $(SUBDIRS) ;\
	do \
	echo "cleaning" all "in $(CURRENT_DIR)/$$i..."; \
	$(MAKE) -C $$i clean; \
	done
	echo "cleaning all objs"
	rm -f ../obj/*.o
	echo "cleaning binaries"
	rm -f ../bin/*
