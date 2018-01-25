//---------------------------------------------------------------------------

#include "Engine/Audio/Audio.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "ThirdParty/fmod/fmod.hpp"
#include "ThirdParty/fmod/fmod.h"
#include "Engine/Core/Win32Includes.hpp"
#include <queue>
#include <iostream>
#include <fstream>



AudioSystem* g_AudioSystem = nullptr;

//---------------------------------------------------------------------------
// FMOD startup code based on "GETTING STARTED With FMOD Ex Programmer’s API for Windows" document
//	from the FMOD programming API at http://www.fmod.org/download/
//
AudioSystem::AudioSystem()
	: m_fmodSystem( nullptr )
{
	InitializeFMOD();
	m_CurrentSong = nullptr;
}


//-----------------------------------------------------------------------------------------------
AudioSystem::~AudioSystem()
{
	ShutDownFMOD();
}


//------------------------------Initalize/Shutdown---------------------------------------------
void AudioSystem::InitializeFMOD()
{
	const int MAX_AUDIO_DEVICE_NAME_LEN = 256;
	FMOD_RESULT result;
	unsigned int fmodVersion;
	int numDrivers;
	FMOD_SPEAKERMODE speakerMode;
	FMOD_CAPS deviceCapabilities;
	char audioDeviceName[ MAX_AUDIO_DEVICE_NAME_LEN ];


	// Create a System object and initialize.
	result = FMOD::System_Create( &m_fmodSystem );
	ValidateResult( result );

	result = m_fmodSystem->getVersion( &fmodVersion );
	ValidateResult( result );

	if( fmodVersion < FMOD_VERSION )
	{
		DebuggerPrintf( "Engine/Audio SYSTEM ERROR!  Your FMOD .dll is of an older version (0x%08x == %d) than that the .lib used to compile this code (0x%08x == %d).\n", fmodVersion, fmodVersion, FMOD_VERSION, FMOD_VERSION );
	}

	result = m_fmodSystem->getNumDrivers( &numDrivers );
	ValidateResult( result );

	if( numDrivers == 0 )
	{
		result = m_fmodSystem->setOutput( FMOD_OUTPUTTYPE_NOSOUND );
		ValidateResult( result );
	}
	else
	{
		result = m_fmodSystem->getDriverCaps( 0, &deviceCapabilities, 0, &speakerMode );
		ValidateResult( result );

		// Set the user selected speaker mode.
		result = m_fmodSystem->setSpeakerMode( speakerMode );
		ValidateResult( result );

		if( deviceCapabilities & FMOD_CAPS_HARDWARE_EMULATED )
		{
			// The user has the 'Acceleration' slider set to off! This is really bad
			// for latency! You might want to warn the user about this.
			result = m_fmodSystem->setDSPBufferSize( 128, 10 );
			ValidateResult( result );
		}

		result = m_fmodSystem->getDriverInfo( 0, audioDeviceName, MAX_AUDIO_DEVICE_NAME_LEN, 0 );
		ValidateResult( result );

		if( strstr( audioDeviceName, "SigmaTel" ) )
		{
			// Sigmatel sound devices crackle for some reason if the format is PCM 16bit.
			// PCM floating point output seems to solve it.
			result = m_fmodSystem->setSoftwareFormat( 44100, FMOD_SOUND_FORMAT_PCM16, 0,0, FMOD_DSP_RESAMPLER_SPLINE);
			ValidateResult( result );
		}
	}

	result = m_fmodSystem->init( 100, FMOD_INIT_NORMAL, 0 );
	if( result == FMOD_ERR_OUTPUT_CREATEBUFFER )
	{
		// Ok, the speaker mode selected isn't supported by this sound card. Switch it
		// back to stereo...
		result = m_fmodSystem->setSpeakerMode( FMOD_SPEAKERMODE_STEREO );
		ValidateResult( result );

		// ... and re-init.
		result = m_fmodSystem->init( 100, FMOD_INIT_NORMAL, 0 );
		ValidateResult( result );
	}
}



void AudioSystem::ShutDownFMOD()
{
//	FMOD_RESULT result = FMOD_OK;
//	result = FMOD_System_Close( m_fmodSystem );
//	result = FMOD_System_Release( m_fmodSystem );
	m_fmodSystem = nullptr;
}


//-----------------------------------Create Sound----------------------------------------
SoundID AudioSystem::CreateOrGetSound( const std::string& soundFileName )
{
	std::map< std::string, SoundID >::iterator found = m_registeredSoundIDs.find( soundFileName );
	if( found != m_registeredSoundIDs.end() )
	{
		return found->second;
	}
	else
	{
		FMOD::Sound* newSound = nullptr;
		m_fmodSystem->createSound( soundFileName.c_str(), FMOD_SOFTWARE, nullptr, &newSound );
		if( newSound )
		{
			SoundID newSoundID = (SoundID)m_registeredSounds.size();
			m_registeredSoundIDs[ soundFileName ] = newSoundID;
			m_registeredSounds.push_back( newSound );

			GetTagData(soundFileName);
			return newSoundID;
		}
	}

	return MISSING_SOUND_ID;
}

void AudioSystem::GetTagData(const std::string& soundFileName)
{
	TAGdata tagStruct;
	//int fileNameLength = 1024;
	int mp3TagSize = 128;

	//char  fileName[1025];
	char buffer[129];

	std::fstream mp3File;
	mp3File.open(soundFileName);
	buffer[mp3TagSize] = '\0';  //just a precaution


	//move file pointer to 128 bytes off the end of the file.
	mp3File.seekg(-(mp3TagSize), mp3File.end);

	if (mp3File.fail()) 
			return;


		//get the data
	mp3File.read(reinterpret_cast <char *>(&tagStruct), mp3TagSize);
	if (mp3File.fail()) 
			return;

	mp3File.close();

}

//-----------------------------Play and Stop----------------------------------------------
AudioChannelHandle AudioSystem::PlaySound( SoundID soundID, float volumeLevel )
{
	unsigned int numSounds =(unsigned int) m_registeredSounds.size();
	if( soundID < 0 || soundID >= numSounds )
		return nullptr;

	FMOD::Sound* sound = m_registeredSounds[ soundID ];
	if( !sound )
		return nullptr;

	FMOD::Channel* channelAssignedToSound = nullptr;
	m_fmodSystem->playSound( FMOD_CHANNEL_FREE, sound, false, &channelAssignedToSound );
	if( channelAssignedToSound )
	{
		channelAssignedToSound->setVolume( volumeLevel );
		//channelAssignedToSound->setLoopCount(3);
	}
	m_registeredChannelsForSoundIDs[soundID] = channelAssignedToSound;

	RestartVariables();

	return (AudioChannelHandle) channelAssignedToSound;
}

void AudioSystem::RestartVariables()
{
	for (int i = 0; i < SAMPLE_SIZE / 2; i++)
	{
		m_AudioSpike[i] = 0;
	}

	for (int i = 0; i < BPM_DETECTION; i++)
	{
		m_TimesBeatDetected[i] = 0;
	}

	m_BPMCounter = 0;
	m_DeltaSecondsOfCurrentSong = 0;
	m_IsPlayingSong = true;
}


void AudioSystem::StopChannel(AudioChannelHandle channel)
{
	if (channel != nullptr)
	{
		FMOD::Channel* fmodChannel = (FMOD::Channel*) channel;
		FMOD_RESULT modresult = fmodChannel->stop();

		delete fmodChannel;
	}
}




//---------------------------------------------------------------------------
void AudioSystem::Update()
{
	FMOD_RESULT result = m_fmodSystem->update();
	GUARANTEE_OR_DIE(result == FMOD_OK, Stringf("Engine/Audio SYSTEM ERROR: Got error result code %d.\n", result));
}

void AudioSystem::ValidateResult( FMOD_RESULT result )
{
	GUARANTEE_OR_DIE( result == FMOD_OK, Stringf( "Engine/Audio SYSTEM ERROR: Got error result code %d.\n", result ) );
}


//---------------------------Analysis Given Channel---------------------------

void AudioSystem::FrequencyAnalysis(AudioChannelHandle* channelHandle, float deltaSeconds)
{
	// getSpectrum() performs the frequency analysis, see explanation below
	int sampleSize = SAMPLE_SIZE;

	float *specLeft, *specRight;

	specLeft = new float[sampleSize];
	specRight = new float[sampleSize];


	// Get spectrum for left and right stereo channels
	//FMOD_System_GetSpectrum((FMOD_SYSTEM)m_fmodSystem, specLeft, sampleSize,0, FMOD_DSP_FFT_WINDOW_TRIANGLE);
	//FMOD_System_GetSpectrum((FFMOD_SYSTEM)m_fmodSystem, specRight, sampleSize, 0, FMOD_DSP_FFT_WINDOW_TRIANGLE);

	m_DeltaSecondsOfCurrentSong += deltaSeconds;

	FMOD::Channel *channel = (FMOD::Channel*)*channelHandle;
	channel->getSpectrum(specLeft, sampleSize, 0, FMOD_DSP_FFT_WINDOW_BLACKMAN);
	channel->getSpectrum(specRight, sampleSize, 1, FMOD_DSP_FFT_WINDOW_BLACKMAN);

	VolumeAnalysis(specLeft, specRight);
	DetermineAudioSpikes();
	BeatDetection();
	BPMDetection();




	if (m_Normalize)
		NormalizeVolume();
}

void AudioSystem::VolumeAnalysis(float * specLeft, float * specRight)
{
	float tempAverageVolumeLastFrame = 0.0f;
	float tempAverageBassVolumeLastFrame = 0.0f;
	float tempAverageMediumAllVolumeLastFrame = 0.0f;
	float tempAverageMediumLowVolumeLastFrame = 0.0f;
	float tempAverageMediumHighVolumeLastFrame = 0.0f;
	float tempAverageHighVolumeLastFrame = 0.0f;

	int tempBassCounter = 0;
	int tempMediumCounter = 0;
	int tempHighCounter = 0;

	for (int i = 0; i < SAMPLE_SIZE / 2; i++)
	{
		m_averageVolumeLastFrame[i] = (specLeft[i] + specRight[i]);
		tempAverageVolumeLastFrame += m_averageVolumeLastFrame[i];

		if (i <BASS_SIZE)
			AnalyseBass(tempAverageBassVolumeLastFrame, m_averageVolumeLastFrame[i], tempBassCounter);
		if (i < MEDIUMALL_SIZE && i > BASS_SIZE)
			AnalyseMedium(tempAverageMediumAllVolumeLastFrame, tempAverageMediumLowVolumeLastFrame, tempAverageMediumHighVolumeLastFrame, m_averageVolumeLastFrame[i], tempMediumCounter);
		if (i <= SAMPLE_SIZE && i > MEDIUMALL_SIZE)
			AnalyseHigh(tempAverageHighVolumeLastFrame, m_averageVolumeLastFrame[i], tempHighCounter);
	}

	IsAverageVolumeIncreasing(tempAverageVolumeLastFrame, tempAverageBassVolumeLastFrame, tempAverageMediumAllVolumeLastFrame, 
		tempAverageMediumHighVolumeLastFrame, tempAverageMediumLowVolumeLastFrame, tempAverageHighVolumeLastFrame);

	m_averageVolumeOfWholeSongLastFrame = tempAverageVolumeLastFrame/SAMPLE_SIZE;
	m_averageBassVolumeOfWholeSongLastFrame = tempAverageBassVolumeLastFrame / BASS_SIZE;
	m_averageMediumAllVolumeOfWholeSongLastFrame = tempAverageMediumAllVolumeLastFrame / MEDIUMALL_SIZE;
	m_averageMediumHighVolumeOfWholeSongLastFrame = tempAverageMediumHighVolumeLastFrame / MEDIUMHIGH_SIZE;
	m_averageMediumLowVolumeOfWholeSongLastFrame = tempAverageMediumLowVolumeLastFrame / MEDIUMLOW_SIZE;
	m_averageHighVolumeOfWholeSongLastFrame = tempAverageHighVolumeLastFrame / HIGH_SIZE;

	if (m_averageVolumeOfWholeSongLastFrame < NO_SOUND_THRESHHOLD)
		m_WasThereNoVolumeLastFrame = true;
	else
		m_WasThereNoVolumeLastFrame = false;
}


void AudioSystem::AnalyseBass(float& tempAverageBassVolumeLastFrame, float averageVolumeForThatSampleBlock, int& bassCounter)
{
	tempAverageBassVolumeLastFrame += averageVolumeForThatSampleBlock;
	m_averageBassVolumeLastFrame[bassCounter] = averageVolumeForThatSampleBlock;

	bassCounter++;
}

void AudioSystem::AnalyseMedium(float& tempAverageMediumAllVolumeLastFrame, float& tempAverageMediumLowVolumeLastFrame, float& tempAverageMediumHighVolumeLastFrame,
	float averageVolumeForThatSampleBlock, int& tempMediumCounter)
{
	tempAverageMediumAllVolumeLastFrame += averageVolumeForThatSampleBlock;
	tempAverageMediumHighVolumeLastFrame += averageVolumeForThatSampleBlock;
	tempAverageMediumLowVolumeLastFrame += averageVolumeForThatSampleBlock;

	m_averageMediumAllVolumeLastFrame[tempMediumCounter] = averageVolumeForThatSampleBlock;

	if (tempMediumCounter < MEDIUMLOW_SIZE - MEDIUMHIGH_SIZE)
	{
		m_averageMediumHighVolumeLastFrame[tempMediumCounter] = averageVolumeForThatSampleBlock;
	}
	else
	{
		int newMediumCounter = MEDIUMHIGH_SIZE - tempMediumCounter;
		m_averageMediumLowVolumeLastFrame[newMediumCounter] = averageVolumeForThatSampleBlock;
	}
	tempMediumCounter++;
}

void AudioSystem::AnalyseHigh(float& tempAverageHighVolumeLastFrame, float averageVolumeForThatSampleBlock, int& tempHighCounter)
{
	tempAverageHighVolumeLastFrame += averageVolumeForThatSampleBlock;
	m_AverageHighVolumeLastFrame[tempHighCounter] = averageVolumeForThatSampleBlock;
	tempHighCounter++;
}


void AudioSystem::IsAverageVolumeIncreasing( float tempAverageVolumeLastFrame, float tempAverageBassVolumeLastFrame, float tempAverageMediumAllVolumeLastFrame,
	float tempAverageMediumHighVolumeLastFrame, float tempAverageMediumLowVolumeLastFrame, float tempAverageHighVolumeLastFrame)
{
	if (m_averageVolumeOfWholeSongLastFrame < tempAverageVolumeLastFrame / SAMPLE_SIZE)
	{
		if (m_isOverallVolumeIncreasing == false)
			m_HowManyFramesWasVolumeConsistent = 0;
		m_isOverallVolumeIncreasing = true;
		m_HowManyFramesWasVolumeConsistent++;
	}

	if (m_averageVolumeOfWholeSongLastFrame > tempAverageVolumeLastFrame / SAMPLE_SIZE)
	{
		if (m_isOverallVolumeIncreasing == true)
			m_HowManyFramesWasVolumeConsistent = 0;
		m_isOverallVolumeIncreasing = false;
		m_HowManyFramesWasVolumeConsistent++;
	}

	if (m_averageBassVolumeOfWholeSongLastFrame < tempAverageBassVolumeLastFrame / BASS_SIZE)
	{
		if (m_isBassVolumeIncreasing == false)
			m_HowManyFramesWasBassVolumeConsistent = 0;
		m_isBassVolumeIncreasing = true;
		m_HowManyFramesWasBassVolumeConsistent++;
	}

	if (m_averageBassVolumeOfWholeSongLastFrame > tempAverageBassVolumeLastFrame / BASS_SIZE)
	{
		if (m_isBassVolumeIncreasing == true)
			m_HowManyFramesWasBassVolumeConsistent = 0;
		m_isBassVolumeIncreasing = false;
		m_HowManyFramesWasBassVolumeConsistent++;
	}

	if (m_averageMediumAllVolumeOfWholeSongLastFrame < tempAverageMediumAllVolumeLastFrame / MEDIUMALL_SIZE)
	{
		if (m_isMediumAllVolumeIncreasing == false)
			m_HowManyFramesWasMediumAllVolumeConsistent = 0;
		m_isMediumAllVolumeIncreasing = true;
		m_HowManyFramesWasMediumAllVolumeConsistent++;
	}

	if (m_averageMediumAllVolumeOfWholeSongLastFrame > tempAverageMediumAllVolumeLastFrame / MEDIUMALL_SIZE)
	{
		if (m_isMediumAllVolumeIncreasing == true)
			m_HowManyFramesWasMediumAllVolumeConsistent = 0;
		m_isMediumAllVolumeIncreasing = false;
		m_HowManyFramesWasMediumAllVolumeConsistent++;
	}

	if (m_averageMediumHighVolumeOfWholeSongLastFrame < tempAverageMediumHighVolumeLastFrame / MEDIUMHIGH_SIZE)
	{
		if (m_isMediumHighVolumeIncreasing == false)
			m_HowManyFramesWasMediumHighVolumeConsistent = 0;
		m_isMediumHighVolumeIncreasing = true;
		m_HowManyFramesWasMediumHighVolumeConsistent++;
	}

	if (m_averageMediumHighVolumeOfWholeSongLastFrame > tempAverageMediumHighVolumeLastFrame / MEDIUMHIGH_SIZE)
	{
		if (m_isMediumHighVolumeIncreasing == true)
			m_HowManyFramesWasMediumHighVolumeConsistent = 0;
		m_isMediumHighVolumeIncreasing = false;
		m_HowManyFramesWasMediumHighVolumeConsistent++;
	}

	if (m_averageMediumLowVolumeOfWholeSongLastFrame < tempAverageMediumLowVolumeLastFrame / MEDIUMLOW_SIZE)
	{
		if (m_isMediumLowVolumeIncreasing == false)
			m_HowManyFramesWasMediumLowVolumeConsistent = 0;
		m_isMediumLowVolumeIncreasing = true;
		m_HowManyFramesWasMediumLowVolumeConsistent++;
	}

	if (m_averageMediumLowVolumeOfWholeSongLastFrame > tempAverageMediumLowVolumeLastFrame / MEDIUMLOW_SIZE)
	{
		if (m_isMediumLowVolumeIncreasing == true)
			m_HowManyFramesWasMediumLowVolumeConsistent = 0;
		m_isMediumLowVolumeIncreasing = false;
		m_HowManyFramesWasMediumLowVolumeConsistent++;
	}

	if (m_averageHighVolumeOfWholeSongLastFrame < tempAverageHighVolumeLastFrame / HIGH_SIZE)
	{
		if (m_isHighVolumeIncreasing == false)
			m_HowManyFramesWasHighVolumeConsistent = 0;
		m_isHighVolumeIncreasing = true;
		m_HowManyFramesWasHighVolumeConsistent++;
	}

	if (m_averageHighVolumeOfWholeSongLastFrame > tempAverageHighVolumeLastFrame / HIGH_SIZE)
	{
		if (m_isHighVolumeIncreasing == true)
			m_HowManyFramesWasHighVolumeConsistent = 0;
		m_isHighVolumeIncreasing = false;
		m_HowManyFramesWasHighVolumeConsistent++;
	}
}



void AudioSystem::DetermineAudioSpikes()
{

	if (m_averageVolumeLastFrame[0] > m_averageVolumeLastFrame[1] && m_averageVolumeLastFrame[1] > m_averageVolumeLastFrame[2])
		m_AudioSpike[0] += SPIKE_SOUND;
	else
		SubtractAudioSpike(m_AudioSpike[0]);
	

	if (m_averageVolumeLastFrame[1] > m_averageVolumeLastFrame[2] && m_averageVolumeLastFrame[2] > m_averageVolumeLastFrame[3] && m_averageVolumeLastFrame[1] > m_averageVolumeLastFrame[0])
			m_AudioSpike[1] += SPIKE_SOUND;
	else
		SubtractAudioSpike(m_AudioSpike[1]);


	for (int i = 2; i < SAMPLE_SIZE / 2; i++)
	{
		if (m_averageVolumeLastFrame[i] > m_averageVolumeLastFrame[i - 1] && m_averageVolumeLastFrame[i - 1] > m_averageVolumeLastFrame[i - 2] && m_averageVolumeLastFrame[i] > m_averageVolumeLastFrame[i + 1] && m_averageVolumeLastFrame[i + 1] > m_averageVolumeLastFrame[i + 2])
		{
			m_AudioSpike[i] += SPIKE_SOUND;

			if (m_AudioSpike[i] > 1)
				m_AudioSpike[i] = 1;
		}
			
		else
			SubtractAudioSpike(m_AudioSpike[i]);

	}
}


void AudioSystem::SubtractAudioSpike(float audioSpike)
{
	audioSpike -= SPIKE_SOUND_NEGATIVE;

	if (audioSpike < 0)
		audioSpike = 0;
}


void AudioSystem::BeatDetection()
{
	// Test for threshold volume being exceeded (if not currently ignoring track)
	if (m_averageBassVolumeOfWholeSongLastFrame >= BEAT_DETECTION_THRESHOLD)
	{
		m_BeatDetection = true;
		AddBeatToBPMDetection();

	}
	else if (m_BeatDetection == false && m_averageBassVolumeOfWholeSongLastFrame <= BEAT_DETECTION_TO_LOW_TO_FACTOR &&
				m_averageMediumAllVolumeOfWholeSongLastFrame <= BEAT_DETECTION_THRESHOLD)
	{
		m_BeatDetection = true;
		AddBeatToBPMDetection();
	}
	else
	{
		m_BeatDetection = false;
	}

}

void AudioSystem::AddBeatToBPMDetection()
{
	m_TimesBeatDetected[m_BPMCounter] = m_DeltaSecondsOfCurrentSong;

	m_BPMCounter++;
	if (m_BPMCounter > BPM_DETECTION)
		m_BPMCounter = 0;
}

void AudioSystem::BPMDetection()
{
	float BPMAverage = 0;

	for (int i = 0; i < BPM_DETECTION; i++)
	{
		if (m_TimesBeatDetected[i] != 0)
		{
			BPMAverage += m_TimesBeatDetected[i];
		}
	}

	float timeDifference = abs(m_TimesBeatDetected[BPM_DETECTION] - m_TimesBeatDetected[0]);
	if (timeDifference != 0)
	{
		float howManyBeatsDetetecdThisMinute = (60 / timeDifference);
		//m_BPM = ((15 * howManyBeatsDetetecdThisMinute)/ 9.1)/2;
		m_BPM = howManyBeatsDetetecdThisMinute/15;
	}
	else
		m_BPM = 0;
}



//-----------------------------------------------------------------------------------------------------------------------------

void AudioSystem::RenderGraphicsEqualizer(Renderer* renderSystem, BitMapFont* textSprites )
{

	//Volume Stats
	if (g_AudioSystem->m_WasThereNoVolumeLastFrame == true)
		renderSystem->DrawText2D(Vector2(950, 350), "NO VOLUME", 25, 100, Rgba(255, 255, 255, 255), textSprites);

	renderSystem->DrawText2D(Vector2(950, 300), "Average Volume: ", 25, 100, Rgba(255, 255, 255, 255), textSprites);
	renderSystem->DrawText2D(Vector2(950, 275), "Whole Song: ", 25, 100, Rgba(255, 255, 255, 255), textSprites);
	renderSystem->DrawText2D(Vector2(1050, 275), std::to_string(g_AudioSystem->m_averageVolumeOfWholeSongLastFrame), 25, 100, Rgba(255, 255, 255, 255), textSprites);
	renderSystem->DrawText2D(Vector2(950, 250), "Bass: ", 25, 100, Rgba(255, 255, 255, 255), textSprites);
	renderSystem->DrawText2D(Vector2(1050, 250), std::to_string(g_AudioSystem->m_averageBassVolumeOfWholeSongLastFrame), 25, 100, Rgba(255, 255, 255, 255), textSprites);
	renderSystem->DrawText2D(Vector2(950, 225), "All Mid: ", 25, 100, Rgba(255, 255, 255, 255), textSprites);
	renderSystem->DrawText2D(Vector2(1050, 225), std::to_string(g_AudioSystem->m_averageMediumAllVolumeOfWholeSongLastFrame), 25, 100, Rgba(255, 255, 255, 255), textSprites);
	renderSystem->DrawText2D(Vector2(950, 200), "High Mid: ", 25, 100, Rgba(255, 255, 255, 255), textSprites);
	renderSystem->DrawText2D(Vector2(1050, 200), std::to_string(g_AudioSystem->m_averageMediumHighVolumeOfWholeSongLastFrame), 25, 100, Rgba(255, 255, 255, 255), textSprites);
	renderSystem->DrawText2D(Vector2(950, 175), "Low Mid: ", 25, 100, Rgba(255, 255, 255, 255), textSprites);
	renderSystem->DrawText2D(Vector2(1050, 175), std::to_string(g_AudioSystem->m_averageMediumLowVolumeOfWholeSongLastFrame), 25, 100, Rgba(255, 255, 255, 255), textSprites);
	renderSystem->DrawText2D(Vector2(950, 150), "High: ", 25, 100, Rgba(255, 255, 255, 255), textSprites);
	renderSystem->DrawText2D(Vector2(1050, 150), std::to_string(g_AudioSystem->m_averageHighVolumeOfWholeSongLastFrame), 25, 100, Rgba(255, 255, 255, 255), textSprites);


	//Is Audio Increasing/Decreasing
	if (g_AudioSystem->m_isOverallVolumeIncreasing == true)
		renderSystem->DrawText2D(Vector2(1250, 300), "Volume Increasing for: ", 25, 150, Rgba(0, 0, 255, 255), textSprites);
	else
		renderSystem->DrawText2D(Vector2(1250, 300), "Volume Decreasing for: ", 25, 150, Rgba(255, 0, 0, 255), textSprites);
	renderSystem->DrawText2D(Vector2(1400, 300), std::to_string(g_AudioSystem->m_HowManyFramesWasVolumeConsistent), 25, 25, Rgba(255, 255, 255, 255), textSprites);

	if (g_AudioSystem->m_isBassVolumeIncreasing == true)
		renderSystem->DrawText2D(Vector2(1250, 275), "Bass Increasing for: ", 25, 150, Rgba(0, 0, 255, 255), textSprites);
	else
		renderSystem->DrawText2D(Vector2(1250, 275), "Bass Decreasing for: ", 25, 150, Rgba(255, 0, 0, 255), textSprites);
	renderSystem->DrawText2D(Vector2(1400, 275), std::to_string(g_AudioSystem->m_HowManyFramesWasBassVolumeConsistent), 25, 25, Rgba(255, 255, 255, 255), textSprites);

	if (g_AudioSystem->m_isMediumAllVolumeIncreasing == true)
		renderSystem->DrawText2D(Vector2(1250, 250), "Med All Increasing for: ", 25, 150, Rgba(0, 0, 255, 255), textSprites);
	else
		renderSystem->DrawText2D(Vector2(1250, 250), "Med All Decreasing for: ", 25, 150, Rgba(255, 0, 0, 255), textSprites);
	renderSystem->DrawText2D(Vector2(1400, 250), std::to_string(g_AudioSystem->m_HowManyFramesWasMediumAllVolumeConsistent), 25, 25, Rgba(255, 255, 255, 255), textSprites);

	if (g_AudioSystem->m_isMediumHighVolumeIncreasing == true)
		renderSystem->DrawText2D(Vector2(1250, 225), "Med High Increasing for: ", 25, 150, Rgba(0, 0, 255, 255), textSprites);
	else
		renderSystem->DrawText2D(Vector2(1250, 225), "Med High Decreasing for: ", 25, 150, Rgba(255, 0, 0, 255), textSprites);
	renderSystem->DrawText2D(Vector2(1400, 225), std::to_string(g_AudioSystem->m_HowManyFramesWasMediumHighVolumeConsistent), 25, 25, Rgba(255, 255, 255, 255), textSprites);

	if (g_AudioSystem->m_isMediumLowVolumeIncreasing == true)
		renderSystem->DrawText2D(Vector2(1250, 200), "Med Low Increasing for: ", 25, 150, Rgba(0, 0, 255, 255), textSprites);
	else
		renderSystem->DrawText2D(Vector2(1250, 200), "Med Low Decreasing for: ", 25, 150, Rgba(255, 0, 0, 255), textSprites);
	renderSystem->DrawText2D(Vector2(1400, 200), std::to_string(g_AudioSystem->m_HowManyFramesWasMediumLowVolumeConsistent), 25, 25, Rgba(255, 255, 255, 255), textSprites);

	if (g_AudioSystem->m_isHighVolumeIncreasing == true)
		renderSystem->DrawText2D(Vector2(1250, 175), "High Increasing for: ", 25, 150, Rgba(0, 0, 255, 255), textSprites);
	else
		renderSystem->DrawText2D(Vector2(1250, 175), "High Decreasing for: ", 25, 150, Rgba(255, 0, 0, 255), textSprites);
	renderSystem->DrawText2D(Vector2(1400, 175), std::to_string(g_AudioSystem->m_HowManyFramesWasHighVolumeConsistent), 25, 25, Rgba(255, 255, 255, 255), textSprites);



	//Volumeizer Visualizer
	int resolutionX = 1600;
	int blockGap = 4 / (SAMPLE_SIZE / 64);
	int blockWidth = (int)((resolutionX * 0.8f) / (SAMPLE_SIZE / 2) - blockGap);
	int blockMaxHeight = 850;

	Rgba bassColor = Rgba(255, 0, 0, 255);
	Rgba middlelowColor = Rgba(46, 139, 87, 255);
	Rgba middlehighColor = Rgba(0, 250, 154, 255);
	Rgba highColor = Rgba(135, 206, 250, 255);
	for (int b = 0; b < SAMPLE_SIZE / 2; b++)
	{
		Vector2 mins(resolutionX * 0.1f + (blockWidth + blockGap) * b, 0);
		Vector2 maxs(mins.x + blockWidth, blockMaxHeight * g_AudioSystem->m_averageVolumeLastFrame[b]);

		renderSystem->DrawAABB2(AABB2(mins, maxs), Rgba(255, 255, 255, 255));



		if (b < SAMPLE_SIZE * .015)
			renderSystem->DrawAABB2(AABB2(mins, maxs), bassColor);
		if (b > SAMPLE_SIZE * .015 && b < SAMPLE_SIZE * .135)
			renderSystem->DrawAABB2(AABB2(mins, maxs), middlelowColor);
		if (b > SAMPLE_SIZE * .135)
			renderSystem->DrawAABB2(AABB2(mins, maxs), middlehighColor);
		if (b > SAMPLE_SIZE  * .24)
			renderSystem->DrawAABB2(AABB2(mins, maxs), highColor);

	}

	//Volume Spikes
	for (int audioSpikeCounter = 0; audioSpikeCounter < SAMPLE_SIZE / 2; audioSpikeCounter++)
	{
		Vector2 mins(resolutionX * 0.1f + (blockWidth + blockGap) * audioSpikeCounter, 0);
		Vector2 maxs(mins.x + blockWidth, blockMaxHeight * g_AudioSystem->m_averageVolumeLastFrame[audioSpikeCounter]);

		Rgba spikeStrengthColor = Rgba(255, 255, 255, 255 * g_AudioSystem->m_AudioSpike[audioSpikeCounter]);
		//spikeStrengthColor.SetAsFloats(255, 255, 255, 255 * g_AudioSystem->m_AudioSpike[audioSpikeCounter]);
		//renderSystem->DrawAABB2(AABB2(mins, maxs), spikeStrengthColor);

	}

	if (g_AudioSystem->m_BeatDetection == true)
		renderSystem->DrawText2D(Vector2(1250, 350), "Beat Detected ", 25, 150, Rgba(255, 255, 255, 255), textSprites);


	renderSystem->DrawText2D(Vector2(1250, 325), "BPM: ", 25, 150, Rgba(255, 255, 255, 255), textSprites);
	renderSystem->DrawText2D(Vector2(1375, 325), std::to_string(g_AudioSystem->m_BPM), 25, 150, Rgba(255, 255, 255, 255), textSprites);
}




//------------------------Normalized Volume meaning loudest Channel is always 1------------------------------------------------
void AudioSystem::NormalizeVolume()
{
	// Find max volume
	auto maxIterator = std::max_element(&m_averageVolumeLastFrame[0], &m_averageVolumeLastFrame[SAMPLE_SIZE]);
	float maxVol = *maxIterator;

	// Normalize
	if (maxVol != 0)
		std::transform(&m_averageVolumeLastFrame[0], &m_averageVolumeLastFrame[SAMPLE_SIZE], &m_averageVolumeLastFrame[0], [maxVol](float dB) -> float { return dB / maxVol; });
}

