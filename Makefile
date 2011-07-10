default: debug test

clean:
	[[ -d build ]] && rm -r build

bake:
	mkdir -p build
	cd build; cmake -DCMAKE_BUILD_TYPE=Release ..; make -j3

debug:
	mkdir -p build
	cd build; cmake -DCMAKE_BUILD_TYPE=Debug ..; make -j3

doc:
	cd build; make doc

test:
	cd build; make test
#
#package:
#	cd build; make package
#
#package_source:
#	cd build; make package_source
