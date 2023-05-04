CXX_FILES=src/account.cpp src/accounts_shared_memory.cpp src/bank_system.cpp src/semaphores.cpp


init:
	mkdir build
	g++ $(CXX_FILES) src/init.cpp -o build/init

delete:
	g++ $(CXX_FILES) src/delete.cpp -o build/delete

main:
	g++ $(CXX_FILES) src/main.cpp -o build/main

debug: init delete main
	g++ -g $(CXX_FILES) src/main.cpp -o build/debug -O0 --coverage
	valgrind ./build/debug < test.txt

test: debug
	./build/debug < test.txt; lcov -c -d . --rc lcov_branch_coverage=1 -o build/coverage.info; \
	genhtml --branch-coverage -o build/html build/coverage.info
