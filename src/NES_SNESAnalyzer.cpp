#include "NES_SNESAnalyzer.h"
#include "NES_SNESAnalyzerSettings.h"
#include <AnalyzerChannelData.h>

using namespace AnalyzerEnums;

NES_SNESAnalyzer::NES_SNESAnalyzer()
:	Analyzer2(),  
	mSettings( new NES_SNESAnalyzerSettings() ),
	mSimulationInitilized( false )
{
	SetAnalyzerSettings( mSettings.get() );
}

NES_SNESAnalyzer::~NES_SNESAnalyzer()
{
	KillThread();
}

void NES_SNESAnalyzer::SetupResults()
{
	mResults.reset( new NES_SNESAnalyzerResults( this, mSettings.get() ) );
	SetAnalyzerResults( mResults.get() );
	mResults->AddChannelBubblesWillAppearOn( mSettings->mLatchChannel );
}

void NES_SNESAnalyzer::WorkerThread()
{
	mLatch = GetAnalyzerChannelData( mSettings->mLatchChannel );
	mClock = GetAnalyzerChannelData(mSettings->mClockChannel);
	mD0 = GetAnalyzerChannelData(mSettings->mD0Channel);
	mD1 = GetAnalyzerChannelData(mSettings->mD1Channel);

	U64 dbd = 0;
	DataBuilder db;
	

	// make sure we are low
	if( mLatch->GetBitState() == BIT_HIGH )
		mLatch->AdvanceToNextEdge();

	// start at first rising edge
	mLatch->AdvanceToNextEdge();

	while(1)
	{
		// prepare data container
		dbd = 0;
		db.Reset(&dbd, MsbFirst, 32); // support up to 32 bit polling in case of overread

		// frame the frame
		U64 frame_start = mLatch->GetSampleNumber();
		if (mLatch->GetBitState() == BIT_HIGH)
			mLatch->AdvanceToNextEdge();
		mLatch->AdvanceToNextEdge();
		U64 frame_end = mLatch->GetSampleNumber();

		// advance all lines up to the rising edge of the latch; the beginning of the frame
		mClock->AdvanceToAbsPosition(frame_start);
		mD0->AdvanceToAbsPosition(frame_start);
		mD1->AdvanceToAbsPosition(frame_start);
		
		// process each clock
		while (mClock->GetSampleOfNextEdge() < frame_end)
		{
			// find next polling spot
			mClock->AdvanceToNextEdge();
			U64 clockEdge = mClock->GetSampleNumber();

			// advance and read the data line(s)
			mD0->AdvanceToAbsPosition(clockEdge);
			BitState D0_bit = mD0->GetBitState();
			mD1->AdvanceToAbsPosition(clockEdge);
			BitState D1_bit = mD0->GetBitState();

			if (D0_bit == BIT_HIGH)
			{
				mResults->AddMarker(mClock->GetSampleNumber(), AnalyzerResults::Zero, mSettings->mD0Channel);
			}
			else
			{
				mResults->AddMarker(mClock->GetSampleNumber(), AnalyzerResults::One, mSettings->mD0Channel);
			}

			if (D1_bit == BIT_HIGH)
			{
				mResults->AddMarker(mClock->GetSampleNumber(), AnalyzerResults::Zero, mSettings->mD1Channel);
			}
			else
			{
				mResults->AddMarker(mClock->GetSampleNumber(), AnalyzerResults::One, mSettings->mD1Channel);
			}

			mClock->AdvanceToNextEdge();
		}

		//we have a frame of data to save. 
		Frame frame;
		frame.mData1 = dbd;
		frame.mFlags = 0;
		frame.mStartingSampleInclusive = frame_start;
		frame.mEndingSampleInclusive = frame_end;

		mResults->AddFrame(frame);
		mResults->CommitResults();
		ReportProgress(frame.mEndingSampleInclusive);
	}

}

bool NES_SNESAnalyzer::NeedsRerun()
{
	return false;
}

U32 NES_SNESAnalyzer::GenerateSimulationData( U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor** simulation_channels )
{
	if( mSimulationInitilized == false )
	{
		mSimulationDataGenerator.Initialize( GetSimulationSampleRate(), mSettings.get() );
		mSimulationInitilized = true;
	}

	return mSimulationDataGenerator.GenerateSimulationData( minimum_sample_index, device_sample_rate, simulation_channels );
}

U32 NES_SNESAnalyzer::GetMinimumSampleRateHz()
{
	return 115200 * 4;
}

const char* NES_SNESAnalyzer::GetAnalyzerName() const
{
	return "NES and SNES Analyzer";
}

const char* GetAnalyzerName()
{
	return "NES and SNES Analyzer";
}

Analyzer* CreateAnalyzer()
{
	return new NES_SNESAnalyzer();
}

void DestroyAnalyzer( Analyzer* analyzer )
{
	delete analyzer;
}