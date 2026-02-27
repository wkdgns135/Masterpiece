// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerMovementComponent.h"


UMPlayerMovementComponent::UMPlayerMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UMPlayerMovementComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UMPlayerMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

