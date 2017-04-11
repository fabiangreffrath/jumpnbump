EXEC ?= exec

GOBPACK = ../gobpack$(EXE)
JNBPACK = ../jnbpack$(EXE)
GOBS = font.gob numbers.gob objects.gob rabbit.gob
DATAFILES = bump.mod calib.dat death.smp fly.smp jump.mod \
	jump.smp levelmap.txt level.pcx mask.pcx menu.pcx \
	menumask.pcx $(GOBS) scores.mod splash.smp spring.smp

all: jumpbump.dat

font.gob: $(GOBPACK)
	$(EXEC) $(GOBPACK) font

rabbit.gob: $(GOBPACK)
	$(EXEC) $(GOBPACK) rabbit

numbers.gob: $(GOBPACK)
	$(EXEC) $(GOBPACK) numbers

objects.gob: $(GOBPACK)
	$(EXEC) $(GOBPACK) objects

jumpbump.dat: $(DATAFILES) $(JNBPACK)
	$(EXEC) $(JNBPACK) -o jumpbump.dat $(DATAFILES)

$(JNBPACK):
	$(MAKE) -C ../modify

clean:
	$(RM) jumpbump.dat $(GOBS)
