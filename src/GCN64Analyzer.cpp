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
	
	// get to the first falling edge
	mGCN64Data->AdvanceToNextEdge();

	U64 starting_sample = mGCN64Data->GetSampleNumber();

	// get middle of first pulse, 2us later
	// TODO: USE CLOCK GENERATOR TO DO THIS?

	U32 command = mGCN64Data->GetBitState();
	U8 bits_read = 1;
	U8 currentBit = 0;

	while (1) // read at least 9 bits (1 byte + stop bit)
	{
		command = command << 1; // make room for the new bit

		// make sure we start at a rising edge
		if (mGCN64Data->GetBitState() == BIT_LOW)
			mGCN64Data->AdvanceToNextEdge();

		// get to the falling edge
		mGCN64Data->AdvanceToNextEdge();

		// get middle of first pulse, 2us later
		// TODO: USE CLOCK GENERATOR TO DO THIS?

		currentBit = mGCN64Data->GetBitState();

		if (currentBit == 5) // timeout TODO: replace this with actual determination if we're at the end of the data
		{
			if (bits_read >= 8)
			{
				command = command >> 2; // get rid of the stop bit AND the room we made for an additional bit
				break;
			}
			else // there is no possible way this can be a real command
			{
				command = 5; // dummy value
				break;
			}
		}
		command += currentBit;

		bits_read++;

		if (bits_read >= 25) // this is the longest known command length
		{
			command = command >> 1; // get rid of the stop bit (which is always a 1)
			break;
		}

		Frame frame;
		frame.mData1 = command;
		frame.mFlags = 0;
		frame.mStartingSampleInclusive = starting_sample;
		frame.mEndingSampleInclusive = mGCN64Data->GetSampleNumber();

		mResults->AddFrame(frame);
		mResults->CommitResults();
		ReportProgress(frame.mEndingSampleInclusive);
	}

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