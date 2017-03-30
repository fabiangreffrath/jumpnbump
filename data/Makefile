GOBS = font.gob numbers.gob objects.gob rabbit.gob
DATAFILES = bump.mod calib.dat death.smp fly.smp jump.mod \
	jump.smp levelmap.txt level.pcx mask.pcx menu.pcx \
	menumask.pcx $(GOBS) scores.mod splash.smp spring.smp

all: jumpbump.dat

font.gob:
	../gobpack font

rabbit.gob:
	../gobpack rabbit

numbers.gob:
	../gobpack numbers

objects.gob:
	../gobpack objects

jumpbump.dat: $(DATAFILES) ../jnbpack
	../jnbpack -o jumpbump.dat $(DATAFILES)

../jnbpack:
	$(MAKE) -C ../modify

clean:
	$(RM) jumpbump.dat $(GOBS)
