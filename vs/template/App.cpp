#include "pch.h"

App::App()
{
	s_pApp = this;
	CPU_CALLBACK_START(OnStart);
	CPU_CALLBACK_UPDATE(OnUpdate);
	CPU_CALLBACK_EXIT(OnExit);
	CPU_CALLBACK_RENDER(OnRender);
}

App::~App()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void App::SpawnAsteroid() {
	cpu_entity* pAsteroid = cpuEngine.CreateEntity();
	pAsteroid->pMesh = &m_meshAsteroid;
	m_asteroids.push_back(pAsteroid);
	spawnPos = 2*XM_PI * cpu::Rand01(seed);
	pAsteroid->transform.OrbitAroundAxis(m_pCenter->transform.pos, CPU_VEC3_UP, 5.f, spawnPos);
	pAsteroid->transform.LookAt(m_pCenter->transform.pos.x, m_pCenter->transform.pos.y, m_pCenter->transform.pos.z);
}

bool App::Collision(cpu_entity* colliding, cpu_entity* collided)
{
	float x1 = colliding->transform.pos.x;
	float y1 = colliding->transform.pos.y;
	float z1 = colliding->transform.pos.z;

	float x2 = collided->transform.pos.x;
	float y2 = collided->transform.pos.y;
	float z2 = collided->transform.pos.z;

	float DistanceX = (x2 - x1) * (x2 - x1);
	float DistanceY = (y2 - y1) * (y2 - y1);
	float DistanceZ = (z2 - z1) * (z2 - z1);

	float DistanceTot = sqrt(DistanceX + DistanceY + DistanceZ);
	
	if (DistanceTot <= colliding->pMesh->radius + collided->pMesh->radius)
		return true;
	else return false;

}
void App::OnStart()
{
	// YOUR CODE HERE
	
	//Rando
	seed = (ui32)timeGetTime();

	//Ressources
	m_textureEarth.Load("earth.png");
	m_meshCenter.CreateSphere(0.15f, 10, 10, CPU_WHITE, CPU_WHITE);
	m_meshPlayer.CreateCube(0.15f, CPU_ORANGE);
	m_meshAsteroid.CreateSphere(0.075f, 10, 10, CPU_GRAY, CPU_GRAY);

	//Textures
	m_materialEarth.pTexture = &m_textureEarth;

	//3D
	m_pCenter = cpuEngine.CreateEntity();
	m_pCenter->pMesh = &m_meshCenter;
	m_pCenter->pMaterial = &m_materialEarth;

	m_pPlayer = cpuEngine.CreateEntity();
	m_materialPlayer.color = cpu::ToColor(255, 125, 0);
	m_pPlayer->pMesh = &m_meshPlayer;
	m_pPlayer->pMaterial = &m_materialPlayer;



	//Others
	cpuEngine.GetCamera()->transform.SetYPR(0.0f, 0.785398163397f);
	cpuEngine.GetCamera()->transform.pos.z = -1.5f;

	m_pPlayer->transform.pos.y = -3.f;
	m_pPlayer->transform.pos.z = 3.f;

	m_pCenter->transform.pos.y = -3.f;
	m_pCenter->transform.pos.z = 1.5f;


}

void App::OnUpdate()
{
	// YOUR CODE HERE

	float dt = cpuTime.delta;
	float time = cpuTime.total;

	if (cpuInput.IsUp())
	{
		SpawnAsteroid();
	}

	//Turn Earth
	m_pCenter->transform.AddYPR(-dt);


	//Move Player
	if (cpuInput.IsLeft())
	{
		m_acce += 0.5f;
		if (m_acce > 3*XM_PI)
			m_acce = 3*XM_PI;
		m_angle += dt * m_acce;
	}
	if (cpuInput.IsRight())
	{
		m_acce += 0.5f;
		if (m_acce > 3*XM_PI)
			m_acce = 3*XM_PI;
		m_angle -= dt * m_acce;
	}
	m_pPlayer->transform.OrbitAroundAxis(m_pCenter->transform.pos, CPU_VEC3_UP, 1.5f, m_angle);
	if (cpuInput.IsLeft() == false && cpuInput.IsRight() == false)
	{
		m_acce = 0;
	}

	//Move Asteroids
	for (auto it = m_asteroids.begin(); it != m_asteroids.end(); ++it)
	{
		cpu_entity* pMissile = *it;
		pMissile->transform.Move(dt * m_AsteroSpeed);
		if (Collision(pMissile, m_pCenter))
			cpuEngine.Release(pMissile);
	}

	// Purge Asteroids
	for (auto it = m_asteroids.begin(); it != m_asteroids.end(); )
	{
		if ((*it)->dead)
			it = m_asteroids.erase(it);
		else
			++it;
	}
}

void App::OnExit()
{
	// YOUR CODE HERE
}

void App::OnRender(int pass)
{
	// YOUR CODE HERE
}

void App::MyPixelShader(cpu_ps_io& io)
{
	// YOUR CODE HERE
	io.color = io.p.color;
}
