#include <iostream>
#include <MxEngine.h>
#include <string>
#include <thread>
#include <cmath>
#include <vector>
#include <stdio.h>

#include "NavigationAgent.h"

#define PI 3.14159265358979323846

#define FREQUENCY 440.0 // Частота в Герцах (например, 440 Гц для ноты "Ля")
#define SAMPLERATE 44100.0 // Частота дискретизации в Герцах
#define DURATION 1.0 // Длительность сигнала в секундах

using namespace std::chrono_literals;

namespace Dron
{
    using namespace MxEngine;

    class MxApplication : public Application 
    {
	private:
		float DroneX = 0.0f, DroneY = 0.0f, DroneZ = 0.0f;
		float up_scale = 0.5f;
		double distance;
		MxObject::Handle cameraObject;
		MxObject::Handle drone;
		NavigationAgent _agent;
		MxObject::Handle towerObject0;
		MxObject::Handle towerObject1;
		MxObject::Handle towerObject2;
		MxObject::Handle towerObject3;
		MxObject::Handle towerObject4;

		// Функция для генерации синусоиды с учетом расстояния
		std::vector<double> generateRadioSignal(double frequency, double sampleRate, double duration, double distance) {
			int totalSamples = static_cast<int>(sampleRate * duration);
			std::vector<double> signal(totalSamples);

			// Изменяем амплитуду в зависимости от расстояния
			double amplitude = 1.0 / (1.0 + distance); // Амплитуда уменьшается с увеличением расстояния

			for (int i = 0; i < totalSamples; ++i) {
				double t = i / sampleRate; // Время в секундах
				signal[i] = amplitude * std::sin(2 * PI * frequency * t); // Значение синусоиды
			}

			return signal;
		}

		// Основная функция для отображения синусоиды в ImGui
		void ShowRadioSignalPlot(const std::vector<double>& signal) {
			ImGui::Begin("Radio Signal Plot"); // Создаем окно
		
			// Устанавливаем размер окна
			ImGui::SetWindowSize(ImVec2(350, 255)); // Устанавливаем размер окна (ширина, высота)
			
			// Преобразуем значения из double в float
			std::vector<float> floatSignal(signal.size());
			for (size_t i = 0; i < signal.size(); ++i) {
				floatSignal[i] = static_cast<float>(signal[i]);
			}

			// Отображаем синусоиду, устанавливаем размеры графика
			ImGui::PlotLines("Radio Signal", floatSignal.data(), static_cast<int>(floatSignal.size()), 0, nullptr, -1.0f, 1.0f, ImVec2(0, 200)); // Высота графика 200 пикселей

			ImGui::Text("Distance: %f", distance);
			ImGui::End();
		}

		// Функция для вычисления расстояния между двумя 3D точками
		inline double countDistance(const Vector3& a, const Vector3& b) {
			return std::sqrt(std::pow(b.x - a.x, 2) + std::pow(b.y - a.y, 2) + std::pow(b.z - a.z, 2));
		}

		void CreateMarker(Vector3 position)
		{
			auto sphere = MxObject::Create();
			sphere->AddComponent<MeshSource>(Primitives::CreateSphere());
			sphere->AddComponent<MeshRenderer>();
			sphere->AddComponent<SphereCollider>();
			sphere->LocalTransform.SetPosition(position);
			sphere->LocalTransform.SetScale({ 0.1f, 0.1f, 0.1f });
		}
			
		inline void DroneCreate()
		{
			drone = MxObject::Create();
			drone->Name = "Drone";
			drone->AddComponent<MeshSource>(AssetManager::LoadMesh("Models/Drone.fbx"));
			drone->AddComponent<MeshRenderer>(AssetManager::LoadMaterials("Models/Drone.fbx"));
			drone->LocalTransform.SetRotation({ 270.0f, 0.0f, 0.0f });
			drone->LocalTransform.SetScale({ 0.5f, 0.5f, 0.5f });
			drone->LocalTransform.SetPosition({ 0.0f, 0.0f, 0.0f });

			auto material = drone->AddComponent<MeshRenderer>()->GetMaterial();
            material->BaseColor = MakeVector3(1.0f, 0.0f, 0.0f);
		}
		
		inline void TowerCreate()
		{
		// TOWER 0
			towerObject0 = MxObject::Create();
			towerObject0->Name = "Tower1";
			towerObject0->AddComponent<MeshSource>(AssetManager::LoadMesh("Models/radio_tower.fbx"));
			towerObject0->AddComponent<MeshRenderer>(AssetManager::LoadMaterials("Models/radio_tower.fbx"));

			auto material0 = towerObject0->AddComponent<MeshRenderer>()->GetMaterial();
			material0->BaseColor = MakeVector3(1.0f, 0.0f, 0.0f);
			
			towerObject0->LocalTransform.SetPosition({ -1.393f, 0.571f, 1.098f });
			towerObject0->LocalTransform.SetRotation({ 270.0f, 0.0f, 0.0f });
			towerObject0->LocalTransform.SetScale({ 0.05f, 0.05f, 0.05f });

		// TOWER 1
			towerObject1 = MxObject::Create();
			towerObject1->Name = "Tower2";
			towerObject1->AddComponent<MeshSource>(AssetManager::LoadMesh("Models/radio_tower.fbx"));
			towerObject1->AddComponent<MeshRenderer>(AssetManager::LoadMaterials("Models/radio_tower.fbx"));

			auto material1 = towerObject1->AddComponent<MeshRenderer>()->GetMaterial();
			material1->BaseColor = MakeVector3(1.0f, 0.0f, 0.0f);
			
			towerObject1->LocalTransform.SetPosition({ -4.748f, 0.571f, -5.808f });
			towerObject1->LocalTransform.SetRotation({ 270.0f, 0.0f, 0.0f });
			towerObject1->LocalTransform.SetScale({ 0.05f, 0.05f, 0.05f });
		
		// TOWER 2
			towerObject2 = MxObject::Create();
			towerObject2->Name = "Tower3";
			towerObject2->AddComponent<MeshSource>(AssetManager::LoadMesh("Models/radio_tower.fbx"));
			towerObject2->AddComponent<MeshRenderer>(AssetManager::LoadMaterials("Models/radio_tower.fbx"));

			auto material2 = towerObject2->AddComponent<MeshRenderer>()->GetMaterial();
			material2->BaseColor = MakeVector3(1.0f, 0.0f, 0.0f);
			
			towerObject2->LocalTransform.SetPosition({ -1.393f, 0.411f, 9.399f });
			towerObject2->LocalTransform.SetRotation({ 270.0f, 0.0f, 0.0f });
			towerObject2->LocalTransform.SetScale({ 0.05f, 0.05f, 0.05f });
		
		// TOWER 3
			towerObject3 = MxObject::Create();
			towerObject3->Name = "Tower4";
			towerObject3->AddComponent<MeshSource>(AssetManager::LoadMesh("Models/radio_tower.fbx"));
			towerObject3->AddComponent<MeshRenderer>(AssetManager::LoadMaterials("Models/radio_tower.fbx"));

			auto material3 = towerObject3->AddComponent<MeshRenderer>()->GetMaterial();
			material3->BaseColor = MakeVector3(1.0f, 0.0f, 0.0f);
			
			towerObject3->LocalTransform.SetPosition({ 7.440f, -0.004f, 1.098f });
			towerObject3->LocalTransform.SetRotation({ 270.0f, 0.0f, 0.0f });
			towerObject3->LocalTransform.SetScale({ 0.05f, 0.05f, 0.05f });
			
		// TOWER 4
			towerObject4 = MxObject::Create();
			towerObject4->Name = "Tower5";
			towerObject4->AddComponent<MeshSource>(AssetManager::LoadMesh("Models/radio_tower.fbx"));
			towerObject4->AddComponent<MeshRenderer>(AssetManager::LoadMaterials("Models/radio_tower.fbx"));

			auto material4 = towerObject4->AddComponent<MeshRenderer>()->GetMaterial();
			material4->BaseColor = MakeVector3(1.0f, 0.0f, 0.0f);
			
			towerObject4->LocalTransform.SetPosition({ 7.568f, 1.487f, 9.410f });
			towerObject4->LocalTransform.SetRotation({ 270.0f, 0.0f, 0.0f });
			towerObject4->LocalTransform.SetScale({ 0.07f, 0.07f, 0.07f });
		}

		inline void MapCreate() { auto mapObject = MxObject::Create();
			mapObject->Name = "Map";
			mapObject->AddComponent<MeshSource>(AssetManager::LoadMesh("Models/smallMounts.fbx"));
			mapObject->AddComponent<MeshRenderer>(AssetManager::LoadMaterials("Models/smallMounts.fbx"));
			
			mapObject->LocalTransform.SetRotation({ 270.0f, 0.0f, 0.0f });
			mapObject->LocalTransform.SetPosition({ 0.0f, 0.0f, 0.0f });
			mapObject->AddComponent<BoxCollider>();
		}

		inline void CamCreate()
		{
			cameraObject = MxObject::Create();
			cameraObject->Name = "Camera Object";
			cameraObject->LocalTransform.SetPosition({ 0.0f, 0.5f, 0.0f });
		
			auto controller = cameraObject->AddComponent<CameraController>();
			controller->ListenWindowResizeEvent();
			Rendering::SetViewport(controller);
			
			auto input = cameraObject->AddComponent<InputController>();
			input->BindMovement(KeyCode::W, KeyCode::A, KeyCode::S, KeyCode::D, KeyCode::SPACE, KeyCode::LEFT_SHIFT);
			input->BindRotation();
		}

		inline void LightInit()
		{
			auto pointLight = MxObject::Create();
            auto lightSource = pointLight->AddComponent<SpotLight>();
            
            lightSource->SetColor(Vector3(1.0f, 0.7f, 0.0f));
			lightSource->SetIntensity(100.0f);
            pointLight->LocalTransform.SetPosition({ 0.0f, 20.0f, 0.0f });
		}

		inline void GetCoord()
		{
			Vector3 cameraPosition = cameraObject->LocalTransform.GetPosition(); // Позиция камеры 

			std::cout << "Camera position: " << cameraPosition.x << " " << cameraPosition.y << " " << cameraPosition.z << " " << std::endl;

			_agent.AddRoutePosition(cameraPosition);
		}

	public:
		////////////////////Основные функции////////////////////
        virtual void OnCreate() override
        {
			CamCreate();

			MapCreate();

			TowerCreate();

			LightInit();

			DroneCreate();

			_agent.Start(drone);
			_agent.SetVelocity(2);
			_agent.moveMode = AgentMode::Teleport;
		}

        virtual void OnUpdate() override
        {
			_agent.Update();

			if (Input::IsKeyPressed(KeyCode::E)) {
				GetCoord();
			}
		
			if (Input::IsKeyPressed(KeyCode::ENTER)) {
				if(!_agent.isMoving) {
					_agent.FollowRoute(true);
				} else {
					_agent.isMoving = false;
				}
			}

			if (Input::IsKeyPressed(KeyCode::F)) {
				Scene::Save("scene.json");
				Logger::Log(VerbosityType::INFO, "scene save");
			}
			
			
			for (short i = 0; i < 5; i++) {

				switch(i) {
					case 0:
						distance = countDistance(drone->LocalTransform.GetPosition(), towerObject0->LocalTransform.GetPosition()) ;
						break;
					case 1:
						distance = countDistance(drone->LocalTransform.GetPosition(), towerObject1->LocalTransform.GetPosition()) ;
						break;
					case 2:
						distance = countDistance(drone->LocalTransform.GetPosition(), towerObject2->LocalTransform.GetPosition()) ;
						break;
					case 3:
						distance = countDistance(drone->LocalTransform.GetPosition(), towerObject3->LocalTransform.GetPosition()) ;
						break;
					case 4:
						distance = countDistance(drone->LocalTransform.GetPosition(), towerObject4->LocalTransform.GetPosition()) ;
						break;
				}

				if (distance <= 6) {
					std::vector<double> radioSignal = generateRadioSignal(FREQUENCY, SAMPLERATE, DURATION, distance);
					ShowRadioSignalPlot(radioSignal);
				}
			}

			auto droneCoord = drone->LocalTransform.GetPosition();
			
			ImGui::Begin("Drone coords");
			ImGui::InputFloat("X: ", &droneCoord.x);
			ImGui::InputFloat("Y: ", &droneCoord.y);
			ImGui::InputFloat("Z: ", &droneCoord.z);
			ImGui::End();
		
		}

        virtual void OnDestroy() override
        {
        }
		////////////////////////////////////////////////////////
	};
}

int main() {
    MxEngine::LaunchFromSourceDirectory();
    Dron::MxApplication app;
    app.Run();
    return 0;
}

