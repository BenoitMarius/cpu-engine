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
	spawnPos = 2 * XM_PI * cpu::Rand01(seed);
	pAsteroid->transform.OrbitAroundAxis(m_pCenter->transform.pos, CPU_VEC3_UP, 5.f, spawnPos);
	pAsteroid->transform.LookAt(m_pCenter->transform.pos.x, m_pCenter->transform.pos.y, m_pCenter->transform.pos.z);


	cpu_particle_emitter* pEmitter = cpuEngine.CreateParticleEmitter();
	cpuEngine.GetParticlePhysics()->gy = -0.5f;
	pEmitter->rate = 0.001f;
	pEmitter->durationMin = 3.f;
	pEmitter->durationMax = 8.f;
	pEmitter->spread = 0.5f;
	pEmitter->colorMin = cpu::ToColor(255, 0, 0);
	pEmitter->colorMax = cpu::ToColor(255, 128, 0);
	m_Emitters.push_back(pEmitter);
}

void App::ExplodeEarth()
{
	m_pEarthExplosion = cpuEngine.CreateParticleEmitter();
	cpuEngine.GetParticlePhysics()->gy = -0.5f;
	m_pEarthExplosion->rate = 0.03f;
	m_pEarthExplosion->spread = 3.f;
	m_pEarthExplosion->colorMin = cpu::ToColor(255, 0, 0);
	m_pEarthExplosion->colorMax = cpu::ToColor(255, 200, 0);
	m_pEarthExplosion->pos = m_pCenter->transform.pos;
	exploding = true;
	ExplosionCDTimer = ExplosionCD;


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
void App::RestartGame()
{
	score = 0;
	HP = 5;
	m_angle = 0.f;
	currentState = Gamestate::Game;
}
void App::OnStart()
{
	// YOUR CODE HERE

	//Rando
	seed = (ui32)timeGetTime();

	m_font.Create(cpuDevice.GetHeight() <= 512 ? 14 : 28);

	//Ressources
	m_textureEarth.Load("earth.png");
	m_textureAxel.Load("miniaxl.png");
	m_textureEliott.Load("minielliot.png");
	m_meshCenter.CreateSphere(0.15f, 10, 10, CPU_WHITE, CPU_WHITE);
	m_meshPlayer.CreateSphere(0.15f, 10, 10, CPU_WHITE, CPU_WHITE);

	m_meshAsteroid.CreateSphere(0.075f, 10, 10, CPU_GRAY, CPU_GRAY);

	//Textures
	m_materialEarth.pTexture = &m_textureEarth;
	m_materialPlayer.pTexture = &m_textureAxel;
	m_materialClone.pTexture = &m_textureEliott;

	//3D
	m_pCenter = cpuEngine.CreateEntity();
	m_pCenter->pMesh = &m_meshCenter;
	m_pCenter->pMaterial = &m_materialEarth;

	m_pPlayer = cpuEngine.CreateEntity();
	m_pPlayer->pMesh = &m_meshPlayer;
	m_pPlayer->pMaterial = &m_materialPlayer;

	//Particule
	cpuEngine.GetParticleData()->Create(2000000);

	//Others
	cpuEngine.GetCamera()->transform.SetYPR(0.0f, 0.785398163397f);
	cpuEngine.GetCamera()->transform.pos.z = -1.5f;

	m_pPlayer->transform.AddYPR(-XM_PI * 0.5,0, -XM_PI * 0.25);


	m_pCenter->transform.pos.y = -3.f;
	m_pCenter->transform.pos.z = 1.5f;


}

void App::OnUpdate()
{
	// YOUR CODE HERE
	float dt = cpuTime.delta;
	float time = cpuTime.total;
	

	switch (currentState)
	{
	case Gamestate::Pause :
		break;
	case Gamestate::GameOver:
		if (cpuInput.IsRetryPressed())
		{
			RestartGame();
		}
		break;

	case Gamestate::Game :

		spawnCDTimer -= dt;
		spawnCD = 3.f - (score / 10) * 0.5f;
		if (spawnCD <= 0.3f)
			spawnCD = 0.3f;
		if (exploding)
		{
			ExplosionCDTimer -= dt;
			if (ExplosionCDTimer <= 0)
			{
				exploding = false;
				ExplosionCDTimer = ExplosionCD;
				cpuEngine.Release(m_pEarthExplosion);
			}
		}
		if (spawnCDTimer <= 0)
		{
			SpawnAsteroid();
			spawnCDTimer += spawnCD;
		}


		//Turn Earth
		m_pCenter->transform.AddYPR(-dt);



		//Move Player
		if (cpuInput.IsLeft())
		{
			m_acce += 0.5f;
			if (m_acce > 3 * XM_PI)
				m_acce = 3 * XM_PI;
			m_angle += dt * m_acce;
		}
		if (cpuInput.IsRight())
		{
			m_acce += 0.5f;
			if (m_acce > 3 * XM_PI)
				m_acce = 3 * XM_PI;
			m_angle -= dt * m_acce;
		}
		m_pPlayer->transform.OrbitAroundAxis(m_pCenter->transform.pos, CPU_VEC3_UP, 0.75f, m_angle);
		if (cpuInput.IsLeft() == false && cpuInput.IsRight() == false)
		{
			m_acce = 0;
		}

		if (cpuInput.IsSpace() && cloneActive == false)
		{
			m_pClone = cpuEngine.CreateEntity();
			m_pClone->pMesh = &m_meshPlayer;
			m_pClone->pMaterial = &m_materialClone;
			m_pClone->transform.pos = m_pPlayer->transform.pos;
			m_pClone->transform.AddYPR(-XM_PI * 0.5, 0, -XM_PI * 0.25);
			cloneActive = true;
		}
		if (cloneActive)
		{
			m_cloneDurationTimer += dt;
			m_pClone->transform.OrbitAroundAxis(m_pCenter->transform.pos, CPU_VEC3_UP, 0.75f, m_angle+XM_PI);
			/*if (m_cloneDurationTimer >= m_cloneDuration)
			{

			}*/
		}

		//Move Asteroids
		auto ut = m_Emitters.begin();
		for (auto it = m_asteroids.begin(); it != m_asteroids.end(); ++it, ++ut)
		{
			cpu_entity* pMissile = *it;

			cpu_particle_emitter* pEmitter = *ut;
			pEmitter->pos = pMissile->transform.pos;
			pEmitter->dir = pMissile->transform.dir;
			pEmitter->dir.x = -pEmitter->dir.x;
			pEmitter->dir.y = -pEmitter->dir.y;
			pEmitter->dir.z = -pEmitter->dir.z;

			pMissile->transform.Move(dt * m_AsteroSpeed);

			if (Collision(pMissile, m_pCenter))
			{
				cpuEngine.Release(pMissile);
				HP--;
				ExplodeEarth();
			}

			if (Collision(pMissile, m_pPlayer))
			{
				cpuEngine.Release(pMissile);
				score++;
				currentscalePlayer += 0.0005;
				m_pPlayer->transform.Scale(currentscalePlayer);
			}
			if(cloneActive)
				if(Collision(pMissile, m_pClone))
			{
					cpuEngine.Release(pMissile);
					score++;
					currentscaleClone += 0.0005;
					m_pClone->transform.Scale(currentscaleClone);
			}
		}

		// Purge Asteroids
		for (auto it = m_asteroids.begin(); it != m_asteroids.end(); )
		{
			if ((*it)->dead)
				it = m_asteroids.erase(it);
			else
				++it;
		}

		break;


	}

	//TO DO
	// 
	////Update Highscore 
	//FILE* fptr;
	//errno_t err;
	//err = fopen_s(&fptr, "highscore.txt", "w");

	

	if (HP <= 0)
		currentState = Gamestate::GameOver;

	if (cpuInput.IsPausePressed())
	{
		if (currentState == Gamestate::Pause)
			currentState = Gamestate::Game;
		else
			currentState = Gamestate::Pause;
	}

	// Quit
	if (cpuInput.IsBackPressed())
		cpuEngine.Quit();
}

void App::OnExit()
{
	// YOUR CODE HERE
}

void App::OnRender(int pass)
{
	// YOUR CODE HERE
	switch (pass)
	{
	case CPU_PASS_PARTICLE_BEGIN:
	{
		// Blur particles
		//cpuEngine.SetRT(m_rts[0]);
		//cpuEngine.ClearColor();
		break;
	}
	case CPU_PASS_PARTICLE_END:
	{
		// Blur particles
		//cpuEngine.Blur(10);
		//cpuEngine.SetMainRT();
		//cpuEngine.AlphaBlend(m_rts[0]);
		break;
	}
	case CPU_PASS_UI_END:
	{
		std::string scoretext = "score : " + std::to_string(score);
		std::string HPtext = "HP : " + std::to_string(HP);
		std::string GameOvertext = "Game Over";
		XMFLOAT3 tint = { 1.0f, 1.0f, 0.8f };
		cpuDevice.DrawText(&m_font, scoretext.c_str(), (int)(cpuDevice.GetWidth() * 0.5f - 100), 10, CPU_TEXT_CENTER, &tint);
		cpuDevice.DrawText(&m_font, HPtext.c_str(), (int)(cpuDevice.GetWidth() * 0.5f + 100), 10, CPU_TEXT_CENTER, &tint);
		if(currentState == Gamestate::GameOver)
		cpuDevice.DrawText(&m_font, GameOvertext.c_str(), (int)(cpuDevice.GetWidth() * 0.5f), cpuDevice.GetHeight() * 0.5f, CPU_TEXT_CENTER, &tint);
		break;
	}
	}

}

void App::MyPixelShader(cpu_ps_io& io)
{
	// YOUR CODE HERE
	io.color = io.p.color;
}
