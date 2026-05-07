#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "Common.h"

typedef enum VisualEntityType
{
	SPRITE,
	TEXT,
	VIEW,
	RECTANGLE_SHAPE,
	CIRCLE_SHAPE,
	CONVEX_SHAPE,
	VERTEX_ARRAY,
	VERTEX_BUFFER,
}VisualEntityType;

typedef enum SoundEntityType
{
	SOUND,
	MUSIC
}SoundEntityType;

typedef struct VisualEntity
{
	VisualEntityType type;
	void* ptr;
	float drawPlan;
	struct VisualEntity* next;
}VisualEntity;

typedef struct SoundEntity
{
	SoundEntityType type;
	void* ptr;
	float volume;
}SoundEntity;

typedef struct AssetEntity
{
	void* ptr;
	char* file;
}AssetEntity;

typedef struct Element
{
	void* value;
	struct Element* next;
}Element;

typedef struct List
{
	Element* first;
}List;

typedef enum GameState
{
	MENU,
	GAME,
	GAME_OVER
}GameState;

typedef struct EntityManager
{
	sfImage* cursorImage;
	sfCursor* cursor;
	GameState gameState;
	sfRenderWindow* renderWindow;
	sfClock* clock;
	sfView* view;
	float viewZoom;

	AssetEntity* asset;
	int assetCount;
	int generalAssetCount;
	VisualEntity* visual;
	int visualCount;
	SoundEntity* sound;
	int soundCount;

	void** callocList;
	int callocListCount;
	List** listList;
	int listListCount;
}EntityManager;

void LoadEntityManager(void);
void Draw(void);

void* GetAsset(char* _file);

sfSprite* LoadBackground(sfTexture* _texture, float _scale);
sfSprite* CreateSprite(sfTexture* _texture, sfVector2f _position, float _scale, float _drawPlan);
sfText* CreateText(sfFont* _font, sfVector2f _position, unsigned _scale, float _drawPlan);
sfSound* CreateSound(sfSoundBuffer* _buffer, float _volume, sfBool _play);
sfMusic* CreateMusic(char* _fileMusic, float _volume, sfBool _play);
sfView* CreateView(sfVector2f _centre, float _zoom, float _drawPlan);
sfRectangleShape* CreateRectangleShape(sfFloatRect _rect, sfColor _fillColor, sfColor _outlineColor, float _drawPlan);
sfCircleShape* CreateCircleShape(sfFloatRect _circle, sfColor _fillColor, sfColor _outlineColor, float _drawPlan);
sfConvexShape* CreateConvexShape(sfVector2f* _points, size_t _pointCount, sfVector2f _position, sfColor _fillColor, sfColor _outlineColor, float _drawPlan);
sfVertexArray* CreateVertexArray(sfPrimitiveType _primitiveType, sfVertex* _vertices, size_t _vertexCount, float _drawPlan);
sfVertexBuffer* CreateVertexBuffer(sfPrimitiveType _primitiveType, sfVertexBufferUsage _usage, sfVertex* _vertices, unsigned _vertexCount, float _drawPlan);

void DestroyVisualEntity(void* _entity);
void DestroySoundEntity(void* _entity);
void DestroyAssetEntity(void* _entity);

void AddVisual(VisualEntityType _type, void* _ptr, float _drawPlan);
void ChangeDrawPlan(void* _ptr, float _drawPlan);
void ChangeVolume(float _volume);

void* Calloc(size_t _count, size_t _size);
void* Realloc(void* _block, size_t _size);
void Free(void* _ptr);

List* CreateList(void);
void RemoveList(List* _list);
unsigned GetListSize(List* _list);

Element* CreateElement(void* _value);
Element* GetElement(List* _list, unsigned _index);
void InsertElement(List* _list, Element* _element, unsigned _index);
void RemoveElement(List* _list, unsigned _index);

void Load(void);
void PollEvent(void);
void Update(void);
void Cleanup(void);

void SetViewCenter(sfVector2f _centre);
void SetViewZoom(float _zoom);
sfVector2f GetViewPosition(void);
sfVector2f GetViewSize(void);
void MoveView(sfVector2f _move);

void SetGameState(GameState _gameState);
sfRenderWindow* GetRenderWindow(void);
void ChangeFullSceen(void);
sfVector2f GetMousePositionToOrigin(void);
sfBool CompareIntRect(sfIntRect _intRect1, sfIntRect _intRect2);
#endif //!ENTITY_MANAGER_H