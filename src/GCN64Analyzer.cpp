#include "GCN64Analyzer.h"
#include "GCN64AnalyzerSettings.h"
#include <AnalyzerChannelData.h>

GCN64Analyzer::GCN64Analyzer()
:	Analyzer2(),  
	mSettings( new GCN64AnalyzerSettings() ),
	mSimulationInitilized( false )
{
	SetAnalyzerSettings( mSettings.get() );
}

GCN64Analyzer::~GCN64Analyzer()
{
	KillThread();
}

void GCN64Analyzer::SetupResults()
{
	mResults.reset( new GCN64AnalyzerResults( this, mSettings.get() ) );
	SetAnalyzerResults( mResults.get() );
	mResults->AddChannelBubblesWillAppearOn( mSettings->mInputChannel );
}

void GCN64Analyzer::WorkerThread()
{
	mSampleRateHz = GetSampleRate();

	mGCN64Data = GetAnalyzerChannelData( mSettings->mInputChannel );

	// make sure we start on a rising edge
	if( mGCN64Data->GetBitState() == BIT_LOW )
		mGCN64Data->AdvanceToNextEdge();
	
	// TODO: do all of the anaylsis

	/*
	U32 samples_per_bit = mSampleRateHz / mSettings->mBitRate;
	U32 samples_to_first_center_of_first_data_bit = U32( 1.5 * double( mSampleRateHz ) / double( mSettings->mBitRate ) );

	for( ; ; )
	{
		U8 data = 0;
		U8 mask = 1 << 7;
		
		mSerial->AdvanceToNextEdge(); //falling edge -- beginning of the start bit

		U64 starting_sample = mSerial->GetSampleNumber();

		mSerial->Advance( samples_to_first_center_of_first_data_bit );

		for( U32 i=0; i<8; i++ )
		{
			//let's put a dot exactly where we sample this bit:
			mResults->AddMarker( mSerial->GetSampleNumber(), AnalyzerResults::Dot, mSettings->mInputChannel );

			if( mSerial->GetBitState() == BIT_HIGH )
				data |= mask;

			//mSerial->Advance( samples_per_bit );

			mask = mask >> 1;
		}


		//we have a byte to save. 
		Frame frame;
		frame.mData1 = data;
		frame.mFlags = 0;
		frame.mStartingSampleInclusive = starting_sample;
		frame.mEndingSampleInclusive = mSerial->GetSampleNumber();

		mResults->AddFrame( frame );
		mResults->CommitResults();
		ReportProgress( frame.mEndingSampleInclusive );
	}
	*/
}

bool GCN64Analyzer::NeedsRerun()
{
	return false;
}

U32 GCN64Analyzer::GenerateSimulationData( U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor** simulation_channels )
{
	if( mSimulationInitilized == false )
	{
		mSimulationDataGenerator.Initialize( GetSimulationSampleRate(), mSettings.get() );
		mSimulationInitilized = true;
	}

	return mSimulationDataGenerator.GenerateSimulationData( minimum_sample_index, device_sample_rate, simulation_channels );
}

U32 GCN64Analyzer::GetMinimumSampleRateHz()
{
	return 1000000 * 4;
}

const char* GCN64Analyzer::GetAnalyzerName() const
{
	return "GC and N64 Analyzer";
}

const char* GetAnalyzerName()
{
	return "GC and N64 Analyzer";
}

Analyzer* CreateAnalyzer()
{
	return new GCN64Analyzer();
}

void DestroyAnalyzer( Analyzer* analyzer )
{
	delete analyzer;
}