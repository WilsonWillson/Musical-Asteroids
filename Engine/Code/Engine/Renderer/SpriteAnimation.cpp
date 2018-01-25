#include "Engine/Renderer/SpriteAnimation.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"


SpriteAnimation::SpriteAnimation(const SpriteSheet& spriteSheet, float durationSeconds, SpriteAnimMode playbackMode, int startSpriteIndex, int endSpriteIndex, Vector2 animationCords)
	:m_elapsedSeconds(0.f),
	m_isFinished(false),
	m_isPlaying(false),
	m_currentSpriteIndex(0),
	m_SecondsSinceLastFrame(0.f),
	m_spriteSheet(spriteSheet),
	m_durationSeconds(durationSeconds),
	m_startSpriteIndex(startSpriteIndex),
	m_endSpriteIndex(endSpriteIndex),
	m_playbackMode(playbackMode),
	m_AnimationCords(animationCords)
{
	int numFrames = (endSpriteIndex - startSpriteIndex) + 1;
	m_SecondsPerFrame = durationSeconds / (float)numFrames;
}

void SpriteAnimation::Update(float deltaSeconds)
{
	if (deltaSeconds == 0)
		Pause();
	if (deltaSeconds > 0)
		Resume();

	if (m_elapsedSeconds >= m_durationSeconds && m_playbackMode == SPRITE_ANIM_MODE_PLAY_TO_END)
		m_isFinished = true;
	if (m_isFinished == true || m_isPlaying == false)
		return;

	m_elapsedSeconds += deltaSeconds;
	m_SecondsSinceLastFrame += deltaSeconds;

	if (m_SecondsSinceLastFrame > m_SecondsPerFrame) {
		++m_currentSpriteIndex;
		m_SecondsSinceLastFrame = 0;

		if (m_currentSpriteIndex > m_endSpriteIndex && m_playbackMode == SPRITE_ANIM_MODE_LOOPING)
		{
			m_currentSpriteIndex = 0;
		}
	}
}


AABB2 SpriteAnimation::GetCurrentTexCoords() const
{
	AABB2 textCords;
	int spriteY = m_currentSpriteIndex / m_spriteSheet.m_spriteLayout.x;
	int spriteX = m_currentSpriteIndex % m_spriteSheet.m_spriteLayout.x;
	textCords.m_mins.x = (float)spriteX / (float)m_spriteSheet.m_spriteLayout.x;
	textCords.m_mins.y = (float)spriteY / (float)m_spriteSheet.m_spriteLayout.y;
	textCords.m_maxs.x = (float)(spriteX + 1) / (float)m_spriteSheet.m_spriteLayout.x;
	textCords.m_maxs.y = (float)(spriteY + 1) / (float)m_spriteSheet.m_spriteLayout.y;
	return textCords;
}

Texture* SpriteAnimation::GetTexture() const
{
	return m_spriteSheet.m_spriteSheetTexture;
}

void SpriteAnimation::Start()
{
	m_isPlaying = true;
	m_currentSpriteIndex = m_startSpriteIndex;
}

void SpriteAnimation::Pause()
{
	m_isPlaying = false;
}

void SpriteAnimation::Resume()
{
	m_isPlaying = true;
}

void SpriteAnimation::Reset()
{
	m_elapsedSeconds = 0;
}

float SpriteAnimation::GetFractionElapsed() const
{
	return m_elapsedSeconds / m_durationSeconds;
}

float SpriteAnimation::GetFractionRemaining() const
{
	return (m_durationSeconds - m_elapsedSeconds) / m_durationSeconds;
}

void SpriteAnimation::SetSecondsElapsed(float secondsElapsed)
{
	m_elapsedSeconds = secondsElapsed;
}

void SpriteAnimation::SetFractionElapsed(float fractionElapsed)
{
	m_elapsedSeconds = fractionElapsed * m_durationSeconds;
}

void SpriteAnimation::SetAnimationCords(Vector2 animationCords)
{
	m_AnimationCords = animationCords;
}

