#ifndef NES_SNES_ANALYZER_H
#define NES_SNES_ANALYZER_H

#include <Analyzer.h>
#include "NES_SNESAnalyzerResults.h"
#include "NES_SNESSimulationDataGenerator.h"

class NES_SNESAnalyzerSettings;
class ANALYZER_EXPORT NES_SNESAnalyzer : public Analyzer2
{
public:
	NES_SNESAnalyzer();
	virtual ~NES_SNESAnalyzer();

	virtual void SetupResults();
	virtual void WorkerThread();

	virtual U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels );
	virtual U32 GetMinimumSampleRateHz();

	virtual const char* GetAnalyzerName() const;
	virtual bool NeedsRerun();

#pragma warning( push )
#pragma warning( disable : 4251 )
protected: //vars
	std::auto_ptr< NES_SNESAnalyzerSettings > mSettings;
	std::auto_ptr< NES_SNESAnalyzerResults > mResults;
	AnalyzerChannelData* mLatch;
	AnalyzerChannelData* mClock;
	AnalyzerChannelData* mD0;
	AnalyzerChannelData* mD1;

	NES_SNESSimulationDataGenerator mSimulationDataGenerator;
	bool mSimulationInitilized;

	U32 mSampleRateHz;

#pragma warning( pop )
};

extern "C" ANALYZER_EXPORT const char* __cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer* __cdecl CreateAnalyzer( );
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer( Analyzer* analyzer );

#endif //NES_SNES_ANALYZER_H
