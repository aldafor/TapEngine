/*
 * ResourceManager.h
 *
 *  Created on: 23/11/2010
 *      Author: Daniel Conde Linares
 */

#ifndef RESOURCEMANAGER_H_
#define RESOURCEMANAGER_H_

#include <map>

template <class T>
class ResourceManager {
public:
	ResourceManager() {};
	virtual ~ResourceManager() { ReleaseAllResources(); };

	T* GetResource(const int ID);

	void ReleaseAllResources();
	void ReleaseResource(const int ID);

public:
	typedef std::map<const int, T*> ResourceMap;

protected:
	ResourceMap resourceList;
};

template<class T>
inline T *ResourceManager<T>::GetResource(const int ID)
{
	typename ResourceMap::iterator it;
	T* resource = NULL;
	it = resourceList.find(ID);
	if(it->first == ID)
	{
		resource = it->second;
	}
	return resource;
}

template<class T>
inline void ResourceManager<T>::ReleaseAllResources()
{
	while(resourceList.begin() != resourceList.end())
	{
		delete resourceList.begin()->second;
		resourceList.erase(resourceList.begin());
	}
}

template<class T>
inline void ResourceManager<T>::ReleaseResource(const int ID)
{
	typename ResourceMap::iterator it;
	it = resourceList.find(ID);
	if(it->first == ID)
	{
		delete it->second;
		resourceList.erase(resourceList.find(ID));
	}
}

#endif /* RESOURCEMANAGER_H_ */
