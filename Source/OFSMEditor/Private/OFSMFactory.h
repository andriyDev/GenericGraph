#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "OFSMFactory.generated.h"

UCLASS()
class OFSMEDITOR_API UOFSMFactory : public UFactory
{
	GENERATED_BODY()

public:
	UOFSMFactory();
	virtual ~UOFSMFactory();

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
