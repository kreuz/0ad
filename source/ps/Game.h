#ifndef _ps_Game_H
#define _ps_Game_H

// Kludge: Our included headers might want to subgroup the Game group, so do it
// here, before including the other guys
#include "ps/Errors.h"
ERROR_GROUP(Game);

#include "World.h"
#include "Simulation.h"
#include "Player.h"
#include "GameView.h"

#include "scripting/SynchedJSObject.h"

#include <vector>

// Hard player limit (not counting the Gaia player)
#define PS_MAX_PLAYERS 6

namespace PlayerArray_JS
{
	JSBool GetProperty( JSContext* cx, JSObject* obj, jsval id, jsval* vp );	
};

#define g_GameAttributes CGameAttributes::GetSingleton()
class CGameAttributes:
	public CSynchedJSObject<CGameAttributes>,
	public Singleton<CGameAttributes>
{
public:
	typedef void (UpdateCallback)(CStrW name, CStrW newValue, void *data);

private:
	friend JSBool PlayerArray_JS::GetProperty( JSContext* cx, JSObject* obj, jsval id, jsval* vp );

	virtual void Update(CStrW name, ISynchedJSProperty *attrib);
	
	UpdateCallback *m_UpdateCB;
	void *m_UpdateCBData;
	
	CPlayer::UpdateCallback *m_PlayerUpdateCB;
	void *m_PlayerUpdateCBData;
	
	jsval JSGetPlayers();

public:
	CStrW m_MapFile;
	uint m_NumPlayers;
	
	CGameAttributes();
	virtual ~CGameAttributes();
	
	void SetValue(CStrW name, CStrW value);
	
	inline void SetUpdateCallback(UpdateCallback *cb, void *userdata)
	{
		m_UpdateCB=cb;
		m_UpdateCBData=userdata;
	}
	
	void SetPlayerUpdateCallback(CPlayer::UpdateCallback *cb, void *userdata);
	
	std::vector <CPlayer *> m_Players;
	JSObject *m_PlayerArrayJS;
};

class CGame
{
	CWorld m_World;
	CSimulation m_Simulation;
	CGameView m_GameView;
	
	CPlayer *m_pLocalPlayer;
	std::vector<CPlayer *> m_Players;
	uint m_NumPlayers;

	bool m_GameStarted;
	
public:
	CGame();
	~CGame();

	/*
		Initialize all local state and members for playing a game described by
		the attribute class, and start the game.

		Return: 0 on OK - a PSRETURN code otherwise
	*/
	PSRETURN StartGame(CGameAttributes *pGameAttributes);
	
	/*
		Perform all per-frame updates
	*/
	void Update(double deltaTime);
	
	inline CPlayer *GetLocalPlayer()
	{	return m_pLocalPlayer; }
	inline void SetLocalPlayer(CPlayer *pLocalPlayer)
	{	m_pLocalPlayer=pLocalPlayer; }
	
	inline CPlayer *GetPlayer(uint idx)
	{
		if (idx >= 0 && idx <= m_NumPlayers)
			return m_Players[idx];
		else
		{
			debug_warn("Invalid player ID");
			return m_Players[0];
		}
	}

	inline std::vector<CPlayer*>* GetPlayers()
	{	return( &m_Players ); }

	inline uint GetNumPlayers()
	{	return m_NumPlayers; }

	inline bool IsGameStarted()
	{
		return m_GameStarted;
	}

	inline CWorld *GetWorld()
	{	return &m_World; }
	inline CGameView *GetView()
	{	return &m_GameView; }
	inline CSimulation *GetSimulation()
	{	return &m_Simulation; }
};

extern CGame *g_Game;

#endif
