// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TargetComponent.h"

// Sets default values for this component's properties
UTargetComponent::UTargetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UTargetComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UTargetComponent::SetCanBeLockedOn(bool InCanBeLockedOn)
{
	canBeLockedOn = InCanBeLockedOn;

	if (!canBeLockedOn && isLockedOn)
	{
		// TODO Unlock On
	}
}

AActor* UTargetComponent::GetTargetActor() const
{
	return GetOwner();
}

