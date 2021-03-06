
#pragma once

#include "CoreMinimal.h"

#include "OFSMHandler.generated.h"

UCLASS(Blueprintable, BlueprintType)
class OFSMRUNTIME_API UOFSMHandler : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "OFSM")
		void BindFSM(class UOFSMComponent* Component);

	UFUNCTION(BlueprintCallable, Category = "OFSM")
		class UOFSMTransitionValid* AddBinding(class UOFSMComponent* Component, FName Id);
};
