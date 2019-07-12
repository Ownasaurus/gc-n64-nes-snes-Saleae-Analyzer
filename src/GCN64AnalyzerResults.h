#ifndef GCN64_ANALYZER_RESULTS
#define GCN64_ANALYZER_RESULTS

#include <AnalyzerResults.h>

class GCN64Analyzer;
class GCN64AnalyzerSettings;

class GCN64AnalyzerResults : public AnalyzerResults
{
public:
	GCN64AnalyzerResults( GCN64Analyzer* analyzer, GCN64AnalyzerSettings* settings );
	virtual ~GCN64AnalyzerResults();

	virtual void GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base );
	virtual void GenerateExportFile( const char* file, DisplayBase display_base, U32 export_type_user_id );

	virtual void GenerateFrameTabularText(U64 frame_index, DisplayBase display_base );
	virtual void GeneratePacketTabularText( U64 packet_id, DisplayBase display_base );
	virtual void GenerateTransactionTabularText( U64 transaction_id, DisplayBase display_base );

protected: //functions

protected:  //vars
	GCN64AnalyzerSettings* mSettings;
	GCN64Analyzer* mAnalyzer;
};

#endif //GCN64_ANALYZER_RESULTS
