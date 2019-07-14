#ifndef NES_SNES_SIMULATION_DATA_GENERATOR
#define NES_SNES_SIMULATION_DATA_GENERATOR

#include <SimulationChannelDescriptor.h>
#include <string>
class NES_SNESAnalyzerSettings;

class NES_SNESSimulationDataGenerator
{
public:
	NES_SNESSimulationDataGenerator();
	~NES_SNESSimulationDataGenerator();

	void Initialize( U32 simulation_sample_rate, NES_SNESAnalyzerSettings* settings );
	U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel );

protected:
	NES_SNESAnalyzerSettings* mSettings;
	U32 mSimulationSampleRateHz;

protected:
	void CreateSerialByte();
	std::string mSerialText;
	U32 mStringIndex;

	SimulationChannelDescriptor mSerialSimulationData;

};
#endif //NES_SNES_SIMULATION_DATA_GENERATOR