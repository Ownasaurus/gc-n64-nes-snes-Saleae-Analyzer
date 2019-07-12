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

protected:
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mInputChannelInterface;
};

#endif //GCN64_ANALYZER_SETTINGS
