#OBJS specifies which files to compile as part of the project
OBJS = new.c linked_list.c

#CC specifies which compiler we're using
CXX = clang

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -w

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -I/Library/Frameworks/SDl2.framework/Headers -I/Library/Frameworks/SDl2_ttf.framework/Headers -F/Library/Frameworks/ -framework SDL2 -framework SDL2_ttf

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = game

#This is the target that compiles our executable
all : $(OBJS)
	$(CXX) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)