#include <iostream>

using namespace MxEngine;

enum class AgentMode
{
    Teleport,
    TeleportAndRotate
};

class NavigationAgent
{
    private:
    std::vector<MxEngine::Vector3> _routePositions;
    float _velocity;
    int _currentPoint;
    MxEngine::MxObject::Handle gameObject;
    
    public:
    bool isMoving;
    AgentMode moveMode;

    NavigationAgent()
    {
    }

    void Start(MxEngine::MxObject::Handle gameObject)
    {
        this->gameObject = gameObject;
        _routePositions = {};
        isMoving = false;
        _velocity = 0;
        _currentPoint = 0;
        this->_velocity = 0;
    }

    void Update()
    {
        if(isMoving && _routePositions.size() > _currentPoint)
        {
            Vector3 targetPositon = _routePositions[_currentPoint];
            Vector3 delta = targetPositon - gameObject->LocalTransform.GetPosition();
            Vector3 direction;

            if(Length2(delta) > 0.001)
            {
                direction = Normalize(delta);
            }
            else 
            {
                direction = Vector3(0, 0, 0);
                _currentPoint ++;
            }

            if(moveMode == AgentMode::TeleportAndRotate)
            {
                gameObject->LocalTransform.LookAt(targetPositon);
            }
            gameObject->LocalTransform.SetPosition(gameObject->LocalTransform.GetPosition() + direction *_velocity * Time::Delta());
        }
    }

    void AddRoutePosition(MxEngine::Vector3 position)
    {
        _routePositions.push_back(position);
    }

    void SetVelocity(float velocity)
    {
        velocity = abs(velocity);
        _velocity = velocity;
    }

    void PrintRoute()
    {
        for (int i = 0; i < _routePositions.size(); i++)
        {
            Logger::Log(VerbosityType::INFO, ("Position " + std::to_string(i) + " : (" + std::to_string(_routePositions[i].x) + " ; " + std::to_string(_routePositions[i].y) + " ; " + std::to_string(_routePositions[i].z) + ")").c_str());
        }
    }

    void FollowRoute(bool isResetCurrentPoint)
    {
        isMoving = true;
        if(isResetCurrentPoint)
        {
            _currentPoint = 0;
        }
    }
};
