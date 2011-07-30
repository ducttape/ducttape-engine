default: debug test

clean:
	[[ -d build ]] && rm -r build

bake:
	mkdir -p build
	cd build; cmake -DCMAKE_BUILD_TYPE=Release ..; make -j$(shell grep -c ^processor /proc/cpuinfo)

debug:
	mkdir -p build
	cd build; cmake -DCMAKE_BUILD_TYPE=Debug ..; make -j$(shell grep -c ^processor /proc/cpuinfo)

doc:
	cd build; make doc
	echo "Generating docs done! Now open build/docs/doxygen/html/index.html"

test:
	cd build; make test
