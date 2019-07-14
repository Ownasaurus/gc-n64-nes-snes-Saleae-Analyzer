#include "NES_SNESAnalyzerSettings.h"
#include <AnalyzerHelpers.h>


NES_SNESAnalyzerSettings::NES_SNESAnalyzerSettings()
:	mLatchChannel( UNDEFINED_CHANNEL ),
	mClockChannel(UNDEFINED_CHANNEL),
	mD0Channel(UNDEFINED_CHANNEL),
	mD1Channel(UNDEFINED_CHANNEL)
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
	mD1ChannelInterface->SetChannel(mD1Channel);


	AddInterface( mLatchChannelInterface.get() );
	AddInterface(mClockChannelInterface.get());
	AddInterface(mD0ChannelInterface.get());
	AddInterface(mD1ChannelInterface.get());

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

	ClearChannels();
	AddChannel( mLatchChannel, "NES and SNES Analyzer", true );
	AddChannel(mClockChannel, "NES and SNES Analyzer", true);
	AddChannel(mD0Channel, "NES and SNES Analyzer", true);
	AddChannel(mD1Channel, "NES and SNES Analyzer", true);

	return true;
}

void NES_SNESAnalyzerSettings::UpdateInterfacesFromSettings()
{
	mLatchChannelInterface->SetChannel( mLatchChannel );
	mLatchChannelInterface->SetChannel(mClockChannel);
	mLatchChannelInterface->SetChannel(mD0Channel);
	mLatchChannelInterface->SetChannel(mD1Channel);
}

void NES_SNESAnalyzerSettings::LoadSettings( const char* settings )
{
	SimpleArchive text_archive;
	text_archive.SetString( settings );

	text_archive >> mLatchChannel;

	ClearChannels();
	AddChannel( mLatchChannel, "NES and SNES Analyzer", true );
	AddChannel(mClockChannel, "NES and SNES Analyzer", true);
	AddChannel(mD0Channel, "NES and SNES Analyzer", true);
	AddChannel(mD1Channel, "NES and SNES Analyzer", true);

	UpdateInterfacesFromSettings();
}

const char* NES_SNESAnalyzerSettings::SaveSettings()
{
	SimpleArchive text_archive;

	text_archive << mLatchChannel;

	return SetReturnString( text_archive.GetString() );
}
