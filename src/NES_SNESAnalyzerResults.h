#ifndef NES_SNES_ANALYZER_RESULTS
#define NES_SNES_ANALYZER_RESULTS

#include <AnalyzerResults.h>

class NES_SNESAnalyzer;
class NES_SNESAnalyzerSettings;

class NES_SNESAnalyzerResults : public AnalyzerResults
{
public:
	NES_SNESAnalyzerResults( NES_SNESAnalyzer* analyzer, NES_SNESAnalyzerSettings* settings );
	virtual ~NES_SNESAnalyzerResults();

	virtual void GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base );
	virtual void GenerateExportFile( const char* file, DisplayBase display_base, U32 export_type_user_id );

	virtual void GenerateFrameTabularText(U64 frame_index, DisplayBase display_base );
	virtual void GeneratePacketTabularText( U64 packet_id, DisplayBase display_base );
	virtual void GenerateTransactionTabularText( U64 transaction_id, DisplayBase display_base );

protected: //functions

protected:  //vars
	NES_SNESAnalyzerSettings* mSettings;
	NES_SNESAnalyzer* mAnalyzer;
};

#endif //NES_SNES_ANALYZER_RESULTS
