#pragma once

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
	bool Collision(cpu_entity* colliding, cpu_entity* collided);

private:
	inline static App* s_pApp = nullptr;

	ui32 seed;
	cpu_font m_font;

	//Player
	cpu_mesh m_meshPlayer;
	cpu_material m_materialPlayer;
	cpu_entity* m_pPlayer;
	int score = 0;
	int HP = 5;

	float m_angle = 0.f;
	float m_acce = 0.f;

	//Earth
	cpu_mesh m_meshCenter;
	cpu_entity* m_pCenter;
	cpu_texture m_textureEarth;
	cpu_material m_materialEarth;
	bool exploding = false;

	//Asteroids
	cpu_mesh m_meshAsteroid;
	cpu_material m_materialAsteroid;
	std::list<cpu_entity*> m_asteroids;
	float spawnPos;
	float m_AsteroSpeed = 1.f;

	float spawnCD = 1.f;
	float spawnCDTimer = 0.f;

	//Particules
	std::list<cpu_particle_emitter*> m_Emitters;
	cpu_particle_emitter* m_pEarthExplosion;
	float ExplosionCDTimer = 0.6f;
	float ExplosionCD = 0.6f;



};
