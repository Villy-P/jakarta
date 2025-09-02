.ONESHELL:

# Run this whenever you first start the project, or when you add another C++ file into the src directory
reset:
	rmdir build
	mkdir build
	cd build
	cmake -GNinja ..

# Run this whenever you want to re-compile your project and run it.
run:
	ninja -C build
	.\JAKARTA.exe $(args)

run-linux:
	ninja -C build
	./JAKARTA $(args)

run-test:
	ninja -C build
	.\JAKARTA_TEST.exe $(args)

run-test-linux:
	ninja -C build
	./JAKARTA_TEST $(args)