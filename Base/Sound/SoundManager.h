/*
 * SoundManager.h
 *
 *  Created on: 14/10/2010
 *      Author: Daniel Conde Linares
 *      Author: Cucu
 *      Author: Manu
 */

#ifndef SOUNDMANAGER_H_
#define SOUNDMANAGER_H_

#include <FBase.h>
#include <FMedia.h>
#include <FIo.h>
//#include <list>
#include <map>

using namespace Osp;

/**
 * A structure that represent a Sound
 */
struct Sound
{
	/*@{*/
	int ID;					/**< the Identifier */
	Base::String mediaPath;	/**< the resource path */
//	Media::Player* player;	/**< the player */
	/*@}*/
	/*@{*/
	bool loop;				/**< flag for repeat from the beginning after finish */
	/*@}*/
};

/**
 * @class SoundManager
 *
 * @brief Class for encapsulate the SoundManager functionality.
 *
 * A SoundManager storage a range of sounds that have different Identifiers for index it.
 * Every sound that want to be played MUST be added to the manager previously without exception.
 * The identifier that was gave to the sound when was added will be used for play it.
 *
 * TODO Add a flag for load the sound resource on play synchronously or asynchronously.
 */
class SoundManager :
	public Media::IPlayerEventListener {

public:
	static SoundManager* getInstance();
	virtual ~SoundManager();

	void AddSound(int soundID, const Base::String& mediaLocalPath, bool looping = false);
	void FreeSound(int soundID);

	void Play(int songID);
	void Stop(int songID);
	void StopAllSounds();

	void IncreaseVolume(void);
	void DecreaseVolume(void);

public:
	void OnPlayerOpened(result r);
	void OnPlayerEndOfClip(void);
	void OnPlayerInterrupted(void);
	void OnPlayerReleased(void);
	void OnPlayerBuffering(int percent);
	void OnPlayerErrorOccurred(Media::PlayerErrorReason r);

private:
	SoundManager();

private:
//	Base::Collection::ArrayListT<Sound*>* m_sounds;
	Base::Collection::HashMapT<int, Sound*>* m_sounds;

#ifdef __HERO_WAVE__
	typedef std::pair<int, Media::Player*> PlayerEntry;
	typedef std::multimap <int, Media::Player*> PlayersBuffer;
	PlayersBuffer* m_players;
	//std::list<Media::Player*>* m_players;
#else
	Media::Player* m_player;
#endif

	int m_iVolume;

private:
	static SoundManager* m_pInstance;

};

#endif /* SOUNDMANAGER_H_ */

