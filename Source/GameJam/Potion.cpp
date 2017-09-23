// Fill out your copyright notice in the Description page of Project Settings.

#include "Potion.h"
#include "PaperSpriteComponent.h"
#include "Components/BoxComponent.h"
#include "GameJamCharacter.h"


// Sets default values
APotion::APotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
	Sprite->AttachTo(RootComponent);


}

// Called when the game starts or when spawned
void APotion::BeginPlay()
{
	Super::BeginPlay();

	// Setup pickup handler
	Sprite->OnComponentBeginOverlap.AddDynamic(this, &APotion::OnOverlapBegin);

}

// Called every frame
void APotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APotion::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this))
	{
		if (OtherActor->ActorHasTag(FName("Character.Hero")))
		{
			UE_LOG(LogTemp, Warning, TEXT("GOT IT BITCH"));
		}
	}
}

