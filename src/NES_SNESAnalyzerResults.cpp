#include "NES_SNESAnalyzerResults.h"
#include <AnalyzerHelpers.h>
#include "NES_SNESAnalyzer.h"
#include "NES_SNESAnalyzerSettings.h"
#include <iostream>
#include <fstream>

NES_SNESAnalyzerResults::NES_SNESAnalyzerResults( NES_SNESAnalyzer* analyzer, NES_SNESAnalyzerSettings* settings )
:	AnalyzerResults(),
	mSettings( settings ),
	mAnalyzer( analyzer )
{
}

NES_SNESAnalyzerResults::~NES_SNESAnalyzerResults()
{
}

void NES_SNESAnalyzerResults::GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base )
{
	ClearResultStrings();
	Frame frame = GetFrame( frame_index );

	char number_str[128];
	memset(&number_str, 0, 128);
	sprintf(number_str, "Frame #%lu: ", (unsigned long)frame.mData2);
	size_t length = strlen(number_str);
	
	if (display_base == ASCII)
	{
		BitExtractor be(frame.mData1, AnalyzerEnums::MsbFirst, 32);

		for (int index = length; index < (32 + length) && index < 128; index++)
		{
			if (be.GetNextBit())
			{
				ConsoleType consoleType = mSettings->mConsole;

				if (consoleType == CONSOLE_NES)
				{
					switch (index-length)
					{
					case 0:
						number_str[index] = 'A';
						break;
					case 1:
						number_str[index] = 'B';
						break;
					case 2:
						number_str[index] = 's';
						break;
					case 3:
						number_str[index] = 'S';
						break;
					case 4:
						number_str[index] = 'U';
						break;
					case 5:
						number_str[index] = 'D';
						break;
					case 6:
						number_str[index] = 'L';
						break;
					case 7:
						number_str[index] = 'R';
						break;
						// O for overrread
					default:
						number_str[index] = 'O';
						break;
					}
				}
				else if (consoleType == CONSOLE_SNES)
				{
					switch (index-length)
					{
						case 0:
							number_str[index] = 'B';
							break;
						case 1:
							number_str[index] = 'Y';
							break;
						case 2:
							number_str[index] = 's';
							break;
						case 3:
							number_str[index] = 'S';
							break;
						case 4:
							number_str[index] = 'U';
							break;
						case 5:
							number_str[index] = 'D';
							break;
						case 6:
							number_str[index] = 'L';
							break;
						case 7:
							number_str[index] = 'R';
							break;
						case 8:
							number_str[index] = 'A';
							break;
						case 9:
							number_str[index] = 'X';
							break;
						case 10:
							number_str[index] = 'l';
							break;
						case 11:
							number_str[index] = 'r';
							break;
						case 12:
							number_str[index] = '1';
							break;
						case 13:
							number_str[index] = '2';
							break;
						case 14:
							number_str[index] = '3';
							break;
						case 15:
							number_str[index] = '4';
							break;
						// Show the letter 'O' for overrread
						default:
							number_str[index] = 'O';
							break;
					}
				}
			}
			else
			{
				number_str[index] = '.';
			}
		}
	}
	else
	{
		AnalyzerHelpers::GetNumberString(frame.mData1, display_base, 8, number_str, 128);
	}

	AddResultString( number_str );
}

void NES_SNESAnalyzerResults::GenerateExportFile( const char* file, DisplayBase display_base, U32 export_type_user_id )
{
	std::ofstream file_stream( file, std::ios::out );

	U64 trigger_sample = mAnalyzer->GetTriggerSample();
	U32 sample_rate = mAnalyzer->GetSampleRate();

	file_stream << "Time [s],Value" << std::endl;

	U64 num_frames = GetNumFrames();
	for( U32 i=0; i < num_frames; i++ )
	{
		Frame frame = GetFrame( i );
		
		char time_str[128];
		AnalyzerHelpers::GetTimeString( frame.mStartingSampleInclusive, trigger_sample, sample_rate, time_str, 128 );

		char number_str[128];
		AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 8, number_str, 128 );

		file_stream << time_str << "," << number_str << std::endl;

		if( UpdateExportProgressAndCheckForCancel( i, num_frames ) == true )
		{
			file_stream.close();
			return;
		}
	}

	file_stream.close();
}

void NES_SNESAnalyzerResults::GenerateFrameTabularText( U64 frame_index, DisplayBase display_base )
{
#ifdef SUPPORTS_PROTOCOL_SEARCH
	Frame frame = GetFrame( frame_index );
	ClearTabularText();

	char number_str[128];
	AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 8, number_str, 128 );
	AddTabularText( number_str );
#endif
}

void NES_SNESAnalyzerResults::GeneratePacketTabularText( U64 packet_id, DisplayBase display_base )
{
	//not supported

}

void NES_SNESAnalyzerResults::GenerateTransactionTabularText( U64 transaction_id, DisplayBase display_base )
{
	//not supported
}