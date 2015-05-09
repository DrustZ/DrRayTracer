.PHONY:all clean
CXX = g++
RM = rm
MKDIR = mkdir
FIND = /bin/find
INCLUDEFLAGS = -I$(incdir)
CXXFLAGS = --std=c++11 -march=native -fopenmp -DDEBUG -mavx2 -O3

incdir = include
srcdir = src
objdir = obj
bindir = bin

dirs := $(objdir) $(bindir)

src = $(wildcard $(srcdir)/*.cpp)
obj = $(patsubst %.cpp, $(objdir)/%.o, $(notdir ${src}))
dep = $(patsubst %.cpp, $(srcdir)/%.d, $(notdir ${src}))
target = ray
bin = $(bindir)/$(target)

all: $(dirs) $(bin)

$(dirs):
	$(MKDIR) $@
	
$(obj): $(objdir)/%.o: $(srcdir)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDEFLAGS) -o $@ -c $<

$(bin): $(obj)
	$(CXX) $(CXXFLAGS) $(INCLUDEFLAGS) -o $@ $^

$(dep): $(srcdir)/%.d: $(srcdir)/%.cpp
	@set -e; \
	rm -f $@; \
	$(CXX) -MM $(INCLUDEFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,$(objdir)/\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

-include $(dep)

.PHONY:clean cleanall
clean:
	# remove temporary build files
	$(RM) -rf $(dirs) $(srcdir)/*.d $(srcdir)/*.d.*
    
cleanall: clean
	# remove backup files either
	$(FIND) . -name "*~" -o -name "*.bak" -o -name "*.swp" | xargs $(RM)
