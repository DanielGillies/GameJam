// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Potion.generated.h"

UCLASS()
class GAMEJAM_API APotion : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APotion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Sprite for potion
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	class UPaperSpriteComponent* Sprite;
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PotionConfig)
	TArray< TSubclassOf<class UPotionGameplayAbility> > PossibleAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PotionConfig)
	TArray< TSubclassOf<class UGameplayEffect> > PossibleEffects;

	TSubclassOf<class UPotionGameplayAbility> SelectRandomAbility();

	TSubclassOf<class UGameplayEffect> SelectRandomEffect();
};
