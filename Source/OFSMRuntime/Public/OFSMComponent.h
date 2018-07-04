
#pragma once

#include "CoreMinimal.h"

#include "OFSM.h"

#include "Components/ActorComponent.h"

#include "OFSMComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTransitionValid, UOFSMTransitionValid*, Source, UOFSMEdge*, Transition);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateChanged, UOFSMNode*, NewNode, UOFSMEdge*, FollowedTransition);

UCLASS(BlueprintType)
class OFSMRUNTIME_API UOFSMTransitionValid : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable, Category = "OFSM Transition")
		FTransitionValid Validity;

	bool Evaluate(UOFSMEdge* Edge);

	UFUNCTION(BlueprintCallable, Category = "OFSM Transition")
		void ReturnValue(bool Valid);

private:
	bool Curr;
};

UCLASS(Blueprintable, BlueprintType, ClassGroup=(OFSM), meta = (BlueprintSpawnableComponent))
class OFSMRUNTIME_API UOFSMComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "OFSM")
		UOFSM* FSM;

	UPROPERTY(BlueprintReadWrite, Category = "OFSM")
		UOFSMNode* State;

	UPROPERTY(BlueprintReadOnly, Category = "OFSM")
		UOFSMHandler* Handler;

	UPROPERTY(BlueprintReadWrite, Category = "OFSM")
		TMap<FName, UOFSMTransitionValid*> TransitionValidity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OFSM")
		bool DefaultTransitionState;

	UPROPERTY(BlueprintAssignable, Category = "OFSM")
		FStateChanged OnStateChange;

	UFUNCTION(BlueprintCallable, Category = "OFSM")
		void InitializeFSM();

	UFUNCTION(BlueprintCallable, Category = "OFSM")
		void UpdateState();

	UFUNCTION(BlueprintPure, Category = "OFSM")
		UOFSMEdge* GetFirstValidTransition();

	UFUNCTION(BlueprintPure, Category = "OFSM")
		TArray<UOFSMEdge*> GetValidTransitions();
};
