default: debug test

clean:
	[[ -d build ]] && rm -r build

bake:
	mkdir -p build
	cd build; cmake -DCMAKE_BUILD_TYPE=Release ..; make -j3

debug:
	mkdir -p build
	cd build; cmake -DCMAKE_BUILD_TYPE=Debug ..; make -j3

test:
	./runtest
