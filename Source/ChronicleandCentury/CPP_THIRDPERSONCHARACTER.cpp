// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_THIRDPERSONCHARACTER.h"

// Sets default values
ACPP_THIRDPERSONCHARACTER::ACPP_THIRDPERSONCHARACTER()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACPP_THIRDPERSONCHARACTER::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPP_THIRDPERSONCHARACTER::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACPP_THIRDPERSONCHARACTER::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

