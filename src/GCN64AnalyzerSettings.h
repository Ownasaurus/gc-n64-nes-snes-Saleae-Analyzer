#ifndef GCN64_ANALYZER_SETTINGS
#define GCN64_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class GCN64AnalyzerSettings : public AnalyzerSettings
{
public:
	GCN64AnalyzerSettings();
	virtual ~GCN64AnalyzerSettings();

	virtual bool SetSettingsFromInterfaces();
	void UpdateInterfacesFromSettings();
	virtual void LoadSettings( const char* settings );
	virtual const char* SaveSettings();

	
	Channel mInputChannel;
	U32 mBitRate;

protected:
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mInputChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceInteger >	mBitRateInterface;
};

#endif //GCN64_ANALYZER_SETTINGS
