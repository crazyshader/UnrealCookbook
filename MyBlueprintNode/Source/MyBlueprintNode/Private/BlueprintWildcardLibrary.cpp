// Fill out your copyright notice in the Description page of Project Settings.

#include "BlueprintWildcardLibrary.h"
#include "Engine/Engine.h"

void UBlueprintWildcardLibrary::ShowStructFields(const FDummyStruct& CustomStruct)
{
	check(0);
	return;
}

void UBlueprintWildcardLibrary::Generic_ShowStructFields(const void* StructAddr, const FStructProperty* StructProperty) {
	UScriptStruct* Struct = StructProperty->Struct;
	for (TFieldIterator<FProperty> iter(Struct); iter; ++iter) {

		/*FScreenMessageString NewMessage;
		NewMessage.CurrentTimeDisplayed = 0.0f;
		NewMessage.Key = INDEX_NONE;
		NewMessage.DisplayColor = FColor::Blue;
		NewMessage.TimeToDisplay = 5;
		NewMessage.ScreenMessage = FString::Printf(TEXT("Property: [%s].[%s]"),
			*(Struct->GetName()),
			*(iter->GetName())
		);
		NewMessage.TextScale = FVector2D::UnitVector;*/
		FString Message = FString::Printf(TEXT("Property: [%s].[%s]"), *(Struct->GetName()), *(iter->GetAuthoredName()));
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Blue, Message);
		UE_LOG(LogTemp, Log, TEXT("%s"), *Message);
	}
}

float UBlueprintWildcardLibrary::Array_NumericPropertyAverage(const TArray<int32>& TargetArray, FName PropertyName) {
	// We should never hit these!  They're stubs to avoid NoExport on the class.  Call the Generic* equivalent instead
	check(0);
	return 0.f;
}

float UBlueprintWildcardLibrary::GenericArray_NumericPropertyAverage(const void* TargetArray, const FArrayProperty* ArrayProperty, FName PropertyName) {

	FStructProperty* InnerProperty = CastField<FStructProperty>(ArrayProperty->Inner);
	if (!InnerProperty) {
		UE_LOG(LogTemp, Error, TEXT("Array inner property is NOT a UStruct!"));
		return 0.f;
	}

	UScriptStruct* Struct = InnerProperty->Struct;
	// FindPropertyByName not working for Blueprint Struct
	//UNumericProperty* NumProperty = Cast<UNumericProperty>(Struct->FindPropertyByName(PropertyName));
	FString PropertyNameStr = PropertyName.ToString();
	FNumericProperty* NumProperty = nullptr;
	for (TFieldIterator<FNumericProperty> iter(Struct); iter; ++iter) {
		if (iter->GetAuthoredName() == PropertyNameStr) {
			NumProperty = *iter;
			break;
		}
	}
	if (!NumProperty) {
		UE_LOG(LogTemp, Log, TEXT("Struct property NOT numeric = [%s]"),
			*(PropertyName.ToString())
		);
	}

	FScriptArrayHelper ArrayHelper(ArrayProperty, TargetArray);
	int Count = ArrayHelper.Num();
	float Sum = 0.f;

	if (Count <= 0)
		return 0.f;

	if (NumProperty->IsFloatingPoint())
		for (int i = 0; i < Count; i++) {
			void* ElemPtr = ArrayHelper.GetRawPtr(i);
			const uint8* ValuePtr = NumProperty->ContainerPtrToValuePtr<uint8>(ElemPtr);
			Sum += NumProperty->GetFloatingPointPropertyValue(ValuePtr);

		}
	else if (NumProperty->IsInteger()) {
		for (int i = 0; i < Count; i++) {
			void* ElemPtr = ArrayHelper.GetRawPtr(i);
			const uint8* ValuePtr = NumProperty->ContainerPtrToValuePtr<uint8>(ElemPtr);
			Sum += NumProperty->GetSignedIntPropertyValue(ValuePtr);
		}
	}
	// TODO: else if(enum¿‡–Õ)

	return Sum / Count;
}