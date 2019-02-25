// Fill out your copyright notice in the Description page of Project Settings.

#include "RewindComponent.h"

const float URewindComponent::s_fREFRESH_RATE = 1.0f / 30.0f;
bool  URewindComponent::s_bIsRewindActive = false;

// Sets default values for this component's properties
URewindComponent::URewindComponent()
	:m_pParentMesh(nullptr), s_fDeltaTime(0.0f), s_ui8RemainingRewind(0), s_ui8HeadLocation(0)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void URewindComponent::BeginPlay()
{
	Super::BeginPlay();
	
	for (int i = 0; i < s_ui8TRACKED_VALUES_SIZE; ++i) // Setting initial values by filling array with starting value on creation.
	{
		m_arrTrackedLocation[i] = GetOwner()->GetActorLocation();
		m_arrTrackedRotation[i] = GetOwner()->GetActorQuat();
		if (m_pParentMesh != nullptr)
		{
			m_arrTrackedLinearVelocity[i] = m_pParentMesh->GetPhysicsLinearVelocity();
			m_arrTrackedAngularVelocity[i] = m_pParentMesh->GetPhysicsAngularVelocity();
		}
	}
}

void URewindComponent::TickComponent(float fDeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(fDeltaTime, TickType, ThisTickFunction);
	if (!GetRewind()) SetRewindState(false);


	s_fDeltaTime += fDeltaTime; // If the framerate changes this will allow us to run the following the correct number of times. The speed multiplier can create a speedup or slowmo effect.
	//GetOwner()->GetRootComponent()->SetMobility(EComponentMobility::Stationary);
	//GetOwner()->GetRootComponent()->SetMobility(EComponentMobility::Movable);

	while (s_fDeltaTime >= s_fREFRESH_RATE) // This loop insures that our rewind is FPS independant. 
	{
		s_fDeltaTime -= s_fREFRESH_RATE; // If the delta time is too hight this will cause the loop to run twice per frame. If it's too low it wont run at all.

		if (s_bIsRewindActive)	// After incrementing set new position.
		{
			GetOwner()->SetActorLocationAndRotation(m_arrTrackedLocation[s_ui8HeadLocation], m_arrTrackedRotation[s_ui8HeadLocation], false, nullptr, ETeleportType::TeleportPhysics);
			if (m_pParentMesh != nullptr)
			{
				m_pParentMesh->SetPhysicsLinearVelocity(m_arrTrackedLinearVelocity[s_ui8HeadLocation]);
				m_pParentMesh->SetPhysicsAngularVelocity(m_arrTrackedAngularVelocity[s_ui8HeadLocation]);
			}
		}
		if (s_bIsRewindActive) // If active decrement loop.
		{
			if (s_ui8RemainingRewind)
			{
				s_ui8HeadLocation ? s_ui8HeadLocation-- : s_ui8HeadLocation = s_ui8TRACKED_VALUES_SIZE - 1;				
				--s_ui8RemainingRewind;
			}			
		}
		else // If inactive increment loop.
		{
			s_ui8HeadLocation == s_ui8TRACKED_VALUES_SIZE - 1 ? s_ui8HeadLocation = 0 : s_ui8HeadLocation++;
			if (s_ui8RemainingRewind != s_ui8TRACKED_VALUES_SIZE)// Should only go up to 255.				
				++s_ui8RemainingRewind;
		}
		if (!s_bIsRewindActive) // Record new position after decrementing.
		{
			m_arrTrackedLocation[s_ui8HeadLocation] = GetOwner()->GetActorLocation();
			m_arrTrackedRotation[s_ui8HeadLocation] = GetOwner()->GetActorQuat();
			if (m_pParentMesh != nullptr)
			{
				m_arrTrackedLinearVelocity[s_ui8HeadLocation] = m_pParentMesh->GetPhysicsLinearVelocity();
				m_arrTrackedAngularVelocity[s_ui8HeadLocation] = m_pParentMesh->GetPhysicsAngularVelocity();
			}
		}
	}
}

void URewindComponent::SetRewindState(bool bNewRewindState)
{	
	s_bIsRewindActive = bNewRewindState ? true : false;
}

uint8 URewindComponent::GetRewind()
{
	return s_ui8RemainingRewind;
}

void URewindComponent::SetRewindMesh(UStaticMeshComponent* mesh)
{
	m_pParentMesh = mesh;
}
