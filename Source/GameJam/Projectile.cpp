// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Math/Vector.h"
#include "PaperSpriteComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(FName("Collision Component"), TEXT("SphereComp"));
	CollisionComp->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	RootComponent = CollisionComp;

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(FName("Movement Component"), TEXT("ProjectileComp"));
	MovementComp->UpdatedComponent = RootComponent;
	MovementComp->InitialSpeed = 2500.f;
	MovementComp->MaxSpeed = 10000.0f;
	MovementComp->bAutoActivate = false;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::LaunchProjectile(FVector Direciton)
{
	FVector Velocity = Direciton * MovementComp->InitialSpeed;
	MovementComp->Velocity = Velocity;
	MovementComp->Activate();
}

