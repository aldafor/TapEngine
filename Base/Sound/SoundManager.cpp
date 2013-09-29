/*
 * SoundManager.cpp
 *
 *  Created on: 14/10/2010
 *      Author: Daniel Conde Linares
 *      Author: Cucu
 *      Author: Manu
 */

#include "SoundManager.h"

#include <FSystem.h>

SoundManager* SoundManager::m_pInstance = NULL;

SoundManager* SoundManager::getInstance() {
	if(m_pInstance == NULL) {
		m_pInstance = new SoundManager();
	}

	return m_pInstance;
}

SoundManager::SoundManager() {
	// TODO Auto-generated constructor stub
	//m_sounds = new Base::Collection::ArrayListT<Sound*>();
	m_iVolume = 20;
	m_sounds = new Base::Collection::HashMapT<int, Sound*>();
	if(IsFailed(m_sounds->Construct()))
	{
		AppLog("Error al crear los Sonidos");
	}

#ifdef __HERO_WAVE__
	//m_players = new std::list<Media::Player*>();
	m_players = new PlayersBuffer();
	if(!m_players)
	{
		AppLog("Error al crear los Player");
	}
#else
	m_player = NULL;
#endif

}

SoundManager::~SoundManager() {
	// TODO Auto-generated destructor stub
#ifdef __HERO_WAVE__
	if(!m_players->empty())
	{
		for(PlayersBuffer::iterator it = m_players->begin(); it != m_players->end(); ++it)
		{
			Media::Player* tempPlayer = NULL;
			tempPlayer = (*it).second;
			if(tempPlayer) {
				if(tempPlayer->GetState() == Media::PLAYER_STATE_PLAYING)
					tempPlayer->Stop();
				tempPlayer->Close();
				delete tempPlayer;
			}
		}
		m_players->clear();
	}
	delete m_players;
//	while(!m_players->empty())
//	{
//		Media::Player* tempPlayer = NULL;
//		tempPlayer = m_players->front();
//		m_players->pop_front();
//		if(tempPlayer) {
//			if(tempPlayer->GetState() == Media::PLAYER_STATE_PLAYING)
//				tempPlayer->Stop();
//			tempPlayer->Close();
//			delete tempPlayer;
//		}
//	}
//	delete m_players;
#else
	if(m_player->GetState() == Media::PLAYER_STATE_PLAYING)
		m_player->Stop();
	m_player->Close();
	delete m_player;
#endif

	Base::Collection::IMapEnumeratorT<int, Sound*>*  pMapEnum = m_sounds->GetMapEnumeratorN();
	while (pMapEnum->MoveNext() == E_SUCCESS)
	{
		Sound* tempSound = NULL;
		pMapEnum->GetValue(tempSound);
		//tempSound->data->Clear();
		//delete tempSound->data;
		if(tempSound) {
			delete tempSound;
			tempSound = NULL;
		}
	}

//	Sound* tempSound;
//	for (int i = 0; i < m_sounds->GetCount(); ++i)
//	{
//		m_sounds->GetAt(i, tempSound);
//		if(tempSound->data->GetState() == Media::PLAYER_STATE_PLAYING)
//			tempSound->data->Stop();
//		tempSound->data->Close();
//		delete tempSound->data;
//		delete tempSound;
//	}

	m_sounds->RemoveAll();

	delete m_sounds;

	AppLog("Destruyendo SoundManager");
}

void SoundManager::AddSound(int soundID, const Base::String& mediaLocalPath, bool looping)
{
	bool isAlreadyAdded = false;
	m_sounds->ContainsKey(soundID, isAlreadyAdded);
	if(!isAlreadyAdded)
	{
		Sound* newSound = new Sound();
		newSound->ID = soundID;
		newSound->mediaPath = mediaLocalPath;
		newSound->loop = looping;
		//newSound->player = NULL;
		//newSound->data = new Base::ByteBuffer();
		//const int BUFFER_SIZE_MAX = 1024;
		//newSound->data->Construct(BUFFER_SIZE_MAX);

//		Io::File file;
//		result r = E_SUCCESS;
//
//		// Create file
//		r = file.Construct(mediaLocalPath, L"r+");
//		if(IsFailed(r))
//			AppLog("Error en la lectura de archivo de sonido");
//
//		// Read
//		r = file.Read(*(newSound->data));
//		if(IsFailed(r))
//			AppLog("Error en el relleno del buffer del sonido");

		m_sounds->Add(soundID, newSound);

//		Sound* newSound = new Sound();
//		Media::Player* newPlayer = new Media::Player();
//		if (IsFailed(newPlayer->Construct(*this, 0)))
//			// TODO: Applog error
//			AppLog("Error al crear el sonido");
//
//		newSound->ID = soundID;
//		newSound->mediaPath = mediaLocalPath;
//		newSound->loop = looping;
//		newSound->data = newPlayer;
//
//		m_sounds->Add(soundID, newSound);
	}
}

void SoundManager::FreeSound(int soundID)
{
	Stop(soundID);
	Sound* tempSound = NULL;
	m_sounds->GetValue(soundID, tempSound);
	if(tempSound)
	{
		//tempSound->data->Clear();
		//delete tempSound->data;
		delete tempSound;
		tempSound = NULL;
	}
	m_sounds->Remove(soundID);
#ifdef __HERO_WAVE__
	AppLog("Sonido %i Liberado. Sonidos restantes: %i en buffer, %i en mapa", soundID, m_players->size(), m_sounds->GetCount());
#else
	AppLog("Sonido %i Liberado. Sonidos restantes: %i en mapa", soundID, m_sounds->GetCount());
#endif
}


void SoundManager::Play(int soundID)
{
	Base::String key(L"SilentMode");
	bool isSilentOut = false;
	//TODO hay que meter el derecho a cambiar setting
	if(IsFailed(System::SettingInfo::GetValue(key,isSilentOut)))
		AppLog("Error getting Silent Mode");

	if (!isSilentOut)
	{
		Sound* tempSound = NULL;
		m_sounds->GetValue(soundID, tempSound);
		if(tempSound)
		{
#ifdef __HERO_WAVE__
			if(m_players->size() < 5) {
#else
			if(m_player) {
				m_player->Stop();
				m_player->Close();
				delete m_player;
				AppLog("SoundManager: Player Deleted");
			}
#endif
				Media::Player* tempPlayer = new Media::Player();
				if (IsFailed(tempPlayer->Construct(*this, 0)))
					// TODO: Applog error
					AppLog("Error al crear el sonido");

				if (IsFailed(tempPlayer->OpenFile(tempSound->mediaPath, false)))
//				if (IsFailed(tempPlayer->OpenBuffer(*(tempSound->data), true)))
					// TODO: Applog error
					AppLog("Error al abrir el archivo de sonido %S", tempSound->mediaPath.GetPointer());

				if (IsFailed(tempPlayer->SetLooping(tempSound->loop)))
					// TODO: Applog error
					AppLog("Error al poner el sonido repetitivo");

				if (IsFailed(tempPlayer->SetVolume(m_iVolume)))
					// TODO: Applog error
					AppLog("Error al ajustar el volumen del sonido");

				tempPlayer->Play();
//				tempSound->player = tempPlayer;
#ifdef __HERO_WAVE__
				m_players->insert(PlayerEntry(soundID, tempPlayer));
				//m_players->push_back(tempPlayer);
			}
			else {
				AppLog("10 o más instancias de Player VIVAS!!");
			}
#else
			m_player = tempPlayer;
#endif

//				if (IsFailed(tempPlayer->data->OpenFile(tempSound->mediaPath, true)))
//					// TODO: Applog error
//					AppLog("Error al abrir el archivo de sonido %S", tempSound->mediaPath.GetPointer());
//
//				if (IsFailed(tempSound->data->SetLooping(tempSound->loop)))
//					// TODO: Applog error
//					AppLog("Error al poner el sonido repetitivo");
//
//				if (IsFailed(tempSound->data->SetVolume(m_iVolume)))
//					// TODO: Applog error
//					AppLog("Error al ajustar el volumen del sonido");
//
//				tempSound->data->Play();
//			}
		}
	}
}

void SoundManager::Stop(int soundID)
{
#ifdef __HERO_WAVE__
	std::pair<PlayersBuffer::iterator, PlayersBuffer::iterator> iter;
	iter = m_players->equal_range(soundID);
	for(PlayersBuffer::iterator it = iter.first; it != iter.second; ++it)
	{
		Media::Player* tempPlayer = NULL;
		tempPlayer = (*it).second;
		if(tempPlayer) {
			if(tempPlayer->GetState() == Media::PLAYER_STATE_PLAYING)
				tempPlayer->Stop();
			tempPlayer->Close();
			delete tempPlayer;
		}
	}
	m_players->erase(soundID);

//	Sound* tempSound = NULL;
//	m_sounds->GetValue(soundID, tempSound);
//	if(tempSound)
//	{
//		if(tempSound->player->GetState()==Media::PLAYER_STATE_PLAYING)
//			tempSound->player->Stop();
//		tempSound->player->Close();
//		m_players->remove(tempSound->player);
//		delete tempSound->player;
//		tempSound->player = NULL;
//	}
#else
	Sound* tempSound = NULL;
	m_sounds->GetValue(soundID, tempSound);
	if(tempSound)
	{
		if(m_player)
		{
			if(tempSound->player == m_player)
			{
				if(tempSound->player->GetState()==Media::PLAYER_STATE_PLAYING)
					tempSound->player->Stop();
				tempSound->player->Close();
				tempSound->player = NULL;
			}
		}
	}
#endif
}

void SoundManager::StopAllSounds()
{
	Base::Collection::IMapEnumeratorT<int, Sound*>*  pMapEnum = m_sounds->GetMapEnumeratorN();
	while (pMapEnum->MoveNext() == E_SUCCESS)
	{
		int soundID;
		pMapEnum->GetKey(soundID);
		Stop(soundID);
	}
}

void
SoundManager::DecreaseVolume()
{

	if (m_iVolume > 0)
		m_iVolume -= 5;

#ifdef __HERO_WAVE__
	if(!m_players->empty())
	{
		for(PlayersBuffer::iterator it = m_players->begin(); it != m_players->end(); ++it)
		{
			Media::Player* tempPlayer = NULL;
			tempPlayer = (*it).second;
			if(tempPlayer) {
				if(tempPlayer->GetState() == Media::PLAYER_STATE_PLAYING)
				{
					if (IsFailed(tempPlayer->SetVolume(m_iVolume)))
						// TODO: Applog error
						AppLog("Error al ajustar el volumen del sonido");
				}
			}
		}
		m_players->clear();
	}
//	std::list<Media::Player*>::iterator it;
//	for ( it = m_players->begin(); it != m_players->end(); it++ )
//	{
//		if ((*it)->GetState() == Media::PLAYER_STATE_PLAYING)
//		{
//			if (IsFailed((*it)->SetVolume(m_iVolume)))
//				// TODO: Applog error
//				AppLog("Error al ajustar el volumen del sonido");
//		}
//	}
#else
	if(m_player)
	{
		if (m_player->GetState() == Media::PLAYER_STATE_PLAYING)
		{
			if (IsFailed(m_player->SetVolume(m_iVolume)))
				// TODO: Applog error
				AppLog("Error al ajustar el volumen del sonido");
		}
	}
#endif

//	Sound* tempSound;
//	Base::Collection::IMapEnumeratorT<int, Sound*>*  pMapEnum = m_sounds->GetMapEnumeratorN();
//	while (pMapEnum->MoveNext() == E_SUCCESS)
//	{
//		pMapEnum->GetValue(tempSound);
//		if (tempSound->player->GetState() == Media::PLAYER_STATE_PLAYING)
//		{
//			if (IsFailed(tempSound->data->SetVolume(m_iVolume)))
//				// TODO: Applog error
//				AppLog("Error al ajustar el volumen del sonido");
//		}
//	}
	AppLog("Volume %i", m_iVolume);

}

void
SoundManager::IncreaseVolume()
{

	if (m_iVolume < 100)
			m_iVolume += 5;

#ifdef __HERO_WAVE__
	if(!m_players->empty())
	{
		for(PlayersBuffer::iterator it = m_players->begin(); it != m_players->end(); ++it)
		{
			Media::Player* tempPlayer = NULL;
			tempPlayer = (*it).second;
			if(tempPlayer) {
				if(tempPlayer->GetState() == Media::PLAYER_STATE_PLAYING)
				{
					if (IsFailed(tempPlayer->SetVolume(m_iVolume)))
						// TODO: Applog error
						AppLog("Error al ajustar el volumen del sonido");
				}
			}
		}
		m_players->clear();
	}
//	std::list<Media::Player*>::iterator it;
//	for ( it = m_players->begin(); it != m_players->end(); it++ )
//	{
//		if ((*it)->GetState() == Media::PLAYER_STATE_PLAYING)
//		{
//			if (IsFailed((*it)->SetVolume(m_iVolume)))
//				// TODO: Applog error
//				AppLog("Error al ajustar el volumen del sonido");
//		}
//	}
#else
	if(m_player)
	{
		if (m_player->GetState() == Media::PLAYER_STATE_PLAYING)
		{
			if (IsFailed(m_player->SetVolume(m_iVolume)))
				// TODO: Applog error
				AppLog("Error al ajustar el volumen del sonido");
		}
	}
#endif

//	Sound* tempSound;
//	Base::Collection::IMapEnumeratorT<int, Sound*>*  pMapEnum = m_sounds->GetMapEnumeratorN();
//	while (pMapEnum->MoveNext() == E_SUCCESS)
//	{
//		pMapEnum->GetValue(tempSound);
//		if (tempSound->data->GetState() == Media::PLAYER_STATE_PLAYING)
//		{
//			if (IsFailed(tempSound->data->SetVolume(m_iVolume)))
//				// TODO: Applog error
//				AppLog("Error al ajustar el volumen del sonido");
//		}
//	}
	AppLog("Volume %i", m_iVolume);

}

void SoundManager::OnPlayerOpened(result r)
{

}

void SoundManager::OnPlayerEndOfClip(void)
{
#ifdef __HERO_WAVE__
	// TODO Fixe to not remove all the end clips on the same event call (because should have a different call for all of them)
	for(PlayersBuffer::iterator it = m_players->begin(); it != m_players->end(); ++it)
	{
		Media::Player* tempPlayer = NULL;
		tempPlayer = (*it).second;
		if(tempPlayer) {
			if(tempPlayer->GetState() == Media::PLAYER_STATE_ENDOFCLIP)
			{
				tempPlayer->Close();
				AppLog("SoundManager: Player Closed");
				delete tempPlayer;
				tempPlayer = NULL;
				AppLog("SoundManager: Player Deleted");
				m_players->erase(it);
				AppLog("SoundManager: Player Removed from List");
				break;
			}
		}
	}

//	std::list<Media::Player*>::iterator it;
//	for ( it = m_players->begin(); it != m_players->end(); it++ )
//	{
//		if ((*it)->GetState() == Media::PLAYER_STATE_ENDOFCLIP)
//		{
//			(*it)->Close();
//			AppLog("SoundManager: Player Closed");
//			m_players->remove((*it));
//			AppLog("SoundManager: Player Removed from List");
//			delete (*it);
//			AppLog("SoundManager: Player Deleted");
//		}
//	}
#else
	if(m_player)
	{
		m_player->Close();
		AppLog("SoundManager: Player Closed");
		delete m_player;
		AppLog("SoundManager: Player Deleted");
		m_player = NULL;
	}
#endif
}
void SoundManager::OnPlayerInterrupted(void)
{

}

void SoundManager::OnPlayerReleased(void)
{

}

void SoundManager::OnPlayerBuffering(int percent)
{

}

void SoundManager::OnPlayerErrorOccurred(Media::PlayerErrorReason r)
{

}
