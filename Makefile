binary=raumscanner
masterDir=master

all:
	$(MAKE) -C $(masterDir)/
	cp $(masterDir)/$(binary) .

clean:
	$(MAKE) -C $(masterDir)/ clean
	rm $(binary)
