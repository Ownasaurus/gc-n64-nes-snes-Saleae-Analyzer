#ifndef GCN64_SIMULATION_DATA_GENERATOR
#define GCN64_SIMULATION_DATA_GENERATOR

#include <SimulationChannelDescriptor.h>
#include <AnalyzerHelpers.h>
#include <string>
class GCN64AnalyzerSettings;

class GCN64SimulationDataGenerator
{
public:
	GCN64SimulationDataGenerator();
	~GCN64SimulationDataGenerator();

	void Initialize( U32 simulation_sample_rate, GCN64AnalyzerSettings* settings );
	U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel );

protected:
	GCN64AnalyzerSettings* mSettings;
	U32 mSimulationSampleRateHz;
	SimulationChannelDescriptor mSerialSimulationData;

protected:
	void CreateN64Byte();
	void CreateN64Zero(ClockGenerator cg);
	void CreateN64One(ClockGenerator cg);

};
#endif //GCN64_SIMULATION_DATA_GENERATOR