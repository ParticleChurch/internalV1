#pragma once
class GameEvent {
public:
    DECLARE_VIRTUAL_METHOD(const char*, GetName, 1, (), ());
    DECLARE_VIRTUAL_METHOD_CUSTOM_ARGS(int, GetInt, 6, (const char* keyName, int defaultValue = 0), (const char*, int), (keyName, defaultValue));
    DECLARE_VIRTUAL_METHOD_CUSTOM_ARGS(float, GetFloat, 8, (const char* keyName, float defaultValue = 0.0f), (const char*, float), (keyName, defaultValue));
    DECLARE_VIRTUAL_METHOD_CUSTOM_ARGS(const char*, GetString, 9, (const char* keyName, const char* defaultValue = ""), (const char*, const char*), (keyName, defaultValue));
    DECLARE_VIRTUAL_METHOD(void, SetString, 16, (const char* keyName, const char* value), (keyName, value));
};

class GameEventListener {
public:
    virtual ~GameEventListener() {}
    virtual void FireGameEvent(GameEvent* event) = 0;
    virtual int GetEventDebugId() { return 42; }
};

class GameEventManager {
public:
    DECLARE_VIRTUAL_METHOD_CUSTOM_ARGS(bool, AddListener, 3, (GameEventListener* listener, const char* name), (GameEventListener*, const char*, bool), (listener, name, false));
    DECLARE_VIRTUAL_METHOD(void, RemoveListener, 5, (GameEventListener* listener), (listener));
};