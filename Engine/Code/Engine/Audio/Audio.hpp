#pragma once


//---------------------------------------------------------------------------
#pragma comment( lib, "ThirdParty/fmod/fmodex_vc" ) // Link in the fmodex_vc.lib static library


//---------------------------------------------------------------------------
#include "ThirdParty/fmod/fmod.hpp"
//#include "ThirdParty/fmod/fmod.h"
#pragma warning( disable : 4244)
#include <string>
#include <vector>
#include <map>
#include <queue>

enum GenreCode {
	BLUES,
	CLASSICROCK,
	COUNTRY,
	DANCE,
    DISCO,
	FUNK,
	GRUNGE,
	HIPHOP,
	 JAZZ,
	 METAL,
	 NEWAGE,
	 OLDIES,
	 OTHER,
	 POP,
	 RB,
	 RAP,
	 REGGAE,
	 ROCK,
	 TECHNO,
	 INDUSTRIAL,
	 ALTERNATIVE,
	 DEATHMETAL,
	PRANKS,
	SOUNDTRACK,
	EUROTECHNO,
	AMBIENT,
	TRIPHOP,
	VOCAL,
	JAZZFUNK,
	FUSION,
	TRANCE,
	CLASSICAL,
	INSTRUMENTAL,
	ACID,
	HOUSE,
	SOUND,
	GOSPEL,
	NOISE,
	ALTERNATIVEROCK,
	BASS,
	PUNK,
	SPACE,
	MEDITATIVE,
	INSTRUMENTALPOP ,
	INSTRUMENTALROCKETHNIC, 
	GOTHIC ,
	DARKWAVE,
	TECHNO2,
	INDUSTRIAL2,
	ELECTRONIC,
	POPFOLK,
	EURODANCE,
	DREAM,
	SOUTHERNROCK,
	COMEDY, 
	CULT,  
	GANGSTA, 
	TOP40, 
	CHRISTIANRAP,
	POPFUNK, 
	JUNGLE,
	NATIVEUS,
	CABARET, 
	NEWWAVE, 
	PSYCHADELIC, 
	RAVE,
	SHOWTUNES,  
	TRAILER,
	LOFI ,
	TRIBAL,
	ACIDPUNK, 
	ACIDJAZZ, 
	POLKA, 
	RETRO, 
	MUSICAL, 
	ROCKROLL, 
	HARDROCK, 
	FOLK, 
	FOLKROCK,
	NATIONALFOLK, 
	SWING ,
	FASTFUSION ,
	BEBOB ,
	LATIN ,
	REVIVAL ,
	CELTIC,
	BLUEGRASS, 
	AVANTGARDE ,
	GOTHICROCK, 
	PROGRESSIVEROCK ,
	PSYCHEDELICROCK ,
	SYMPHONICROCK,
	SLOWROCK ,
	BIGBAND ,
	CHORUS ,
	EASYLISTENING, 
	ACOUSTIC, 
	HUMOR ,
	SPEECH ,
	CHANSON, 
	OPERA ,
	CHAMBERMUSIC, 
	SONATA ,
	SYMPHONY ,
	BOOTYBASS,
	PRIMUS,
	PORNGROOVE ,
	SATIRE ,
	SLOWJAM,
	CLUB,
	TANGO ,
	SAMBA ,
    FOLKLORE ,
    BALLAD,
	POWERBALLAD ,
    RHYTHMSOUL,
	FREESTYLE ,
	DUET ,
	PUNKROCK, 
	DRUMSOLO ,
	ACAPELLA ,
	EUROHOUSE,
	DANCEHALL,
	GOA ,
	DRUMBASS, 
	CLUBHOUSE, 
	HARDCORE, 
	TERROR,
	INDIE ,
	BRITPOP,
	NEGROPUNK,
	POLSKPUNK ,
	BEAT ,
	CHRISTIANGANGSTA ,
	HEAVYMETAL,
	BLACKMETAL, 
	CROSSOVER ,
	CONTEMPORARYC ,
	CHRISTIANROCK ,
	MERENGUE, 
	SALSA,
	THRASHMETAL ,
	ANIME,
	JPOP ,
	SYNTHPOPI ,
	UNKOWN,
};


//---------------------------------------------------------------------------
typedef unsigned int SoundID;
typedef void* AudioChannelHandle;
const unsigned int MISSING_SOUND_ID = 0xffffffff;



const int SAMPLE_SIZE = 256;
//const int SAMPLE_SIZE = 512;
//const int SAMPLE_SIZE = 1024;
//const int SAMPLE_SIZE = 2048;

const float SPIKE_SOUND = .0015f;
const float SPIKE_SOUND_NEGATIVE = .5f;
const float BEAT_DETECTION_THRESHOLD = .15f;
const float BEAT_DETECTION_TO_LOW_TO_FACTOR = .02f;

//The Max's these value can be, everything lower than this falls into the range of the variable.
const int BASS_SIZE = (int)(SAMPLE_SIZE * .015);
const int MEDIUMALL_SIZE = (int)(SAMPLE_SIZE * .24);
const int MEDIUMHIGH_SIZE = (int)(SAMPLE_SIZE * .135);
const int MEDIUMLOW_SIZE = MEDIUMALL_SIZE;
const int HIGH_SIZE = SAMPLE_SIZE/2 - MEDIUMALL_SIZE;

const float NO_SOUND_THRESHHOLD = .001f;
const int BPM_DETECTION = 15;


struct TAGdata
{
		char tag[3];
		char title[30];
		char artist[30];
		char album[30];
		char year[4];
		char comment[30];
		char genre;
};


class Renderer;
class BitMapFont;

/////////////////////////////////////////////////////////////////////////////
class AudioSystem
{
public:
	AudioSystem();
	virtual ~AudioSystem();

	SoundID CreateOrGetSound( const std::string& soundFileName );
	AudioChannelHandle PlaySound( SoundID soundID, float volumeLevel=1.f );
	void RestartVariables();
	void StopChannel( AudioChannelHandle channel );

	void Update();

	void FrequencyAnalysis(AudioChannelHandle* channelHandle, float deltaSeconds);
	void NormalizeVolume();

	void RenderGraphicsEqualizer(Renderer* renderSystem, BitMapFont* textSprites);

private:
	void GetTagData(const std::string& soundFileName);
	void AnalyseBass(float& tempAverageBassVolumeLastFrame, float averageVolumeForThatSampleBlock, int& bassCounter);
	void AnalyseMedium(float& tempAverageMediumAllVolumeLastFrame, float& tempAverageMediumLowVolumeLastFrame, float& tempAverageMediumHighVolumeLastFrame,
		float averageVolumeForThatSampleBlock, int& tempMediumCounter);
	void AnalyseHigh(float& tempAverageHighVolumeLastFrame, float averageVolumeForThatSampleBlock, int& tempHighCounter);

	void IsAverageVolumeIncreasing(float tempAverageVolumeLastFrame, float tempAverageBassVolumeLastFrame, float tempAverageMediumAllVolumeLastFrame,
		float tempAverageMediumHighVolumeLastFrame, float tempAverageMediumLowVolumeLastFrame, float tempAverageHighVolumeLastFrame);


	void VolumeAnalysis(float * specLeft, float * specRight);
	void DetermineAudioSpikes();
	void AddAudioSpike(float audioSpike, int index);
	void SubtractAudioSpike(float audioSpike);
	void BeatDetection();
	void AddBeatToBPMDetection();
	void BPMDetection();



protected:
	void InitializeFMOD();
	void ShutDownFMOD();
	void ValidateResult( FMOD_RESULT result );

public:
	float* m_averageVolumeLastFrame = new float[SAMPLE_SIZE];
	float* m_averageBassVolumeLastFrame = new float[BASS_SIZE];
	float* m_averageMediumAllVolumeLastFrame = new float[MEDIUMALL_SIZE];
	float* m_averageMediumLowVolumeLastFrame = new float[MEDIUMLOW_SIZE];
	float* m_averageMediumHighVolumeLastFrame = new float[MEDIUMHIGH_SIZE];
	float* m_AverageHighVolumeLastFrame = new float[HIGH_SIZE];

	float m_averageVolumeOfWholeSongLastFrame = 0;
	float m_averageBassVolumeOfWholeSongLastFrame = 0;
	float m_averageMediumAllVolumeOfWholeSongLastFrame = 0;
	float m_averageMediumHighVolumeOfWholeSongLastFrame = 0;
	float m_averageMediumLowVolumeOfWholeSongLastFrame = 0;
	float m_averageHighVolumeOfWholeSongLastFrame = 0;

	bool m_WasThereNoVolumeLastFrame = false;

	bool m_isOverallVolumeIncreasing = false;
	int	 m_HowManyFramesWasVolumeConsistent = 0;
	bool m_isBassVolumeIncreasing = false;
	int	 m_HowManyFramesWasBassVolumeConsistent = 0;
	bool m_isMediumAllVolumeIncreasing = false;
	int	 m_HowManyFramesWasMediumAllVolumeConsistent = 0;
	bool m_isMediumLowVolumeIncreasing = false;
	int	 m_HowManyFramesWasMediumLowVolumeConsistent = 0;
	bool m_isMediumHighVolumeIncreasing = false;
	int	 m_HowManyFramesWasMediumHighVolumeConsistent = 0;
	bool m_isHighVolumeIncreasing = false;
	int	 m_HowManyFramesWasHighVolumeConsistent = 0;

	float* m_AudioSpike = new float[SAMPLE_SIZE/2];
	//Maybe need to track in seperate variables how long spike has been happening or do it in there

	bool								m_BeatDetection = false;
	float								m_BPM = 0.0f;
	float* m_TimesBeatDetected = new float[BPM_DETECTION];
	float								m_DeltaSecondsOfCurrentSong;
	int									m_BPMCounter;
	TAGdata*							m_CurrentSong;
	bool								m_IsPlayingSong = false;

protected:
	FMOD::System*						m_fmodSystem;
	std::map< std::string, SoundID >	m_registeredSoundIDs;
	std::vector< FMOD::Sound* >			m_registeredSounds;
	std::map < SoundID, AudioChannelHandle> m_registeredChannelsForSoundIDs;
	bool								m_Normalize = false;
};

extern AudioSystem* g_AudioSystem;
