
#pragma once

#include "CoreMinimal.h"

#include "OFSM.h"

#include "Components/ActorComponent.h"

#include "OFSMComponent.generated.h"

UCLASS()
class OFSMRUNTIME_API UOFSMComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "OFSM")
		UOFSM* FSM;

	UFUNCTION(BlueprintCallable, Category = "OFSM")
		void InitializeFSM();

	UFUNCTION(BlueprintCallable, Category = "OFSM")
		void UpdateState();

	UFUNCTION(BlueprintCallable, Category = "OFSM")
		void AddVariable(UOFSM_Variable* Variable);

	UPROPERTY()
		UOFSMNode* State;

private:
	UPROPERTY()
		TSet<UOFSM_Variable*> Vars;
};
