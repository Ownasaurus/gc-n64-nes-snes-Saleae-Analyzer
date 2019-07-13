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

	U64 command[3];

	mState = GC_COMMAND_PREFIX;
	U8 numBytes = 0;

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
		if (bs == BitState::BIT_HIGH)
		{
			/*either this is:
				an 0xFF packet
				a stop bit
				just byte data in the middle of a response
			*/
			// stop bit
			if (mState == GC_COMMAND_SUFFIX)
			{
				// mark it as a stop and move on to analyzing the next bit
				mResults->AddMarker(mGCN64Data->GetSampleNumber(), AnalyzerResults::Stop, mSettings->mInputChannel);
				mGCN64Data->AdvanceToNextEdge();

				// if recognized command with 
				if (command[0] == 0x00 || command[0] == 0x01 || command[0] == 0xFF)
				{
					mState = GC_RESPONSE_THREE;
					numBytes = 0;
				}
				else if (command[0] == 0x41)// || command[0] == 0x02 || command[0] == 0x03)
				{
					mState = GC_RESPONSE_TEN;
					numBytes = 0;
				}
				else if (command[0] == 0x40 && numBytes == 3)
				{
					mState = GC_RESPONSE_EIGHT;
					numBytes = 0;
				}
				continue;
			}
			else if (mState >= 3 && numBytes == mState)
			{
				// mark it as a stop, and we're done with the command
				mResults->AddMarker(mGCN64Data->GetSampleNumber(), AnalyzerResults::Stop, mSettings->mInputChannel);
				mGCN64Data->AdvanceToNextEdge();

				// reset to initial state
				mState = GC_COMMAND_PREFIX;
				memset(&command, 0, sizeof(U64) * 3);
				numBytes = 0;

				continue;
			}
			// 0xFF packet or regular byte data
			else
			{
				// mark it as a 1 and keep on going
				mResults->AddMarker(mGCN64Data->GetSampleNumber(), AnalyzerResults::One, mSettings->mInputChannel);
			}
		}
		else
		{
			mResults->AddMarker(mGCN64Data->GetSampleNumber(), AnalyzerResults::Zero, mSettings->mInputChannel);
		}

		U8 bits_read = 1;

		// make sure we start at a rising edge
		if (mGCN64Data->GetBitState() == BIT_LOW)
			mGCN64Data->AdvanceToNextEdge();

		// get to the falling edge
		mGCN64Data->AdvanceToNextEdge();

		while (1) // read at least 9 bits (1 byte + stop bit)
		{
			// get middle of first pulse, 2us later
			U32 numToggles = mGCN64Data->Advance(cg.AdvanceByTimeS(0.000002)); // advance by 2us
			// numToggles should be either 0 or 1 for our protocol
			bs = mGCN64Data->GetBitState();
			db.AddBit(bs);
			if (bs == BIT_HIGH)
				mResults->AddMarker(mGCN64Data->GetSampleNumber(), AnalyzerResults::One, mSettings->mInputChannel);
			else
				mResults->AddMarker(mGCN64Data->GetSampleNumber(), AnalyzerResults::Zero, mSettings->mInputChannel);

			bits_read++;

			// finish the bit properly
			mGCN64Data->AdvanceToNextEdge();

			if (mGCN64Data->GetBitState() == BIT_HIGH)
				mGCN64Data->AdvanceToNextEdge();

			if (bits_read == 8)
			{
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

		if (mState == GC_COMMAND_PREFIX)
		{
			command[0] = dbd;
			mState = GC_COMMAND_SUFFIX;
		}
		else if (mState == GC_COMMAND_SUFFIX)
		{
			command[numBytes] = dbd;
		}

		numBytes++;
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