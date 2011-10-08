Project: EduNetGames

1.Dependencies: 
	-glut.
	-Raknet
	-boost 1.47.0
		- at least boost system, filesystem, regex, program options

2.Recommend project structre
-root
	|
	--hgclone1
	|
	--hgclone2
	|
	--hgclone3
	|
	--bin
	|
	--include
		|
		--boost(header)
		|
		--raknet(header)
	|
	--lib
	|
	--extlibs
		|
		--RakNet-3.81
		
3.Build project
-build boost from source or use binary packages
	-build from source short steps
	-goto source folder
	-call bootstrap
	-call b2.exe threading=multi link=shared --with-system --with-filesystem --with-regex --with-date_time --with-program_options --build-dir=../boost_build --toolset=msvc --prefix=<root> stage
-build raknet
	-use cmake
	-set target dir to root
-build EduNet
	-use cmake
		