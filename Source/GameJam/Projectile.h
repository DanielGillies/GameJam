// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class GAMEJAM_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly, Category = "Collision")
	class USphereComponent* CollisionComp;

	UFUNCTION(BlueprintCallable, Category = "Fire")
	void LaunchProjectile(FVector Direciton);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	class UProjectileMovementComponent* MovementComp;
};
