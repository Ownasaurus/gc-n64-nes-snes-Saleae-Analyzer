linux: LIB := -lAnalyzer64
linux: GPP := g++
linux: GFLAGS := -m64
linux: FT := so
linux: main
linux32: LIB := -lAnalyzer
linux32: GPP := g++
linux32: GFLAGS := -m32
linux32: FT := so
linux32: main
# win64: LIB := -lAnalyzer
# win64: GPP := x86_64-w64-mingw32-g++
# win64: GFLAGS := -m64
# win64: FT := dll
# win64: main
# win32: LIB := -lAnalyzer
# win32: GPP := i686-w64-mingw32-g++
# win32: GFLAGS := -m32
# win32: FT := dll
# win32: main
main: 
	${GPP} ${GFLAGS} -I"./AnalyzerSDK/include" -O3 -w -c -fpic -o"release/GCN64SimulationDataGenerator.o" "src/GCN64SimulationDataGenerator.cpp"
	${GPP} ${GFLAGS} -I"./AnalyzerSDK/include" -O0 -w -c -fpic -g -o"debug/GCN64SimulationDataGenerator.o" "src/GCN64SimulationDataGenerator.cpp"
	${GPP} ${GFLAGS} -I"./AnalyzerSDK/include" -O3 -w -c -fpic -o"release/GCN64AnalyzerResults.o" "src/GCN64AnalyzerResults.cpp"
	${GPP} ${GFLAGS} -I"./AnalyzerSDK/include" -O0 -w -c -fpic -g -o"debug/GCN64AnalyzerResults.o" "src/GCN64AnalyzerResults.cpp"
	${GPP} ${GFLAGS} -I"./AnalyzerSDK/include" -O3 -w -c -fpic -o"release/GCN64Analyzer.o" "src/GCN64Analyzer.cpp"
	${GPP} ${GFLAGS} -I"./AnalyzerSDK/include" -O0 -w -c -fpic -g -o"debug/GCN64Analyzer.o" "src/GCN64Analyzer.cpp"
	${GPP} ${GFLAGS} -I"./AnalyzerSDK/include" -O3 -w -c -fpic -o"release/GCN64AnalyzerSettings.o" "src/GCN64AnalyzerSettings.cpp"
	${GPP} ${GFLAGS} -I"./AnalyzerSDK/include" -O0 -w -c -fpic -g -o"debug/GCN64AnalyzerSettings.o" "src/GCN64AnalyzerSettings.cpp"
	${GPP} ${GFLAGS} -L"./AnalyzerSDK/lib" ${LIB} -shared -o"release/libGCN64Analyzer.${FT}" release/GCN64SimulationDataGenerator.o release/GCN64AnalyzerResults.o release/GCN64Analyzer.o release/GCN64AnalyzerSettings.o
	${GPP} ${GFLAGS} -L"./AnalyzerSDK/lib" ${LIB} -shared -o"debug/libGCN64Analyzer.${FT}" debug/GCN64SimulationDataGenerator.o debug/GCN64AnalyzerResults.o debug/GCN64Analyzer.o debug/GCN64AnalyzerSettings.o
	${GPP} ${GFLAGS} -I"./AnalyzerSDK/include" -O3 -w -c -fpic -o"release/NES_SNESSimulationDataGenerator.o" "src/NES_SNESSimulationDataGenerator.cpp"
	${GPP} ${GFLAGS} -I"./AnalyzerSDK/include" -O0 -w -c -fpic -g -o"debug/NES_SNESSimulationDataGenerator.o" "src/NES_SNESSimulationDataGenerator.cpp"
	${GPP} ${GFLAGS} -I"./AnalyzerSDK/include" -O3 -w -c -fpic -o"release/NES_SNESAnalyzerResults.o" "src/NES_SNESAnalyzerResults.cpp"
	${GPP} ${GFLAGS} -I"./AnalyzerSDK/include" -O0 -w -c -fpic -g -o"debug/NES_SNESAnalyzerResults.o" "src/NES_SNESAnalyzerResults.cpp"
	${GPP} ${GFLAGS} -I"./AnalyzerSDK/include" -O3 -w -c -fpic -o"release/NES_SNESAnalyzer.o" "src/NES_SNESAnalyzer.cpp"
	${GPP} ${GFLAGS} -I"./AnalyzerSDK/include" -O0 -w -c -fpic -g -o"debug/NES_SNESAnalyzer.o" "src/NES_SNESAnalyzer.cpp"
	${GPP} ${GFLAGS} -I"./AnalyzerSDK/include" -O3 -w -c -fpic -o"release/NES_SNESAnalyzerSettings.o" "src/NES_SNESAnalyzerSettings.cpp"
	${GPP} ${GFLAGS} -I"./AnalyzerSDK/include" -O0 -w -c -fpic -g -o"debug/NES_SNESAnalyzerSettings.o" "src/NES_SNESAnalyzerSettings.cpp"
	${GPP} ${GFLAGS} -L"./AnalyzerSDK/lib" ${LIB} -shared -o"release/libNES_SNESAnalyzer.${FT}" release/NES_SNESSimulationDataGenerator.o release/NES_SNESAnalyzerResults.o release/NES_SNESAnalyzer.o release/NES_SNESAnalyzerSettings.o
	${GPP} ${GFLAGS} -L"./AnalyzerSDK/lib" ${LIB} -shared -o"debug/libNES_SNESAnalyzer.${FT}" debug/NES_SNESSimulationDataGenerator.o debug/NES_SNESAnalyzerResults.o debug/NES_SNESAnalyzer.o debug/NES_SNESAnalyzerSettings.o

.phony clean:
	rm release/*
	rm debug/*