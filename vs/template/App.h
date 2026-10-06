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
	bool Collision(cpu_entity* colliding, cpu_entity* collided);

private:
	inline static App* s_pApp = nullptr;

	ui32 seed;

	//Player
	cpu_mesh m_meshPlayer;
	cpu_material m_materialPlayer;
	cpu_entity* m_pPlayer;

	float m_angle = 0.f;
	float m_acce = 0.f;

	//Earth
	cpu_mesh m_meshCenter;
	cpu_entity* m_pCenter;
	cpu_texture m_textureEarth;
	cpu_material m_materialEarth;

	//Asteroids
	cpu_mesh m_meshAsteroid;
	cpu_material m_materialAsteroid;
	std::list<cpu_entity*> m_asteroids;
	float spawnPos;
	float m_AsteroSpeed = 1.f;
};
