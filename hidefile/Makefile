all:	hidefile.so

zip: hidefile.c
	@ zip p3-submit.zip hidefile.c
	@ echo created p3-submit.zip for upload to canvas

hidefile.so:	hidefile.c
	gcc -shared -fPIC hidefile.c -o hidefile.so 

test:	all
	@ echo ======== testing hiding files ======== 
	@ echo creating "secret-1.txt"; echo "these are the contents of secret-1.txt" >secret-1.txt
	@ echo creating "secret-2.txt"; echo "these are the contents of secret-2.txt" >secret-2.txt
	@ echo creating "secret-3.txt"; echo "these are the contents of secret-3.txt" >secret-3.txt
	@ echo Three files: ;ls -l
	@ echo;echo "hiding secret-1.txt"
	export HIDDEN=secret-1.txt LD_PRELOAD="$$PWD"/hidefile.so; ls -l
	@ echo; echo "hiding secret-2.txt"
	export HIDDEN=secret-2.txt LD_PRELOAD="$$PWD"/hidefile.so; ls -l
	@ echo; echo "hiding secret-1.txt and secret-2.txt"
	export HIDDEN=secret-1.txt:secret-2.txt LD_PRELOAD="$$PWD"/hidefile.so; ls -l
	@ echo;echo "hiding nothing"
	unset HIDDEN
	LD_PRELOAD="$$PWD"/hidefile.so; ls -l
	@ echo ======== testing blocking files ======== 
	@ echo creating "testfile.csv"; echo "contents of testfile.csv" >testfile.csv
	@ echo creating "testfile.a"; echo "contents of testfile.a" >testfile.a
	@ echo creating "testfile.b"; echo "contents of testfile.b" >testfile.b
	@ echo;echo "setting BLOCKED=.csv"
	@ echo cat testfile.csv should fail
	export BLOCKED=.csv LD_PRELOAD="$$PWD"/hidefile.so; cat testfile.csv && echo open succeeded - test failed || echo open failed - test passed
	@ echo;echo cat testfile.a should succeed
	cat testfile.a && echo open succeeded - test passed || echo open failed - test failed
	@ echo;echo "setting BLOCKED=.csv:b:.a"
	@ echo cat testfile.a should fail
	export BLOCKED=.csv:b:a LD_PRELOAD="$$PWD"/hidefile.so; cat testfile.a && echo open succeeded - test failed || echo open failed - test passed
	@ echo;echo cat testfile.a should fail
	export BLOCKED=.csv:b:a LD_PRELOAD="$$PWD"/hidefile.so; cat testfile.b && echo open succeeded - test failed || echo open failed - test passed
	@ echo;echo "setting BLOCKED=a_long_string_that_should-not_cause_problems"
	@ echo cat testfile.a should succeed 
	export BLOCKED=a_long_string_that_should-not_cause_problems LD_PRELOAD="$$PWD"/hidefile.so; cat testfile.a && echo open succeeded - test passed || echo open failed - test failed

clean:
	rm -f hidefile.so secret-1.txt secret-2.txt secret-3.txt testfile.a testfile.b testfile.csv
