/****************************************************************************
Copyright (c) 2011-2013,WebJet Business Division,CYOU
 
http://www.genesis-3d.com.cn

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#include "stdneb.h"
#include "exception/exceptions.h"
#include "graphicsystem/Vision/RenderScene.h"
#include "basegamefeature/managers/sceneschedulemanager.h"
#include "graphicfeature/graphicsfeature.h"
#include "graphicfeature/components/cameracomponent.h"
#include "serialization/serializeserver.h"
#include "vegetationfeature/vegetationfeature.h"
#include "appframework/actormanager.h"
#include "Lightmap/lightmapSetting.h"
#include "profilesystem/ProfileSystem.h"

namespace App
{
	class CameraLoadListener : public App::CameraComponentLoadListener
	{
	public:
		CameraLoadListener()
		{

		}
		~CameraLoadListener()
		{

		}

		typedef Util::Array<CameraComponent*> CameraComponentList;

		virtual void OnLoad(CameraComponent* sender)
		{
			LoadCameraComList.Append(sender);
		}

		CameraComponent* GetLastCameraCom()
		{
			for(IndexT i = LoadCameraComList.Size() - 1; i >= 0; i--)
			{
				CameraComponent* pCom = LoadCameraComList[i];
				if(pCom 
					&& pCom->GetActor() 
					&& pCom->GetActor()->IsActive()
					)
				{
					return pCom;
				}
			}
			return NULL;
		}

		CameraComponentList LoadCameraComList;
	};

	static CameraLoadListener sCameraLoadListener;

	__ImplementClass(App::SceneScheduleManager, 'GSSM', App::Manager);
	__ImplementThreadSingleton(App::SceneScheduleManager);

	//------------------------------------------------------------------------
	SceneScheduleManager::SceneScheduleManager()
		: mMainRenderScene(NULL)
	{
		__ConstructThreadSingleton;
	}
	//------------------------------------------------------------------------
	SceneScheduleManager::~SceneScheduleManager()
	{
		__DestructThreadSingleton;			
		if (mMainRenderScene)
		{
			mMainRenderScene->Destroy();
			n_delete(mMainRenderScene);
			mMainRenderScene = NULL;
		}		
	}

	void SceneScheduleManager::Open()
	{
		n_assert(NULL == mMainRenderScene);
		mMainRenderScene = n_new(Graphic::RenderScene);
		mMainRenderScene->Setup();
		
	}

	void SceneScheduleManager::OnBeginFrame()
	{
		mScenesDelayDel.Clear();
	}

	void SceneScheduleManager::Close()
	{
		CloseAllScenes();
	}

	//------------------------------------------------------------------------
	SceneScheduleManager::ScenePtr SceneScheduleManager::OpenScene(const Util::String& sceneName, bool newScene, bool privateScene)
	{
#ifdef __PROFILER_IO__
		//begint tick
		PROFILER_IO_TICKBEGIN(tickBegin);
		PROFILER_IO_LOGTIMES("open scene begin, %s",sceneName.Get());
#endif
		ScenePtr scene = NULL;
		if (sceneName.Length() < 1)
		{
			n_warning("SceneScheduleManager::OpenScene(): scene file name invalid!\n" );
			return scene;	
		}

		sCameraLoadListener.LoadCameraComList.Clear();
		CameraComponent::RegisterLoadListener(&sCameraLoadListener);

		if ( newScene )
		{
			scene = App::Scene::Create();
			n_assert( scene );
			scene->_Open(privateScene);
			scene->SetName(sceneName);
		}
		else
		{
			scene = GetScene(sceneName);
			if(scene)
			{
				this->CloseScene(scene);
				scene = NULL;
			}
			
			Serialization::SerializationServer* pSerialzeMgr = Serialization::SerializationServer::Instance();
			n_assert( pSerialzeMgr );

			// - FT_UNKONW means:let the the function to distinguish the file type
			GPtr<Serialization::SerializeReader> pReader = pSerialzeMgr->OpenReadFile( sceneName, Serialization::FT_DEFAULT );
			if ( pReader )
			{
				try
				{
					Scene::_Load(pReader, scene, privateScene);
					
				}
				catch(const Exceptions::Exception& e)
				{
					if (scene.isvalid())
					{
						scene->_Close();
						scene = NULL;
					}
					n_warning( "SceneScheduleManager::OpenScene(): read %s failed:%s\n", sceneName.AsCharPtr(), e.what() );

				}
				pSerialzeMgr->CloseReadFile( pReader );		
			}
		}

		if (scene.isvalid())
		{
			mScenes.Append(scene);

			if( sCameraLoadListener.LoadCameraComList.Size() )
			{
				if(sCameraLoadListener.GetLastCameraCom())
				{
					Graphic::Camera* camera = sCameraLoadListener.GetLastCameraCom()->GetCameraObject();

					n_assert( camera && camera->GetRenderScene());
					camera->GetRenderScene()->SetDefaultCamera(camera);
				}
			}

			if (privateScene)
			{

				scene->_ApplyEnvironment();		
			}
			else
			{
				_SetMainScene(scene->GetName());
				scene->_ApplyEnvironment(true);

			}
		}
#ifdef __PROFILER_IO__
		PROFILER_IO_TICKEND(tickBegin,delta);
		PROFILER_IO_LOGTIMES("open scene end,time consume: %f ms",__tick_delta_delta/1000.f);
#endif
		return scene;
	}

	GPtr<Scene> SceneScheduleManager::GetScene(const Util::String& sceneName) const
	{
		Scenes::Iterator it = mScenes.Begin();
		Scenes::Iterator end = mScenes.End();
		while(it != mScenes.End())
		{
			if ((*it)->GetName() == sceneName)
			{
				return *it;
			}
			++it;
		}
		return NULL;
	}

	bool SceneScheduleManager::_SetMainScene(const Util::String& sceneName)
	{
		ScenePtr ptr = GetScene(sceneName);
		if (ptr)
		{
			mMainScene = ptr;
			return true;
		}
		return false;
	}

	bool SceneScheduleManager::CloseScene(const Util::String& sceneName)
	{
		Scenes::Iterator it = mScenes.Begin();
		Scenes::Iterator end = mScenes.End();
		while(it != mScenes.End())
		{
			if ((*it)->GetName() == sceneName)
			{
				(*it)->_Close();
				if (mMainScene == (*it))
				{
					mMainScene = NULL;
				}
				mScenesDelayDel.Append(*it);//只为了拿着智能指针的引用，防止触发析构，析构应该在下一帧再开始。
				mScenes.Erase(it);
				return true;
			}
			++it;
		}
		return false;
	}

	bool SceneScheduleManager::CloseScene(ScenePtr& scene)
	{
		Scenes::Iterator it = mScenes.Find(scene);
		if(it != mScenes.End())
		{
			(*it)->_Close();
			if (mMainScene == (*it))
			{
				mMainScene = NULL;
			}
			mScenesDelayDel.Append(scene);
			mScenes.Erase(it);
			return true;
		}
		return false;
	}

	void SceneScheduleManager::CloseAllScenes()
	{
		if ( VegetationFeature::HasInstance())
		{
			VegetationFeature::Instance()->ReActive();
		}
		Scenes::Iterator it = mScenes.Begin();
		Scenes::Iterator end = mScenes.End();
		while(it != mScenes.End())
		{
			(*it)->_Close();
			++it;
		}
		mScenesDelayDel.AppendArray(mScenes);
		mScenes.Clear();
		mMainScene = NULL;
		//释放lightmap资源
		LightmapSetting::GetInstance().ClearLightmapInfos();
	}
	//------------------------------------------------------------------------
	bool SceneScheduleManager::SaveScene(const Util::String& sceneFile )
	{
		return SaveScene(sceneFile, Serialization::FT_BINARY );
	}
	//------------------------------------------------------------------------
	bool SceneScheduleManager::SaveScene(const Util::String& sceneFile, int iFileType)
	{
		// - check if it is a valid file type
		if ( iFileType<0 || 
			iFileType>=Serialization::FT_NUM  )
		{
			return false;
		}

		bool ret = false;
		Serialization::FileType eFileType = static_cast<Serialization::FileType>( iFileType );
		ScenePtr scene = GetScene(sceneFile);
		if ( !scene.isvalid() || 
			sceneFile.Length() < 1)
		{	
			return ret;
		}	

		Serialization::SerializationServer* pSerialzeMgr = Serialization::SerializationServer::Instance();
		n_assert( pSerialzeMgr );

		GPtr<Serialization::SerializeWriter> pWriter = pSerialzeMgr->OpenWriteFile( sceneFile, eFileType );
		if ( pWriter )
		{
			pWriter->SerializeObject( scene );
			pSerialzeMgr->CloseWriteFile( pWriter );
			ret = true;
		}

		/// load terrain data
		const IndexT actorCount = scene->GetActorCount();
		for ( IndexT iActor = 0; iActor < actorCount; ++iActor )
		{
			const GPtr<Actor>& pActor = scene->GetActor( iActor );
			if ( pActor )
			{
				pActor->SaveTerrainFile();
			}
		}

		

		return ret;
	}

}