FRONT_STL=a8picoplus_front.stl
BACK_STL=a8picoplus_back.stl
COMBINED_STL=a8picoplus_combined.stl

MAIN_SCAD=a8picoplus_shell.scad

SCAD_MODULES=\
	a8picoplus_back.scad\
	a8picoplus_front.scad\
	a8picoplus_dimensions.scad\
	cube_round.scad


all: $(FRONT_STL) $(BACK_STL) $(COMBINED_STL)

clean:
	rm -rf $(FRONT_STL) $(BACK_STL) $(COMBINED_STL)

$(FRONT_STL): $(MAIN_SCAD) $(SCAD_MODULES)
	openscad -q -o $@ -D item=1 $<

$(BACK_STL): $(MAIN_SCAD) $(SCAD_MODULES)
	openscad -q -o $@ -D item=2 $<

$(COMBINED_STL): $(MAIN_SCAD) $(SCAD_MODULES)
	openscad -q -o $@ -D item=3 $<


