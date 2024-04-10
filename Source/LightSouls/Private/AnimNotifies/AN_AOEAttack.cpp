// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AN_AOEAttack.h"
#include "Characters/BaseCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

void UAN_AOEAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner()))
	{
		FVector AttackLoc = MeshComp->GetSocketLocation(LocationSocketName);

		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(Character);

		TArray<AActor*> HitActors;

		TArray<FHitResult> HitResults;

		UKismetSystemLibrary::SphereTraceMultiForObjects(Character, AttackLoc, AttackLoc, Radius, ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::None, HitResults, true);

		for (FHitResult HitResult : HitResults)
		{
			if (HitResult.GetActor() && HitResult.GetActor()->GetClass()->ImplementsInterface(UCombatable::StaticClass()) && !HitActors.Contains(HitResult.GetActor()))
			{
				HitActors.Add(HitResult.GetActor());
				ICombatable* AttackedActor = Cast<ICombatable>(HitResult.GetActor());
				AttackedActor->OnGetHit(Character->CalculateDamage(), AttackType, AttackIntensityType, Character, HitResult);
			}
		}
	}
}
