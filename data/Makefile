all: jumpbump.dat

jumpbump.dat: bump.mod calib.dat death.smp fly.smp font.gob jump.mod jump.smp levelmap.txt level.pcx mask.pcx menu.pcx menumask.pcx numbers.gob objects.gob rabbit.gob scores.mod splash.smp spring.smp
	../jnbpack -o jumpbump.dat bump.mod calib.dat death.smp fly.smp font.gob jump.mod jump.smp levelmap.txt level.pcx mask.pcx menu.pcx menumask.pcx numbers.gob objects.gob rabbit.gob scores.mod splash.smp spring.smp

clean:
	rm -f jumpbump.dat
