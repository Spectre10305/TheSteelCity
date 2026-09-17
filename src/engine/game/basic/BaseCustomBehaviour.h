#pragma once
#include <entt.hpp>
#include <string>
#include <glm/glm.hpp>
#include "../../physics/PhysicsManager.h" // RaycastHit
#include <functional>


namespace nothing
{

	// Function pointers per servizi dal codice dell'engine
	struct EngineServices
	{

		void (*PrintInfoMessage)(const std::string&);
		//bool (*Raycast)(const glm::vec3&, const glm::vec3&, RaycastHit&);
		std::function<bool(const glm::vec3&, const glm::vec3&, RaycastHit&)> Raycast;
		std::function<entt::entity(uint64_t)> ResolveEntityID;

	};


	class BaseCustomBehaviour
	{

	public:

		virtual ~BaseCustomBehaviour() = default;


		virtual void Create()                           {}
		virtual void Update(double deltaTime)           {}
		virtual void LateUpdate(double deltaTime)       {}
		virtual void Interact()                         {}
		virtual void TriggerTouch(entt::entity visitor) {}
		virtual void Destroy()                          {}

	protected:

		EngineServices* engineServices_ = nullptr;


		entt::registry* sceneRegistry_ = nullptr;
		entt::entity self_ = entt::null;


		double localTimer_ = 0.0;


		template<typename T>
		T& GetComponent() // Da questa stessa entità...
		{
			
			return sceneRegistry_->get<T>(self_);

		}


		template<typename T>
		T& GetComponent(entt::entity ent) // Da un'altra entità...
		{

			return sceneRegistry_->get<T>(ent);

		}


		template<typename T>
		T& GetContextComponent() // Dal registry context di EnTT
		{

			return sceneRegistry_->ctx().get<T>();

		}


		void         PrintInfoMessage(const std::string& message)                                     { engineServices_->PrintInfoMessage(message); }
		bool         Raycast(const glm::vec3& origin, const glm::vec3& direction, RaycastHit& outRay) { return engineServices_->Raycast(origin, direction, outRay); }
		entt::entity ResolveEntityID(uint64_t entID)                                                  { return engineServices_->ResolveEntityID(entID); }

		void SetRegistry(entt::registry& r) { sceneRegistry_ = &r; }
		void SetEntity(entt::entity e) { self_ = e; }


		friend class SceneManager;
		friend class GameContext;

	};

}