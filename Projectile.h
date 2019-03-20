#pragma once
#include <SDL.h>
#include <vector>
#include "AnimatedObject.h"
#include "Globals.cpp"
#include "Calculate.h"
struct ProjectileDetails
{
	SDL_Rect m_textureInfo;
	SDL_Rect m_location;
	SDL_Point m_totalRowColumn;
	SDL_Point movementPoint;
	Direction m_movementDirection;
	std::pair <double, double> Velocity;
	double Rotation;
};

class Projectile : public AnimatedObject
{
public:
	Projectile();
	Projectile(ProjectileDetails details);
	~Projectile();
	void Update(int* _bulletCount, SDL_Rect * camera, std::vector<SDL_Rect> solids);
	double GetRotation() const;
	void SetRotation(double rotation);
	Direction GetMovementDirection(); 

private:
	Direction MovementDirection;
	SDL_Point MovementPoint;
	std::pair <double, double> Velocity;
	int MovementSpeed;
	double Rotation;
};

class ProjectileManager
{
public:
	ProjectileManager();
	ProjectileManager(int maxNumberOfProjectiles);
	~ProjectileManager();

	void Update(SDL_Rect * camera, std::vector<SDL_Rect> solids);
	void Draw(SDL_Renderer* renderer, Texture* texture, SDL_Rect * camera);
	bool CheckCollision(SDL_Rect* destinationRec);
	void FireProjectile(SDL_Rect* startLocation, Direction movementDirection);
	void FireProjectile(SDL_Rect* startLocation, SDL_Point movementDirection, double rotation);
	void SetProjectileDetails(SDL_Rect* textureInfo, SDL_Point* rowCol);
	int GetDamage() const { return Damage; };
	void SetDamage(int damage); 
	void Clear();
private:
	Texture m_projectileTexture;
	int MaxNumberOfProjectiles;
	int BulletCount;
	std::vector<Projectile> Projectiles;
	ProjectileDetails Details;
	int Damage = 1;
	int FirstDeadProjectile();
	Calculate Calculator;

	void AddProjectile(ProjectileDetails projectileDetails);
};