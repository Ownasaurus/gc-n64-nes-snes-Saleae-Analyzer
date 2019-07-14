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

	
	Channel mLatchChannel;
	Channel mClockChannel;
	Channel mD0Channel;
	Channel mD1Channel;

protected:
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mLatchChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mClockChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mD0ChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mD1ChannelInterface;
};

#endif //NES_SNES_ANALYZER_SETTINGS
