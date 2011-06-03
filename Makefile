default: debug test

clean:
	[[ -d build ]] && rm -r build
	[[ -d docs/doxygen ]] && rm -r docs/doxygen

bake:
	mkdir -p build
	cd build; cmake -DCMAKE_BUILD_TYPE=Release ..; make -j3

debug:
	mkdir -p build
	cd build; cmake -DCMAKE_BUILD_TYPE=Debug ..; make -j3

test:
	./runtest

install:
	echo "Installation not implemented yet."

# Editor

run_editor_client:
	cd bin; ./editor_client

run_editor_server:
	cd bin; ./editor_server	

doc:
	doxygen docs/Doxyfile
