#include "GCN64Analyzer.h"
#include "GCN64AnalyzerSettings.h"
#include <AnalyzerChannelData.h>

using namespace AnalyzerEnums;

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

	// make sure we start with the data line high
	if( mGCN64Data->GetBitState() == BIT_LOW )
		mGCN64Data->AdvanceToNextEdge();

	// get to the first falling edge
	mGCN64Data->AdvanceToNextEdge();

	ClockGenerator cg;
	cg.Init(250000, mSampleRateHz);

	U64 dbd;
	DataBuilder db;

	//TODO: HANDLE STOP BITS AND PAUSES ON THE LINE
	while (1)
	{
		db.Reset(&dbd, MsbFirst, 8);
		dbd = 0;

		// this is the beginning of the first bit
		U64 starting_sample = mGCN64Data->GetSampleNumber();

		// get middle of first pulse, 2us later
		mGCN64Data->Advance(cg.AdvanceByTimeS(0.000002)); // advance by 2us
		BitState bs = mGCN64Data->GetBitState();
		db.AddBit(bs);
		if(bs == BitState::BIT_HIGH)
			mResults->AddMarker(mGCN64Data->GetSampleNumber(), AnalyzerResults::One, mSettings->mInputChannel);
		else
			mResults->AddMarker(mGCN64Data->GetSampleNumber(), AnalyzerResults::Zero, mSettings->mInputChannel);

		U8 bits_read = 1;

		// make sure we start at a rising edge
		if (mGCN64Data->GetBitState() == BIT_LOW)
			mGCN64Data->AdvanceToNextEdge();

		while (1) // read at least 9 bits (1 byte + stop bit)
		{
			// get to the falling edge
			mGCN64Data->AdvanceToNextEdge();

			// get middle of first pulse, 2us later
			U32 numToggles = mGCN64Data->Advance( cg.AdvanceByTimeS(0.000002) ); // advance by 2us
			// numToggles should be either 0 or 1 for our protocol
			bs = mGCN64Data->GetBitState();
			db.AddBit(bs);
			if (bs == BIT_HIGH)
				mResults->AddMarker(mGCN64Data->GetSampleNumber(), AnalyzerResults::One, mSettings->mInputChannel);
			else
				mResults->AddMarker(mGCN64Data->GetSampleNumber(), AnalyzerResults::Zero, mSettings->mInputChannel);

			bits_read++;

			if (bits_read == 8)
			{
				// finish the frame properly
				mGCN64Data->AdvanceToNextEdge();

				if (mGCN64Data->GetBitState() == BIT_HIGH)
					mGCN64Data->AdvanceToNextEdge();

				bits_read = 0;

				break;
			}
		}

		Frame frame;
		frame.mData1 = dbd;
		frame.mFlags = 0;
		frame.mStartingSampleInclusive = starting_sample;
		frame.mEndingSampleInclusive = mGCN64Data->GetSampleNumber();

		mResults->AddFrame(frame);
		mResults->CommitResults();
		ReportProgress(frame.mEndingSampleInclusive);
	}
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