TARGET    := libfsky.a
CFLAGS    := -g -std=c++11 -O2 -Wno-deprecated -Wall
OBJ_DIR   := objs

INCLUDE   := -I./

DIRS      := fs_tpls fs_str fs_io fs_dtime fs_math fs_encode
SRCS	  := $(foreach dir, $(DIRS), $(wildcard $(dir)/*.c $(dir)/*.cpp))
OBJS      := $(patsubst %.c,%.o, $(patsubst %.c,%.o, $(SRCS)))
OBJS      := $(patsubst %.cpp,%.o, $(patsubst %.c,%.o, $(OBJS)))
OBJNAMES  := $(addprefix $(OBJ_DIR)/, $(notdir $(OBJS)))

$(TARGET): $(OBJS)
	ar r $@ $(OBJNAMES)
	cp $@ ../lib -f
	mv $@ /home/tarsproto/majong/thirds/
	echo "INCLUDE += -I../thirds/libfsky/src" > /home/tarsproto/majong/thirds/fsky.mk
	echo "LIB += -L/home/tarsproto/majong/thirds/ -lfsky" >> /home/tarsproto/majong/thirds/fsky.mk
#	chmod 777 -R /home/tarsproto/majong/thirds

$(OBJS): %.o:%.cpp
	$(CXX)  $(CFLAGS) -o $(OBJ_DIR)/$(notdir $@) -c $< $(INCLUDE)

clean:
	-$(RM) $(OBJ_DIR)/*.o
	-$(RM) $(TARGET)
	-$(RM) ../lib/*.a
	-$(RM) /home/tarsproto/majong/thirds/$(TARGET)

