import std.io;


// Header file
struct Header
{

    char magic[4];
    u32 version;
    u32 objsCount;
    
};


// Tipo di oggetto
enum ObjectType : u32
{

    Cube     = 0x01,
    Plane    = 0x02,
    Prop     = 0x03,
    Player   = 0x04,
    Trigger  = 0x05,
    Interact = 0x06,
    TestEnt  = 0x07,
    MoveObj  = 0x08
    
};


//=======TIPI DI OGGETTO===================================


struct ObjectCube
{

    u64   ID;
    float positionX;
    float positionY;
    float positionZ;
    float rotationX;
    float rotationY;
    float rotationZ;
    float width;
    float height;
    float depth;
    u32   texNameLenght;
    char  texName[texNameLenght];
    u32   isDoubleTiled;
    u32   usePhysics;
    
};


struct ObjectPlane
{

    u64   ID;
    float positionX;
    float positionY;
    float positionZ;
    float rotationX;
    float rotationY;
    float rotationZ;
    float width;
    float height;
    u32   texNameLenght;
    char  texName[texNameLenght];
    u32   isDoubleTiled;
    
};


struct ObjectProp
{

    u64   ID;
    float positionX;
    float positionY;
    float positionZ;
    float rotationX;
    float rotationY;
    float rotationZ;
    u32   modelNameLenght;
    char  modelName[modelNameLenght];
    u32   usePhysics;
    
};


struct ObjectPlayer
{

    u64   ID;
    float positionX;
    float positionY;
    float positionZ;
    float rotationY;
    
};


struct ObjectTriggerArea
{

    u64   ID;
    float positionX;
    float positionY;
    float positionZ;
    float rotationX;
    float rotationY;
    float rotationZ;
    float width;
    float height;
    float depth;
    u64   targetID;
    
};


struct ObjectInteractArea
{

    u64   ID;
    float positionX;
    float positionY;
    float positionZ;
    float rotationX;
    float rotationY;
    float rotationZ;
    float width;
    float height;
    float depth;
    u64   targetID;
    
};


struct ObjectTestEntity
{

    u64   ID;
    float garbageVal1;
    float garbageVal2;
    float garbageVal3;
    
};


struct ObjectMoveEntity
{
    u64 ID;
    u64 targetID;
    float desiredPosX;
    float desidedPosY;
    float desiredPosZ;
    u32 ignoreY;
};


//=======BASE OGGETTO===================================


struct Object
{

    ObjectType type;
    
    
    if(type == ObjectType::Cube)
    {
    
        ObjectCube obj;
        
    }
    else if(type == ObjectType::Plane)
    {
    
        ObjectPlane obj;
        
    }
    else if(type == ObjectType::Prop)
    {
    
        ObjectProp obj;
        
    }
    else if(type == ObjectType::Player)
    {
    
        ObjectPlayer obj;
        
    }
    else if(type == ObjectType::Trigger)
    {
    
        ObjectTriggerArea obj;
        
    }
    else if(type == ObjectType::Interact)
    {
    
        ObjectInteractArea obj;
        
    }
    else if(type == ObjectType::TestEnt)
    {
    
        ObjectTestEntity obj;
        
    }
    else if(type == ObjectType::MoveObj)
    {
    
        ObjectMoveEntity obj;
        
    }
    
};


Header header @ 0x00;
Object objects[header.objsCount] @ 0x0C;