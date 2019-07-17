#include "NES_SNESAnalyzerSettings.h"
#include <AnalyzerHelpers.h>


NES_SNESAnalyzerSettings::NES_SNESAnalyzerSettings()
:	mLatchChannel( UNDEFINED_CHANNEL ),
	mClockChannel(UNDEFINED_CHANNEL),
	mD0Channel(UNDEFINED_CHANNEL),
	mD1Channel(UNDEFINED_CHANNEL),
	mConsole(CONSOLE_UNDEFINED)
{
	mLatchChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mLatchChannelInterface->SetTitleAndTooltip( "Latch", "Standard NES and SNES Analyzer" );
	mLatchChannelInterface->SetChannel( mLatchChannel );

	mClockChannelInterface.reset(new AnalyzerSettingInterfaceChannel());
	mClockChannelInterface->SetTitleAndTooltip("Clock", "Standard NES and SNES Analyzer");
	mClockChannelInterface->SetChannel(mClockChannel);

	mD0ChannelInterface.reset(new AnalyzerSettingInterfaceChannel());
	mD0ChannelInterface->SetTitleAndTooltip("D0", "Standard NES and SNES Analyzer");
	mD0ChannelInterface->SetChannel(mD0Channel);

	mD1ChannelInterface.reset(new AnalyzerSettingInterfaceChannel());
	mD1ChannelInterface->SetTitleAndTooltip("D1", "Standard NES and SNES Analyzer");
	mD1ChannelInterface->SetSelectionOfNoneIsAllowed(true);
	mD1ChannelInterface->SetChannel(mD1Channel);

	mConsoleInterface.reset(new AnalyzerSettingInterfaceNumberList());
	mConsoleInterface->SetTitleAndTooltip("Console", "Standard NES and SNES Analyzer");
	mConsoleInterface->AddNumber(CONSOLE_NES, "NES", "NES");
	mConsoleInterface->AddNumber(CONSOLE_SNES, "SNES", "SNES");

	mLatchWindowInterface.reset(new AnalyzerSettingInterfaceInteger());
	mLatchWindowInterface->SetTitleAndTooltip("Latch window (ms)", "Standard NES and SNES Analyzer");
	mLatchWindowInterface->SetMin(0);
	mLatchWindowInterface->SetMax(100);

	mClockFilterInterface.reset(new AnalyzerSettingInterfaceInteger());
	mClockFilterInterface->SetTitleAndTooltip("Latch window (us)", "Standard NES and SNES Analyzer");
	mClockFilterInterface->SetMin(0);
	mClockFilterInterface->SetMax(100);

	AddInterface( mLatchChannelInterface.get() );
	AddInterface(mClockChannelInterface.get());
	AddInterface(mD0ChannelInterface.get());
	AddInterface(mD1ChannelInterface.get());
	AddInterface(mConsoleInterface.get());
	AddInterface(mLatchWindowInterface.get());
	AddInterface(mClockFilterInterface.get());

	AddExportOption( 0, "Export as text/csv file" );
	AddExportExtension( 0, "text", "txt" );
	AddExportExtension( 0, "csv", "csv" );

	ClearChannels();
	AddChannel( mLatchChannel, "Latch", false );
	AddChannel(mClockChannel, "Clock", false);
	AddChannel(mD0Channel, "D0", false);
	AddChannel(mD1Channel, "D1", false);
}

NES_SNESAnalyzerSettings::~NES_SNESAnalyzerSettings()
{
}

bool NES_SNESAnalyzerSettings::SetSettingsFromInterfaces()
{
	mLatchChannel = mLatchChannelInterface->GetChannel();
	mClockChannel = mClockChannelInterface->GetChannel();
	mD0Channel = mD0ChannelInterface->GetChannel();
	mD1Channel = mD1ChannelInterface->GetChannel();
	mConsole = (ConsoleType)(unsigned int)mConsoleInterface->GetNumber();
	mLatchWindow = mLatchWindowInterface->GetInteger();
	mClockFilter = mClockFilterInterface->GetInteger();

	ClearChannels();
	AddChannel( mLatchChannel, "Latch", true );
	AddChannel(mClockChannel, "Clock", true);
	AddChannel(mD0Channel, "D0", true);
	AddChannel(mD1Channel, "D1", true);

	return true;
}

void NES_SNESAnalyzerSettings::UpdateInterfacesFromSettings()
{
	mLatchChannelInterface->SetChannel( mLatchChannel );
	mClockChannelInterface->SetChannel(mClockChannel);
	mD0ChannelInterface->SetChannel(mD0Channel);
	mD1ChannelInterface->SetChannel(mD1Channel);
	mConsoleInterface->SetNumber(mConsole);
	mLatchWindowInterface->SetInteger(mLatchWindow);
	mClockFilterInterface->SetInteger(mClockFilter);
}

void NES_SNESAnalyzerSettings::LoadSettings( const char* settings )
{
	SimpleArchive text_archive;
	text_archive.SetString( settings );

	text_archive >> mLatchChannel;
	text_archive >> mClockChannel;
	text_archive >> mD0Channel;
	text_archive >> mD1Channel;

	ClearChannels();
	AddChannel( mLatchChannel, "Latch", true );
	AddChannel(mClockChannel, "Clock", true);
	AddChannel(mD0Channel, "D0", true);
	AddChannel(mD1Channel, "D1", true);

	UpdateInterfacesFromSettings();
}

const char* NES_SNESAnalyzerSettings::SaveSettings()
{
	SimpleArchive text_archive;

	text_archive << mLatchChannel;
	text_archive << mClockChannel;
	text_archive << mD0Channel;
	text_archive << mD1Channel;

	return SetReturnString( text_archive.GetString() );
}
