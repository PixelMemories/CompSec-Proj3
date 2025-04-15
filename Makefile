zip:
	@ (cd hidefile; zip -r ../p3-submit.zip hidefile.c)
	@ echo created p3-submit.zip for uploading to canvas

all:
	@ cd ..;zip -r p3.zip p3/random/myrand.c p3/random/random.c p3/random/Makefile p3/README.txt p3/hidefile/Makefile p3/hidefile/hidefile.c p3/hidefile/env.c p3/Makefile 
	@ echo created ../p3.zip

clean:
	rm -f p3-submit.zip
