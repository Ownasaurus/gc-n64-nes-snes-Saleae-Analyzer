#ifndef NES_SNES_ANALYZER_SETTINGS
#define NES_SNES_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class NES_SNESAnalyzerSettings : public AnalyzerSettings
{
public:
	NES_SNESAnalyzerSettings();
	virtual ~NES_SNESAnalyzerSettings();

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

#endif //NES_SNES_ANALYZER_SETTINGS
