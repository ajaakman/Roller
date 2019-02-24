// Fill out your copyright notice in the Description page of Project Settings.

#include "RewindBox.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "RewindComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"

// Sets default values
ARewindBox::ARewindBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Game/Meshes/Cube"));

	// Create mesh component for the ball
	Cube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball0"));
	Cube->SetStaticMesh(CubeMesh.Object);
	Cube->BodyInstance.SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	Cube->SetSimulatePhysics(true);
	Cube->SetAngularDamping(0.1f);
	Cube->SetLinearDamping(0.1f);
	Cube->BodyInstance.MassScale = 3.5f;
	Cube->BodyInstance.MaxAngularVelocity = 800.0f;
	Cube->SetNotifyRigidBodyCollision(true);
	RootComponent = Cube;

	m_Rewinder = CreateDefaultSubobject<URewindComponent>(TEXT("Rewinder"));
}

void ARewindBox::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
}

// Called when the game starts or when spawned
void ARewindBox::BeginPlay()
{
	Super::BeginPlay();
	m_Rewinder->SetRewindMesh(Cube);
}

// Called every frame
void ARewindBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

