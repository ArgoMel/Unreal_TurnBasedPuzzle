// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowableStone.h"

#include "NodeBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TurnBasedPuzzle/Characters/HeroCharacter.h"

// Sets default values
AThrowableStone::AThrowableStone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	StoneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Stone Mesh"));
	StoneMesh->SetupAttachment(RootComponent);
	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
	SphereCollider->SetupAttachment(RootComponent);
	StoneMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Comp"));
	ProjectileMovementComp->InitialSpeed = 500.0f;
	ProjectileMovementComp->MaxSpeed = 500.0f;
	ProjectileMovementComp->HomingAccelerationMagnitude = 1000.0f;
	ProjectileMovementComp->Velocity = FVector::Zero();
	ProjectileMovementComp->ProjectileGravityScale = 0.0f;
	
}

// Called when the game starts or when spawned
void AThrowableStone::BeginPlay()
{
	Super::BeginPlay();
	SphereCollider->OnComponentBeginOverlap.AddDynamic(this,&AThrowableStone::OnSphereColliderBeginOverlap);
	
}

// Called every frame
void AThrowableStone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AThrowableStone::ThrowStone(AActor* Actor, FVector InVelocity)
{
	ProjectileMovementComp->bIsHomingProjectile = true;
	ProjectileMovementComp->Velocity = InVelocity;
	ProjectileMovementComp->HomingTargetComponent = Actor->GetRootComponent();
	ProjectileMovementComp->HomingAccelerationMagnitude = 2000.0f;
	
}


void AThrowableStone::OnSphereColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ANodeBase* NodeBase = Cast<ANodeBase>(OtherActor))
	{
		if (AHeroCharacter* HeroCharacter = Cast<AHeroCharacter>(GetOwner()))
		{
			HeroCharacter->bCanClickNode = true;
			HeroCharacter->ReportNoise();
			if (Cue_Impact)
			{
				UGameplayStatics::PlaySound2D(GetWorld(),Cue_Impact);
			}
			Destroy();
		}
	}
}