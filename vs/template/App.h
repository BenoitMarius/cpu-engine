#pragma once

enum class Gamestate
{
	Pause,
	Game,
	GameOver,

	Count
	
};

class App
{
public:
	App();
	virtual ~App();

	static App& GetInstance() { return *s_pApp; }

	void OnStart();
	void OnUpdate();
	void OnExit();
	void OnRender(int pass);

	static void MyPixelShader(cpu_ps_io& io);

	void SpawnAsteroid();
	void ExplodeEarth();
	void ExplodeEliott();
	bool Collision(cpu_entity* colliding, cpu_entity* collided);

	void RestartGame();

private:
	inline static App* s_pApp = nullptr;

	ui32 seed;
	cpu_font m_font;
	Gamestate currentState = Gamestate::Game;

	//Player
	cpu_mesh m_meshPlayer;
	cpu_material m_materialPlayer;
	cpu_entity* m_pPlayer;
	cpu_entity* m_pClone;
	int score = 0;
	int HP = 5;
	float currentscalePlayer = 1.f;
	float m_angle = 0.f;
	float m_acce = 0.f;

	//Clone
	cpu_material m_materialClone;
	bool cloneActive = false;
	bool cloneExploding = false;
	bool cloneReady = true;
	float currentscaleClone = 1.f;
	float m_cloneDuration = 10.f;
	float m_cloneDurationTimer = 0.f;
	float m_cloneExplosionCD = 0.3f;
	float m_cloneExplosionCDTimer = 0.f;
	float m_cloneCD = 10.f;
	float m_cloneCDTimer = 10.f;

	//Earth
	cpu_mesh m_meshCenter;
	cpu_entity* m_pCenter;
	cpu_texture m_textureEarth;
	cpu_texture m_textureAxel;
	cpu_texture m_textureEliott;
	cpu_material m_materialEarth;
	bool exploding = false;

	//Asteroids
	cpu_mesh m_meshAsteroid;
	cpu_material m_materialAsteroid;
	std::list<cpu_entity*> m_asteroids;
	float spawnPos;
	float m_AsteroSpeed = 1.f;

	float spawnCD = 3.f;
	float spawnCDTimer = 0.f;

	//Particules
	std::list<cpu_particle_emitter*> m_Emitters;
	cpu_particle_emitter* m_pEarthExplosion;
	cpu_particle_emitter* m_pEliottExplosion;
	float ExplosionCDTimer = 0.6f;
	float ExplosionCD = 0.6f;



};
