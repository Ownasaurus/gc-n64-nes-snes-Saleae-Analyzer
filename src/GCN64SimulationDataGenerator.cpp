#include "GCN64SimulationDataGenerator.h"
#include "GCN64AnalyzerSettings.h"

GCN64SimulationDataGenerator::GCN64SimulationDataGenerator()
{
}

GCN64SimulationDataGenerator::~GCN64SimulationDataGenerator()
{
}

void GCN64SimulationDataGenerator::Initialize( U32 simulation_sample_rate, GCN64AnalyzerSettings* settings )
{
	mSimulationSampleRateHz = simulation_sample_rate;
	mSettings = settings;

	mSerialSimulationData.SetChannel( mSettings->mInputChannel );
	mSerialSimulationData.SetSampleRate( simulation_sample_rate );
	mSerialSimulationData.SetInitialBitState( BIT_HIGH );
}

U32 GCN64SimulationDataGenerator::GenerateSimulationData( U64 largest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel )
{
	U64 adjusted_largest_sample_requested = AnalyzerHelpers::AdjustSimulationTargetSample( largest_sample_requested, sample_rate, mSimulationSampleRateHz );

	while( mSerialSimulationData.GetCurrentSampleNumber() < adjusted_largest_sample_requested )
	{
		CreateN64Byte();
	}

	*simulation_channel = &mSerialSimulationData;
	return 1;
}

void GCN64SimulationDataGenerator::CreateN64Byte()
{
	// can modify this to generate sample data

	ClockGenerator cg;
	cg.Init(1000000, mSimulationSampleRateHz); // target frequency of 1000000 should correspond to 1us per period

	/*
	mSerialSimulationData.Advance( samples_per_bit * 10 );

	mSerialSimulationData.Transition();  //low-going edge for start bit
	mSerialSimulationData.Advance( samples_per_bit );  //add start bit time

	U8 mask = 0x1 << 7;
	for( U32 i=0; i<8; i++ )
	{
		if( ( byte & mask ) != 0 )
			mSerialSimulationData.TransitionIfNeeded( BIT_HIGH );
		else
			mSerialSimulationData.TransitionIfNeeded( BIT_LOW );

		mSerialSimulationData.Advance( samples_per_bit );
		mask = mask >> 1;
	}

	mSerialSimulationData.TransitionIfNeeded( BIT_HIGH ); //we need to end high

	//lets pad the end a bit for the stop bit:
	mSerialSimulationData.Advance( samples_per_bit );

	*/
}

void GCN64SimulationDataGenerator::CreateN64Zero(ClockGenerator cg)
{

}

void GCN64SimulationDataGenerator::CreateN64One(ClockGenerator cg)
{

}